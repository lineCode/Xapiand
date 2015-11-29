/*
 * Copyright (C) 2015 deipi.com LLC and contributors. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include "client_base.h"

#include "utils.h"

#include <cassert>
#include <sys/socket.h>
#include <unistd.h>
#include <memory>

#define BUF_SIZE 4096

#define NO_COMPRESSOR "\01"
#define LZ4_COMPRESSOR "\02"
#define TYPE_COMPRESSOR LZ4_COMPRESSOR

constexpr int WRITE_QUEUE_SIZE = 10;

enum class WR {
	OK,
	ERR,
	RETRY,
	PENDING,
	CLOSED
};

enum class MODE {
	READ_BUF,
	READ_FILE_TYPE,
	READ_FILE
};


class ClientReader : public CompressorBufferReader
{
protected:
	int fd;
	size_t file_size;
	BaseClient *client;
	std::string header;

public:
	ClientReader(BaseClient *client_, int fd_, size_t file_size_, const std::string &header_)
		: fd(fd_),
		  file_size(file_size_),
		  client(client_),
		  header(header_) { }
};


class ClientCompressorReader : public ClientReader {
protected:
	ssize_t begin();
	ssize_t read(char **buf, size_t size);
	ssize_t write(const char *buf, size_t size);
	ssize_t done();

public:
	ClientCompressorReader(BaseClient *client_, int fd_, size_t file_size_, const std::string &header_)
		: ClientReader(client_, fd_, file_size_, header_) { }
};


ssize_t
ClientCompressorReader::begin()
{
	if (!client->write(header)) {
		return -1;
	}
	return 1;
}


ssize_t
ClientCompressorReader::read(char **buf, size_t size)
{
	assert(*buf);
	return ::read(fd, *buf, size);
}


ssize_t
ClientCompressorReader::write(const char *buf, size_t size)
{
	std::string length(encode_length(size));
	if (!client->write(length) || !client->write(buf, size)) {
		return -1;
	}
	return length.size() + size;
}


ssize_t
ClientCompressorReader::done()
{
	std::string length(encode_length(0));
	if (!client->write(length)) {
		return -1;
	}
	return length.size();
}


class ClientDecompressorReader : public ClientReader {
protected:
	ssize_t write(const char *buf, size_t size);
public:
	ClientDecompressorReader(BaseClient *client_, int fd_, size_t file_size_, const std::string &header_)
		: ClientReader(client_, fd_, file_size_, header_) { }
};


ssize_t
ClientDecompressorReader::write(const char *buf, size_t size)
{
	client->on_read_file(buf, size);
	return size;
}


class ClientNoCompressor : public NoCompressor {
public:
	ClientNoCompressor(BaseClient *client_, int fd_=0, size_t file_size_=0) :
		NoCompressor(
			std::make_unique<ClientDecompressorReader>(client_, fd_, file_size_, NO_COMPRESSOR),
			std::make_unique<ClientCompressorReader>(client_, fd_, file_size_, NO_COMPRESSOR)
		) {}
};


class ClientLZ4Compressor : public LZ4Compressor {
public:
	ClientLZ4Compressor(BaseClient *client_, int fd_=0, size_t file_size_=0) :
		LZ4Compressor(
			std::make_unique<ClientDecompressorReader>(client_, fd_, file_size_, LZ4_COMPRESSOR),
			std::make_unique<ClientCompressorReader>(client_, fd_, file_size_, LZ4_COMPRESSOR)
		) {}
};


BaseClient::BaseClient(std::shared_ptr<BaseServer> server_, ev::loop_ref *loop_, int sock_)
	: Worker(std::move(server_), loop_),
	  io_read(*loop),
	  io_write(*loop),
	  async_write(*loop),
	  closed(false),
	  sock(sock_),
	  written(0),
	  read_buffer(new char[BUF_SIZE]),
	  mode(MODE::READ_BUF),
	  write_queue(WRITE_QUEUE_SIZE)
{
	async_write.set<BaseClient, &BaseClient::async_write_cb>(this);
	async_write.start();
	LOG_EV(this, "\tStart async write event\n");

	io_read.set<BaseClient, &BaseClient::io_cb>(this);
	io_read.start(sock, ev::READ);
	LOG_EV(this, "\tStart read event (sock=%d)\n", sock);

	io_write.set<BaseClient, &BaseClient::io_cb>(this);
	io_write.set(sock, ev::WRITE);
	LOG_EV(this, "\tSetup write event (sock=%d)\n", sock);

	int total_clients = ++XapiandServer::total_clients;
	LOG_OBJ(this, "CREATED CLIENT! (%d clients)\n", total_clients);
}


BaseClient::~BaseClient()
{
	destroy();

	delete []read_buffer;

	int total_clients = --XapiandServer::total_clients;
	assert(total_clients >= 0);

	LOG_OBJ(this, "DELETED CLIENT! (%d clients left)\n", total_clients);
}


void
BaseClient::destroy()
{
	close();

	std::unique_lock<std::mutex> lk(qmtx);
	if (sock == -1) {
		return;
	}

	// Stop and free watcher if client socket is closing
	io_read.stop();
	LOG_EV(this, "\tStop read event (sock=%d)\n", sock);

	io_write.stop();
	LOG_EV(this, "\tStop write event (sock=%d)\n", sock);

	::close(sock);
	sock = -1;
	lk.unlock();

	write_queue.finish();
	while (!write_queue.empty()) {
		std::shared_ptr<Buffer> buffer;
		write_queue.pop(buffer, 0);
	}

	LOG_OBJ(this, "DESTROYED CLIENT!\n");

	detach();
}


void
BaseClient::close()
{
	if (closed) {
		return;
	}

	closed = true;
	LOG_OBJ(this, "CLOSED CLIENT!\n");
}


void
BaseClient::io_cb_update()
{
	if (sock != -1) {
		if (write_queue.empty()) {
			if (closed) {
				destroy();
			} else {
				io_write.stop();
				LOG_EV(this, "\tDisable write event (sock=%d)\n", sock);
			}
		} else {
			io_write.start();
			LOG_EV(this, "\tEnable write event (sock=%d)\n", sock);
		}
	}
}


void
BaseClient::io_cb(ev::io &watcher, int revents)
{
	LOG_EV_BEGIN(this, "BaseClient::io_cb:BEGIN\n");
	LOG_EV(this, "%s (sock=%d) %x\n", (revents & EV_ERROR) ? "EV_ERROR" : (revents & EV_WRITE & EV_READ) ? "IO_CB" : (revents & EV_WRITE) ? "WRITE_CB" : (revents & EV_READ) ? "READ_CB" : "IO_CB", sock, revents);

	if (revents & EV_ERROR) {
		LOG_ERR(this, "ERROR: got invalid event (sock=%d): %s\n", sock, strerror(errno));
		destroy();
		LOG_EV_END(this, "BaseClient::io_cb:END\n");
		return;
	}

	assert(sock == watcher.fd || sock == -1);

	if (revents & EV_WRITE) {
		io_cb_write(watcher.fd);
	}

	if (revents & EV_READ) {
		io_cb_read(watcher.fd);
	}

	io_cb_update();
	LOG_EV_END(this, "BaseClient::io_cb:END\n");
}


WR
BaseClient::write_directly(int fd)
{
	if (fd == -1) {
		LOG_ERR(this, "ERROR: write error (sock=%d): Socket already closed!\n", sock);
		LOG_DEBUG(this, "WR:ERR.1: (sock=%d)\n", sock);
		return WR::ERR;
	} else if (!write_queue.empty()) {
		std::shared_ptr<Buffer> buffer = write_queue.front();

		size_t buf_size = buffer->nbytes();
		const char *buf_data = buffer->dpos();

#ifdef MSG_NOSIGNAL
		ssize_t written = ::send(fd, buf_data, buf_size, MSG_NOSIGNAL);
#else
		ssize_t written = ::write(fd, buf_data, buf_size);
#endif

		if (written < 0) {
			if (ignored_errorno(errno, false)) {
				LOG_DEBUG(this, "WR:RETRY: (sock=%d)\n", sock);
				return WR::RETRY;
			} else {
				LOG_ERR(this, "ERROR: write error (sock=%d): %s\n", sock, strerror(errno));
				LOG_DEBUG(this, "WR:ERR.2: (sock=%d)\n", sock);
				return WR::ERR;
			}
		} else if (written == 0) {
			LOG_DEBUG(this, "WR:CLOSED: (sock=%d)\n", sock);
			return WR::CLOSED;
		} else {
			auto str(repr(buf_data, written, true, 500));
			LOG_CONN_WIRE(this, "(sock=%d) <<-- '%s' [%zu] (%zu bytes)\n", sock, str.c_str(), str.size(), written);
			buffer->pos += written;
			if (buffer->nbytes() == 0) {
				if (write_queue.pop(buffer)) {
					if (write_queue.empty()) {
						LOG_DEBUG(this, "WR:OK.1: (sock=%d)\n", sock);
						return WR::OK;
					} else {
						LOG_DEBUG(this, "WR:PENDING.1: (sock=%d)\n", sock);
						return WR::PENDING;
					}
				}
			} else {
				LOG_DEBUG(this, "WR:PENDING.2: (sock=%d)\n", sock);
				return WR::PENDING;
			}
		}
	}
	LOG_DEBUG(this, "WR:OK.2: (sock=%d)\n", sock);
	return WR::OK;
}


bool
BaseClient::_write(int fd, bool async)
{
	WR status;

	do {
		std::unique_lock<std::mutex> lk(qmtx);
		status = write_directly(fd);
		lk.unlock();

		switch (status) {
			case WR::ERR:
			case WR::CLOSED:
				if (!async) {
					io_write.stop();
					LOG_EV(this, "\tDisable write event (sock=%d)\n", sock);
				}
				destroy();
				return false;
			case WR::RETRY:
				if (!async) {
					io_write.start();
					LOG_EV(this, "\tEnable write event (sock=%d)\n", sock);
				} else {
					async_write.send();
				}
				return true;
			default:
				break;
		}
	} while (status != WR::OK);

	if (!async) {
		io_write.stop();
		LOG_EV(this, "\tDisable write event (sock=%d)\n", sock);
	}

	return true;
}

bool
BaseClient::write(const char *buf, size_t buf_size)
{
	if (!write_queue.push(std::make_shared<Buffer>('\0', buf, buf_size))) {
		return false;
	}

	//LOG_CONN_WIRE(this, "(sock=%d) <ENQUEUE> '%s'\n", sock, repr(buf, buf_size).c_str());

	written += 1;

	return _write(sock, true);
}


void
BaseClient::io_cb_write(int fd)
{
	_write(fd, false);
}


void
BaseClient::io_cb_read(int fd)
{
	if (!closed) {
		ssize_t received = ::read(fd, read_buffer, BUF_SIZE);
		const char *buf_end = read_buffer + received;
		const char *buf_data = read_buffer;

		if (received < 0) {
			if (!ignored_errorno(errno, false)) {
				LOG_ERR(this, "ERROR: read error (sock=%d): %s\n", sock, strerror(errno));
				destroy();
				return;
			}
		} else if (received == 0) {
			// The peer has closed its half side of the connection.
			LOG_CONN(this, "Received EOF (sock=%d)!\n", sock);
			destroy();
			return;
		} else {
			auto str(repr(buf_data, received, true, 500));
			LOG_CONN_WIRE(this, "(sock=%d) -->> '%s' [%zu] (%zu bytes)\n", sock, str.c_str(), str.size(), received);

			if (mode == MODE::READ_FILE_TYPE) {
				switch (*buf_data++) {
					case *NO_COMPRESSOR:
						LOG_CONN(this, "Receiving uncompressed file (sock=%d)...\n", sock);
						compressor = std::make_unique<ClientNoCompressor>(this);
						break;
					case *LZ4_COMPRESSOR:
						LOG_CONN(this, "Receiving LZ4 compressed file (sock=%d)...\n", sock);
						compressor = std::make_unique<ClientLZ4Compressor>(this);
						break;
					default:
						LOG_CONN(this, "Received wrong file mode (sock=%d)!\n", sock);
						destroy();
						return;
				}
				received--;
				length_buffer.clear();
				mode = MODE::READ_FILE;
			}

			if (received && mode == MODE::READ_FILE) {
				do {
					if (file_size == -1) {
						if (buf_data) {
							length_buffer.append(buf_data, received);
						}
						buf_data = length_buffer.data();

						buf_end = buf_data + length_buffer.size();
						file_size = decode_length(&buf_data, buf_end, false);

						if (file_size == -1) {
							return;
						}
						block_size = file_size;
						compressor->decompressor->clear();
					}

					const char *file_buf_to_write;
					size_t block_size_to_write;
					size_t buf_left_size = buf_end - buf_data;
					if (block_size < buf_left_size) {
						file_buf_to_write = buf_data;
						block_size_to_write = block_size;
						buf_data += block_size;
						received = buf_left_size - block_size;
					} else {
						file_buf_to_write = buf_data;
						block_size_to_write = buf_left_size;
						buf_data = nullptr;
						received = 0;
					}

					if (block_size_to_write) {
						compressor->decompressor->append(file_buf_to_write, block_size_to_write);
						block_size -= block_size_to_write;
					}
					if (file_size == 0) {
						compressor->decompressor->clear();
						compressor->decompress();

						on_read_file_done();
						mode = MODE::READ_BUF;
						compressor.reset();
					} else if (block_size == 0) {
						compressor->decompress();

						if (buf_data) {
							length_buffer = std::string(buf_data, received);
							buf_data = nullptr;
							received = 0;
						} else {
							length_buffer.clear();
						}

						file_size = -1;
					}
				} while (file_size == -1);
			}

			if (received && mode == MODE::READ_BUF) {
				on_read(buf_data, received);
			}
		}
	}
}


void
BaseClient::async_write_cb(ev::async &, int)
{
	LOG_EV_BEGIN(this, "BaseClient::async_write_cb:BEGIN\n");
	io_cb_update();
	LOG_EV_END(this, "BaseClient::async_write_cb:END\n");
}


void
BaseClient::shutdown()
{
	LOG_OBJ(this, "BaseClient::shutdown()\n");

	::shutdown(sock, SHUT_RDWR);

	Worker::shutdown();

	if (XapiandManager::shutdown_now) {
		LOG_EV(this, "\tSignaled destroy!!\n");
		destroy();
	}
}


void
BaseClient::read_file()
{
	mode = MODE::READ_FILE_TYPE;
	file_size = -1;
}


bool
BaseClient::send_file(int fd)
{
	ssize_t file_size = ::lseek(fd, 0, SEEK_END);
	::lseek(fd, 0, SEEK_SET);

	switch (*TYPE_COMPRESSOR) {
		case *NO_COMPRESSOR:
			compressor = std::make_unique<ClientNoCompressor>(this, fd, file_size);
			break;
		case *LZ4_COMPRESSOR:
			compressor = std::make_unique<ClientLZ4Compressor>(this, fd, file_size);
			break;
	}

	ssize_t compressed = compressor->compress();

	compressor.reset();

	return (compressed == file_size);
}
