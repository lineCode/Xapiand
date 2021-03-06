/*
 * Copyright (C) 2015-2018 Dubalu LLC. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "binary_client.h"

#ifdef XAPIAND_CLUSTERING

#include <errno.h>                            // for errno
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sysexits.h>
#include <unistd.h>

#include "error.hh"                           // for error:name, error::description
#include "fs.hh"                              // for delete_files, build_path_index
#include "io.hh"                              // for io::*
#include "length.h"
#include "manager.h"                          // for XapiandManager
#include "metrics.h"                          // for Metrics::metrics
#include "repr.hh"                            // for repr
#include "utype.hh"                           // for toUType


// #undef L_DEBUG
// #define L_DEBUG L_GREY
// #undef L_CALL
// #define L_CALL L_STACKED_DIM_GREY
// #undef L_REPLICATION
// #define L_REPLICATION L_RED
// #undef L_CONN
// #define L_CONN L_GREEN
// #undef L_BINARY_WIRE
// #define L_BINARY_WIRE L_ORANGE
// #undef L_BINARY
// #define L_BINARY L_TEAL
// #undef L_BINARY_PROTO
// #define L_BINARY_PROTO L_TEAL


//
// Xapian binary client
//

BinaryClient::BinaryClient(const std::shared_ptr<Worker>& parent_, ev::loop_ref* ev_loop_, unsigned int ev_flags_, int sock_, double /*active_timeout_*/, double /*idle_timeout_*/, bool cluster_database_)
	: MetaBaseClient<BinaryClient>(std::move(parent_), ev_loop_, ev_flags_, sock_),
	  state(State::INIT_REMOTE),
#ifdef SAVE_LAST_MESSAGES
	  last_message_received('\xff'),
	  last_message_sent('\xff'),
#endif
	  file_descriptor(-1),
	  file_message_type('\xff'),
	  temp_file_template("xapiand.XXXXXX"),
	  cluster_database(cluster_database_),
	  remote_protocol(*this),
	  replication_protocol(*this)
{
	++XapiandManager::binary_clients();

	Metrics::metrics()
		.xapiand_binary_connections
		.Increment();

	L_CONN("New Binary Client in socket %d, %d client(s) of a total of %d connected.", sock_, XapiandManager::binary_clients().load(), XapiandManager::total_clients().load());
}


BinaryClient::~BinaryClient() noexcept
{
	try {
		if (XapiandManager::binary_clients().fetch_sub(1) == 0) {
			L_CRIT("Inconsistency in number of binary clients");
			sig_exit(-EX_SOFTWARE);
		}

		if (file_descriptor != -1) {
			io::close(file_descriptor);
			file_descriptor = -1;
		}

		for (const auto& filename : temp_files) {
			io::unlink(filename.c_str());
		}

		if (!temp_directory.empty()) {
			delete_files(temp_directory.c_str());
		}

		if (is_shutting_down() && !is_idle()) {
			L_INFO("Binary client killed!");
		}

		if (cluster_database) {
			L_CRIT("Cannot synchronize cluster database!");
			sig_exit(-EX_CANTCREAT);
		}
	} catch (...) {
		L_EXC("Unhandled exception in destructor");
	}
}


bool
BinaryClient::is_idle() const
{
	if (!is_waiting() && !is_running() && write_queue.empty()) {
		std::lock_guard<std::mutex> lk(runner_mutex);
		return messages.empty();
	}
	return false;
}


bool
BinaryClient::init_remote() noexcept
{
	L_CALL("BinaryClient::init_remote()");

	std::lock_guard<std::mutex> lk(runner_mutex);

	ASSERT(!running);

	// Setup state...
	state = State::INIT_REMOTE;

	// And start a runner.
	running = true;
	XapiandManager::binary_client_pool()->enqueue(share_this<BinaryClient>());
	return true;
}


bool
BinaryClient::init_replication(const Endpoint &src_endpoint, const Endpoint &dst_endpoint) noexcept
{
	L_CALL("BinaryClient::init_replication(%s, %s)", repr(src_endpoint.to_string()), repr(dst_endpoint.to_string()));

	std::lock_guard<std::mutex> lk(runner_mutex);

	ASSERT(!running);

	// Setup state...
	state = State::INIT_REPLICATION;

	if (replication_protocol.init_replication(src_endpoint, dst_endpoint)) {
		// And start a runner.
		running = true;
		XapiandManager::binary_client_pool()->enqueue(share_this<BinaryClient>());
		return true;
	}
	return false;
}


ssize_t
BinaryClient::on_read(const char *buf, ssize_t received)
{
	L_CALL("BinaryClient::on_read(<buf>, %zu)", received);

	if (received <= 0) {
		return received;
	}

	L_BINARY_WIRE("BinaryClient::on_read: %zd bytes", received);
	ssize_t processed = -buffer.size();
	buffer.append(buf, received);
	while (buffer.size() >= 2) {
		const char *o = buffer.data();
		const char *p = o;
		const char *p_end = p + buffer.size();

		char type = *p++;
		L_BINARY_WIRE("on_read message: %s {state:%s}", repr(std::string(1, type)), StateNames(state));
		switch (type) {
			case SWITCH_TO_REPL: {
				std::lock_guard<std::mutex> lk(runner_mutex);
				state = State::REPLICATION_SERVER;  // Switch to replication protocol
				type = toUType(ReplicationMessageType::MSG_GET_CHANGESETS);
				L_BINARY("Switched to replication protocol");
				break;
			}
			case FILE_FOLLOWS: {
				char path[PATH_MAX];
				if (temp_directory.empty()) {
					if (temp_directory_template.empty()) {
						temp_directory = "/tmp";
					} else {
						strncpy(path, temp_directory_template.c_str(), PATH_MAX);
						build_path_index(temp_directory_template);
						if (io::mkdtemp(path) == nullptr) {
							L_ERR("Directory %s not created: %s (%d): %s", temp_directory_template, error::name(errno), errno, error::description(errno));
							detach();
							return processed;
						}
						temp_directory = path;
					}
				}
				strncpy(path, (temp_directory + "/" + temp_file_template).c_str(), PATH_MAX);
				file_descriptor = io::mkstemp(path);
				temp_files.push_back(path);
				file_message_type = *p++;
				if (file_descriptor == -1) {
					L_ERR("Cannot create temporary file: %s (%d): %s", error::name(errno), errno, error::description(errno));
					detach();
					return processed;
				} else {
					L_BINARY("Start reading file: %s (%d)", path, file_descriptor);
				}
				read_file();
				processed += p - o;
				buffer.clear();
				return processed;
			}
		}

		ssize_t len;
		try {
			len = unserialise_length(&p, p_end, true);
		} catch (const Xapian::SerialisationError) {
			return received;
		}

		if (!closed) {
			std::lock_guard<std::mutex> lk(runner_mutex);
			if (!running) {
				// Enqueue message...
				messages.push_back(Buffer(type, p, len));
				// And start a runner.
				running = true;
				XapiandManager::binary_client_pool()->enqueue(share_this<BinaryClient>());
			} else {
				// There should be a runner, just enqueue message.
				messages.push_back(Buffer(type, p, len));
			}
		}

		buffer.erase(0, p - o + len);
		processed += p - o + len;
	}

	return received;
}


void
BinaryClient::on_read_file(const char *buf, ssize_t received)
{
	L_CALL("BinaryClient::on_read_file(<buf>, %zu)", received);

	L_BINARY_WIRE("BinaryClient::on_read_file: %zd bytes", received);

	io::write(file_descriptor, buf, received);
}


void
BinaryClient::on_read_file_done()
{
	L_CALL("BinaryClient::on_read_file_done()");

	L_BINARY_WIRE("BinaryClient::on_read_file_done");

	io::close(file_descriptor);
	file_descriptor = -1;

	const auto& temp_file = temp_files.back();

	if (!closed) {
		std::lock_guard<std::mutex> lk(runner_mutex);
		if (!running) {
			// Enqueue message...
			messages.push_back(Buffer(file_message_type, temp_file.data(), temp_file.size()));
			// And start a runner.
			running = true;
			XapiandManager::binary_client_pool()->enqueue(share_this<BinaryClient>());
		} else {
			// There should be a runner, just enqueue message.
			messages.push_back(Buffer(file_message_type, temp_file.data(), temp_file.size()));
		}
	}
}


char
BinaryClient::get_message(std::string &result, char max_type)
{
	L_CALL("BinaryClient::get_message(<result>, <max_type>)");

	auto& msg = messages.front();

	char type = msg.type;

#ifdef SAVE_LAST_MESSAGES
	last_message_received.store(type, std::memory_order_relaxed);
#endif

	if (type >= max_type) {
		std::string errmsg("Invalid message type ");
		errmsg += std::to_string(int(type));
		THROW(InvalidArgumentError, errmsg);
	}

	const char *msg_str = msg.dpos();
	size_t msg_size = msg.nbytes();
	result.assign(msg_str, msg_size);

	messages.pop_front();

	return type;
}


void
BinaryClient::send_message(char type_as_char, const std::string &message)
{
	L_CALL("BinaryClient::send_message(<type_as_char>, <message>)");

#ifdef SAVE_LAST_MESSAGES
	last_message_sent.store(type_as_char, std::memory_order_relaxed);
#endif

	std::string buf;
	buf += type_as_char;
	buf += serialise_length(message.size());
	buf += message;
	write(buf);
}


void
BinaryClient::send_file(char type_as_char, int fd)
{
	L_CALL("BinaryClient::send_file(<type_as_char>, <fd>)");

	std::string buf;
	buf += FILE_FOLLOWS;
	buf += type_as_char;
	write(buf);

	MetaBaseClient<BinaryClient>::send_file(fd);
}


void
BinaryClient::operator()()
{
	L_CALL("BinaryClient::operator()()");

	L_CONN("Start running in binary worker...");

	std::unique_lock<std::mutex> lk(runner_mutex);

	switch (state) {
		case State::INIT_REMOTE:
			state = State::REMOTE_SERVER;
			lk.unlock();
			try {
				remote_protocol.msg_update(std::string());
			} catch (...) {
				lk.lock();
				running = false;
				lk.unlock();
				L_CONN("Running in worker ended with an exception.");
				detach();
				throw;
			}
			lk.lock();
			break;
		case State::INIT_REPLICATION:
			state = State::REPLICATION_CLIENT;
			break;
		default:
			break;
	}

	while (!messages.empty() && !closed) {
		switch (state) {
			case State::REMOTE_SERVER: {
				std::string message;
				RemoteMessageType type = static_cast<RemoteMessageType>(get_message(message, static_cast<char>(RemoteMessageType::MSG_MAX)));
				lk.unlock();
				try {

					L_BINARY_PROTO(">> get_message[REMOTE_SERVER] (%s): %s", RemoteMessageTypeNames(type), repr(message));
					remote_protocol.remote_server(type, message);

					auto sent = total_sent_bytes.exchange(0);
					Metrics::metrics()
						.xapiand_remote_protocol_sent_bytes
						.Increment(sent);

					auto received = total_received_bytes.exchange(0);
					Metrics::metrics()
						.xapiand_remote_protocol_received_bytes
						.Increment(received);

				} catch (...) {
					lk.lock();
					running = false;
					lk.unlock();
					L_CONN("Running in worker ended with an exception.");
					detach();
					throw;
				}
				lk.lock();
				break;
			}

			case State::REPLICATION_SERVER: {
				std::string message;
				ReplicationMessageType type = static_cast<ReplicationMessageType>(get_message(message, static_cast<char>(ReplicationMessageType::MSG_MAX)));
				lk.unlock();
				try {

					L_BINARY_PROTO(">> get_message[REPLICATION_SERVER] (%s): %s", ReplicationMessageTypeNames(type), repr(message));
					replication_protocol.replication_server(type, message);

					auto sent = total_sent_bytes.exchange(0);
					Metrics::metrics()
						.xapiand_replication_sent_bytes
						.Increment(sent);

					auto received = total_received_bytes.exchange(0);
					Metrics::metrics()
						.xapiand_replication_received_bytes
						.Increment(received);

				} catch (...) {
					lk.lock();
					running = false;
					lk.unlock();
					L_CONN("Running in worker ended with an exception.");
					detach();
					throw;
				}
				lk.lock();
				break;
			}

			case State::REPLICATION_CLIENT: {
				std::string message;
				ReplicationReplyType type = static_cast<ReplicationReplyType>(get_message(message, static_cast<char>(ReplicationReplyType::REPLY_MAX)));
				lk.unlock();
				try {

					L_BINARY_PROTO(">> get_message[REPLICATION_CLIENT] (%s): %s", ReplicationReplyTypeNames(type), repr(message));
					replication_protocol.replication_client(type, message);

					auto sent = total_sent_bytes.exchange(0);
					Metrics::metrics()
						.xapiand_replication_sent_bytes
						.Increment(sent);

					auto received = total_received_bytes.exchange(0);
					Metrics::metrics()
						.xapiand_replication_received_bytes
						.Increment(received);

				} catch (...) {
					lk.lock();
					running = false;
					lk.unlock();
					L_CONN("Running in worker ended with an exception.");
					detach();
					throw;
				}
				lk.lock();
				break;
			}

			default:
				running = false;
				lk.unlock();
				L_ERR("Unexpected BinaryClient State!");
				stop();
				destroy();
				detach();
				return;
		}
	}

	running = false;
	lk.unlock();

	if (is_shutting_down() && is_idle()) {
		L_CONN("Running in worker ended due shutdown.");
		detach();
		return;
	}

	L_CONN("Running in binary worker ended.");
	redetach();  // try re-detaching if already flagged as detaching
}


std::string
BinaryClient::__repr__() const
{
#ifdef SAVE_LAST_MESSAGES
	auto state_repr = ([this]() -> std::string {
		auto received = last_message_received.load(std::memory_order_relaxed);
		auto sent = last_message_sent.load(std::memory_order_relaxed);
		auto st = state.load(std::memory_order_relaxed);
		switch (st) {
			case State::INIT_REMOTE:
			case State::REMOTE_SERVER:
				return string::format("%s) (%s<->%s",
					StateNames(st),
					RemoteMessageTypeNames(static_cast<RemoteMessageType>(received)),
					RemoteReplyTypeNames(static_cast<RemoteReplyType>(sent)));
			case State::INIT_REPLICATION:
			case State::REPLICATION_CLIENT:
				return string::format("%s) (%s<->%s",
					StateNames(st),
					ReplicationReplyTypeNames(static_cast<ReplicationReplyType>(received)),
					ReplicationMessageTypeNames(static_cast<ReplicationMessageType>(sent)));
			case State::REPLICATION_SERVER:
				return string::format("%s) (%s<->%s",
					StateNames(st),
					ReplicationMessageTypeNames(static_cast<ReplicationMessageType>(received)),
					ReplicationReplyTypeNames(static_cast<ReplicationReplyType>(sent)));
			default:
				return "";
		}
	})();
#else
	auto& state_repr = StateNames(state.load(std::memory_order_relaxed));
#endif
	return string::format("<BinaryClient (%s) {cnt:%ld, sock:%d}%s%s%s%s%s%s%s%s>",
		state_repr,
		use_count(),
		sock,
		is_runner() ? " (runner)" : " (worker)",
		is_running_loop() ? " (running loop)" : " (stopped loop)",
		is_detaching() ? " (deteaching)" : "",
		is_idle() ? " (idle)" : "",
		is_waiting() ? " (waiting)" : "",
		is_running() ? " (running)" : "",
		is_shutting_down() ? " (shutting down)" : "",
		is_closed() ? " (closed)" : "");
}

#endif  /* XAPIAND_CLUSTERING */
