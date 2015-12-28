../# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Paul Mensonides 2002.
#  *     Distributed under the Boost Software License, Version 1.0. (See
#  *     accompanying file LICENSE_1_0.txt or copy at
#  *     http://www.boost.org/LICENSE_1_0.txt)
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef MSGPACK_PREPROCESSOR_SEQ_ENUM_HPP
# define MSGPACK_PREPROCESSOR_SEQ_ENUM_HPP
#
# include "../cat.hpp"
# include "../config/config.hpp"
# include "size.hpp"
#
# /* MSGPACK_PP_SEQ_ENUM */
#
# if MSGPACK_PP_CONFIG_FLAGS() & MSGPACK_PP_CONFIG_EDG()
#    define MSGPACK_PP_SEQ_ENUM(seq) MSGPACK_PP_SEQ_ENUM_I(seq)
#    define MSGPACK_PP_SEQ_ENUM_I(seq) MSGPACK_PP_CAT(MSGPACK_PP_SEQ_ENUM_, MSGPACK_PP_SEQ_SIZE(seq)) seq
# elif MSGPACK_PP_CONFIG_FLAGS() & MSGPACK_PP_CONFIG_MWCC()
#    define MSGPACK_PP_SEQ_ENUM(seq) MSGPACK_PP_SEQ_ENUM_I(MSGPACK_PP_SEQ_SIZE(seq), seq)
#    define MSGPACK_PP_SEQ_ENUM_I(size, seq) MSGPACK_PP_CAT(MSGPACK_PP_SEQ_ENUM_, size) seq
# else
#    define MSGPACK_PP_SEQ_ENUM(seq) MSGPACK_PP_CAT(MSGPACK_PP_SEQ_ENUM_, MSGPACK_PP_SEQ_SIZE(seq)) seq
# endif
#
# define MSGPACK_PP_SEQ_ENUM_1(x) x
# define MSGPACK_PP_SEQ_ENUM_2(x) x, MSGPACK_PP_SEQ_ENUM_1
# define MSGPACK_PP_SEQ_ENUM_3(x) x, MSGPACK_PP_SEQ_ENUM_2
# define MSGPACK_PP_SEQ_ENUM_4(x) x, MSGPACK_PP_SEQ_ENUM_3
# define MSGPACK_PP_SEQ_ENUM_5(x) x, MSGPACK_PP_SEQ_ENUM_4
# define MSGPACK_PP_SEQ_ENUM_6(x) x, MSGPACK_PP_SEQ_ENUM_5
# define MSGPACK_PP_SEQ_ENUM_7(x) x, MSGPACK_PP_SEQ_ENUM_6
# define MSGPACK_PP_SEQ_ENUM_8(x) x, MSGPACK_PP_SEQ_ENUM_7
# define MSGPACK_PP_SEQ_ENUM_9(x) x, MSGPACK_PP_SEQ_ENUM_8
# define MSGPACK_PP_SEQ_ENUM_10(x) x, MSGPACK_PP_SEQ_ENUM_9
# define MSGPACK_PP_SEQ_ENUM_11(x) x, MSGPACK_PP_SEQ_ENUM_10
# define MSGPACK_PP_SEQ_ENUM_12(x) x, MSGPACK_PP_SEQ_ENUM_11
# define MSGPACK_PP_SEQ_ENUM_13(x) x, MSGPACK_PP_SEQ_ENUM_12
# define MSGPACK_PP_SEQ_ENUM_14(x) x, MSGPACK_PP_SEQ_ENUM_13
# define MSGPACK_PP_SEQ_ENUM_15(x) x, MSGPACK_PP_SEQ_ENUM_14
# define MSGPACK_PP_SEQ_ENUM_16(x) x, MSGPACK_PP_SEQ_ENUM_15
# define MSGPACK_PP_SEQ_ENUM_17(x) x, MSGPACK_PP_SEQ_ENUM_16
# define MSGPACK_PP_SEQ_ENUM_18(x) x, MSGPACK_PP_SEQ_ENUM_17
# define MSGPACK_PP_SEQ_ENUM_19(x) x, MSGPACK_PP_SEQ_ENUM_18
# define MSGPACK_PP_SEQ_ENUM_20(x) x, MSGPACK_PP_SEQ_ENUM_19
# define MSGPACK_PP_SEQ_ENUM_21(x) x, MSGPACK_PP_SEQ_ENUM_20
# define MSGPACK_PP_SEQ_ENUM_22(x) x, MSGPACK_PP_SEQ_ENUM_21
# define MSGPACK_PP_SEQ_ENUM_23(x) x, MSGPACK_PP_SEQ_ENUM_22
# define MSGPACK_PP_SEQ_ENUM_24(x) x, MSGPACK_PP_SEQ_ENUM_23
# define MSGPACK_PP_SEQ_ENUM_25(x) x, MSGPACK_PP_SEQ_ENUM_24
# define MSGPACK_PP_SEQ_ENUM_26(x) x, MSGPACK_PP_SEQ_ENUM_25
# define MSGPACK_PP_SEQ_ENUM_27(x) x, MSGPACK_PP_SEQ_ENUM_26
# define MSGPACK_PP_SEQ_ENUM_28(x) x, MSGPACK_PP_SEQ_ENUM_27
# define MSGPACK_PP_SEQ_ENUM_29(x) x, MSGPACK_PP_SEQ_ENUM_28
# define MSGPACK_PP_SEQ_ENUM_30(x) x, MSGPACK_PP_SEQ_ENUM_29
# define MSGPACK_PP_SEQ_ENUM_31(x) x, MSGPACK_PP_SEQ_ENUM_30
# define MSGPACK_PP_SEQ_ENUM_32(x) x, MSGPACK_PP_SEQ_ENUM_31
# define MSGPACK_PP_SEQ_ENUM_33(x) x, MSGPACK_PP_SEQ_ENUM_32
# define MSGPACK_PP_SEQ_ENUM_34(x) x, MSGPACK_PP_SEQ_ENUM_33
# define MSGPACK_PP_SEQ_ENUM_35(x) x, MSGPACK_PP_SEQ_ENUM_34
# define MSGPACK_PP_SEQ_ENUM_36(x) x, MSGPACK_PP_SEQ_ENUM_35
# define MSGPACK_PP_SEQ_ENUM_37(x) x, MSGPACK_PP_SEQ_ENUM_36
# define MSGPACK_PP_SEQ_ENUM_38(x) x, MSGPACK_PP_SEQ_ENUM_37
# define MSGPACK_PP_SEQ_ENUM_39(x) x, MSGPACK_PP_SEQ_ENUM_38
# define MSGPACK_PP_SEQ_ENUM_40(x) x, MSGPACK_PP_SEQ_ENUM_39
# define MSGPACK_PP_SEQ_ENUM_41(x) x, MSGPACK_PP_SEQ_ENUM_40
# define MSGPACK_PP_SEQ_ENUM_42(x) x, MSGPACK_PP_SEQ_ENUM_41
# define MSGPACK_PP_SEQ_ENUM_43(x) x, MSGPACK_PP_SEQ_ENUM_42
# define MSGPACK_PP_SEQ_ENUM_44(x) x, MSGPACK_PP_SEQ_ENUM_43
# define MSGPACK_PP_SEQ_ENUM_45(x) x, MSGPACK_PP_SEQ_ENUM_44
# define MSGPACK_PP_SEQ_ENUM_46(x) x, MSGPACK_PP_SEQ_ENUM_45
# define MSGPACK_PP_SEQ_ENUM_47(x) x, MSGPACK_PP_SEQ_ENUM_46
# define MSGPACK_PP_SEQ_ENUM_48(x) x, MSGPACK_PP_SEQ_ENUM_47
# define MSGPACK_PP_SEQ_ENUM_49(x) x, MSGPACK_PP_SEQ_ENUM_48
# define MSGPACK_PP_SEQ_ENUM_50(x) x, MSGPACK_PP_SEQ_ENUM_49
# define MSGPACK_PP_SEQ_ENUM_51(x) x, MSGPACK_PP_SEQ_ENUM_50
# define MSGPACK_PP_SEQ_ENUM_52(x) x, MSGPACK_PP_SEQ_ENUM_51
# define MSGPACK_PP_SEQ_ENUM_53(x) x, MSGPACK_PP_SEQ_ENUM_52
# define MSGPACK_PP_SEQ_ENUM_54(x) x, MSGPACK_PP_SEQ_ENUM_53
# define MSGPACK_PP_SEQ_ENUM_55(x) x, MSGPACK_PP_SEQ_ENUM_54
# define MSGPACK_PP_SEQ_ENUM_56(x) x, MSGPACK_PP_SEQ_ENUM_55
# define MSGPACK_PP_SEQ_ENUM_57(x) x, MSGPACK_PP_SEQ_ENUM_56
# define MSGPACK_PP_SEQ_ENUM_58(x) x, MSGPACK_PP_SEQ_ENUM_57
# define MSGPACK_PP_SEQ_ENUM_59(x) x, MSGPACK_PP_SEQ_ENUM_58
# define MSGPACK_PP_SEQ_ENUM_60(x) x, MSGPACK_PP_SEQ_ENUM_59
# define MSGPACK_PP_SEQ_ENUM_61(x) x, MSGPACK_PP_SEQ_ENUM_60
# define MSGPACK_PP_SEQ_ENUM_62(x) x, MSGPACK_PP_SEQ_ENUM_61
# define MSGPACK_PP_SEQ_ENUM_63(x) x, MSGPACK_PP_SEQ_ENUM_62
# define MSGPACK_PP_SEQ_ENUM_64(x) x, MSGPACK_PP_SEQ_ENUM_63
# define MSGPACK_PP_SEQ_ENUM_65(x) x, MSGPACK_PP_SEQ_ENUM_64
# define MSGPACK_PP_SEQ_ENUM_66(x) x, MSGPACK_PP_SEQ_ENUM_65
# define MSGPACK_PP_SEQ_ENUM_67(x) x, MSGPACK_PP_SEQ_ENUM_66
# define MSGPACK_PP_SEQ_ENUM_68(x) x, MSGPACK_PP_SEQ_ENUM_67
# define MSGPACK_PP_SEQ_ENUM_69(x) x, MSGPACK_PP_SEQ_ENUM_68
# define MSGPACK_PP_SEQ_ENUM_70(x) x, MSGPACK_PP_SEQ_ENUM_69
# define MSGPACK_PP_SEQ_ENUM_71(x) x, MSGPACK_PP_SEQ_ENUM_70
# define MSGPACK_PP_SEQ_ENUM_72(x) x, MSGPACK_PP_SEQ_ENUM_71
# define MSGPACK_PP_SEQ_ENUM_73(x) x, MSGPACK_PP_SEQ_ENUM_72
# define MSGPACK_PP_SEQ_ENUM_74(x) x, MSGPACK_PP_SEQ_ENUM_73
# define MSGPACK_PP_SEQ_ENUM_75(x) x, MSGPACK_PP_SEQ_ENUM_74
# define MSGPACK_PP_SEQ_ENUM_76(x) x, MSGPACK_PP_SEQ_ENUM_75
# define MSGPACK_PP_SEQ_ENUM_77(x) x, MSGPACK_PP_SEQ_ENUM_76
# define MSGPACK_PP_SEQ_ENUM_78(x) x, MSGPACK_PP_SEQ_ENUM_77
# define MSGPACK_PP_SEQ_ENUM_79(x) x, MSGPACK_PP_SEQ_ENUM_78
# define MSGPACK_PP_SEQ_ENUM_80(x) x, MSGPACK_PP_SEQ_ENUM_79
# define MSGPACK_PP_SEQ_ENUM_81(x) x, MSGPACK_PP_SEQ_ENUM_80
# define MSGPACK_PP_SEQ_ENUM_82(x) x, MSGPACK_PP_SEQ_ENUM_81
# define MSGPACK_PP_SEQ_ENUM_83(x) x, MSGPACK_PP_SEQ_ENUM_82
# define MSGPACK_PP_SEQ_ENUM_84(x) x, MSGPACK_PP_SEQ_ENUM_83
# define MSGPACK_PP_SEQ_ENUM_85(x) x, MSGPACK_PP_SEQ_ENUM_84
# define MSGPACK_PP_SEQ_ENUM_86(x) x, MSGPACK_PP_SEQ_ENUM_85
# define MSGPACK_PP_SEQ_ENUM_87(x) x, MSGPACK_PP_SEQ_ENUM_86
# define MSGPACK_PP_SEQ_ENUM_88(x) x, MSGPACK_PP_SEQ_ENUM_87
# define MSGPACK_PP_SEQ_ENUM_89(x) x, MSGPACK_PP_SEQ_ENUM_88
# define MSGPACK_PP_SEQ_ENUM_90(x) x, MSGPACK_PP_SEQ_ENUM_89
# define MSGPACK_PP_SEQ_ENUM_91(x) x, MSGPACK_PP_SEQ_ENUM_90
# define MSGPACK_PP_SEQ_ENUM_92(x) x, MSGPACK_PP_SEQ_ENUM_91
# define MSGPACK_PP_SEQ_ENUM_93(x) x, MSGPACK_PP_SEQ_ENUM_92
# define MSGPACK_PP_SEQ_ENUM_94(x) x, MSGPACK_PP_SEQ_ENUM_93
# define MSGPACK_PP_SEQ_ENUM_95(x) x, MSGPACK_PP_SEQ_ENUM_94
# define MSGPACK_PP_SEQ_ENUM_96(x) x, MSGPACK_PP_SEQ_ENUM_95
# define MSGPACK_PP_SEQ_ENUM_97(x) x, MSGPACK_PP_SEQ_ENUM_96
# define MSGPACK_PP_SEQ_ENUM_98(x) x, MSGPACK_PP_SEQ_ENUM_97
# define MSGPACK_PP_SEQ_ENUM_99(x) x, MSGPACK_PP_SEQ_ENUM_98
# define MSGPACK_PP_SEQ_ENUM_100(x) x, MSGPACK_PP_SEQ_ENUM_99
# define MSGPACK_PP_SEQ_ENUM_101(x) x, MSGPACK_PP_SEQ_ENUM_100
# define MSGPACK_PP_SEQ_ENUM_102(x) x, MSGPACK_PP_SEQ_ENUM_101
# define MSGPACK_PP_SEQ_ENUM_103(x) x, MSGPACK_PP_SEQ_ENUM_102
# define MSGPACK_PP_SEQ_ENUM_104(x) x, MSGPACK_PP_SEQ_ENUM_103
# define MSGPACK_PP_SEQ_ENUM_105(x) x, MSGPACK_PP_SEQ_ENUM_104
# define MSGPACK_PP_SEQ_ENUM_106(x) x, MSGPACK_PP_SEQ_ENUM_105
# define MSGPACK_PP_SEQ_ENUM_107(x) x, MSGPACK_PP_SEQ_ENUM_106
# define MSGPACK_PP_SEQ_ENUM_108(x) x, MSGPACK_PP_SEQ_ENUM_107
# define MSGPACK_PP_SEQ_ENUM_109(x) x, MSGPACK_PP_SEQ_ENUM_108
# define MSGPACK_PP_SEQ_ENUM_110(x) x, MSGPACK_PP_SEQ_ENUM_109
# define MSGPACK_PP_SEQ_ENUM_111(x) x, MSGPACK_PP_SEQ_ENUM_110
# define MSGPACK_PP_SEQ_ENUM_112(x) x, MSGPACK_PP_SEQ_ENUM_111
# define MSGPACK_PP_SEQ_ENUM_113(x) x, MSGPACK_PP_SEQ_ENUM_112
# define MSGPACK_PP_SEQ_ENUM_114(x) x, MSGPACK_PP_SEQ_ENUM_113
# define MSGPACK_PP_SEQ_ENUM_115(x) x, MSGPACK_PP_SEQ_ENUM_114
# define MSGPACK_PP_SEQ_ENUM_116(x) x, MSGPACK_PP_SEQ_ENUM_115
# define MSGPACK_PP_SEQ_ENUM_117(x) x, MSGPACK_PP_SEQ_ENUM_116
# define MSGPACK_PP_SEQ_ENUM_118(x) x, MSGPACK_PP_SEQ_ENUM_117
# define MSGPACK_PP_SEQ_ENUM_119(x) x, MSGPACK_PP_SEQ_ENUM_118
# define MSGPACK_PP_SEQ_ENUM_120(x) x, MSGPACK_PP_SEQ_ENUM_119
# define MSGPACK_PP_SEQ_ENUM_121(x) x, MSGPACK_PP_SEQ_ENUM_120
# define MSGPACK_PP_SEQ_ENUM_122(x) x, MSGPACK_PP_SEQ_ENUM_121
# define MSGPACK_PP_SEQ_ENUM_123(x) x, MSGPACK_PP_SEQ_ENUM_122
# define MSGPACK_PP_SEQ_ENUM_124(x) x, MSGPACK_PP_SEQ_ENUM_123
# define MSGPACK_PP_SEQ_ENUM_125(x) x, MSGPACK_PP_SEQ_ENUM_124
# define MSGPACK_PP_SEQ_ENUM_126(x) x, MSGPACK_PP_SEQ_ENUM_125
# define MSGPACK_PP_SEQ_ENUM_127(x) x, MSGPACK_PP_SEQ_ENUM_126
# define MSGPACK_PP_SEQ_ENUM_128(x) x, MSGPACK_PP_SEQ_ENUM_127
# define MSGPACK_PP_SEQ_ENUM_129(x) x, MSGPACK_PP_SEQ_ENUM_128
# define MSGPACK_PP_SEQ_ENUM_130(x) x, MSGPACK_PP_SEQ_ENUM_129
# define MSGPACK_PP_SEQ_ENUM_131(x) x, MSGPACK_PP_SEQ_ENUM_130
# define MSGPACK_PP_SEQ_ENUM_132(x) x, MSGPACK_PP_SEQ_ENUM_131
# define MSGPACK_PP_SEQ_ENUM_133(x) x, MSGPACK_PP_SEQ_ENUM_132
# define MSGPACK_PP_SEQ_ENUM_134(x) x, MSGPACK_PP_SEQ_ENUM_133
# define MSGPACK_PP_SEQ_ENUM_135(x) x, MSGPACK_PP_SEQ_ENUM_134
# define MSGPACK_PP_SEQ_ENUM_136(x) x, MSGPACK_PP_SEQ_ENUM_135
# define MSGPACK_PP_SEQ_ENUM_137(x) x, MSGPACK_PP_SEQ_ENUM_136
# define MSGPACK_PP_SEQ_ENUM_138(x) x, MSGPACK_PP_SEQ_ENUM_137
# define MSGPACK_PP_SEQ_ENUM_139(x) x, MSGPACK_PP_SEQ_ENUM_138
# define MSGPACK_PP_SEQ_ENUM_140(x) x, MSGPACK_PP_SEQ_ENUM_139
# define MSGPACK_PP_SEQ_ENUM_141(x) x, MSGPACK_PP_SEQ_ENUM_140
# define MSGPACK_PP_SEQ_ENUM_142(x) x, MSGPACK_PP_SEQ_ENUM_141
# define MSGPACK_PP_SEQ_ENUM_143(x) x, MSGPACK_PP_SEQ_ENUM_142
# define MSGPACK_PP_SEQ_ENUM_144(x) x, MSGPACK_PP_SEQ_ENUM_143
# define MSGPACK_PP_SEQ_ENUM_145(x) x, MSGPACK_PP_SEQ_ENUM_144
# define MSGPACK_PP_SEQ_ENUM_146(x) x, MSGPACK_PP_SEQ_ENUM_145
# define MSGPACK_PP_SEQ_ENUM_147(x) x, MSGPACK_PP_SEQ_ENUM_146
# define MSGPACK_PP_SEQ_ENUM_148(x) x, MSGPACK_PP_SEQ_ENUM_147
# define MSGPACK_PP_SEQ_ENUM_149(x) x, MSGPACK_PP_SEQ_ENUM_148
# define MSGPACK_PP_SEQ_ENUM_150(x) x, MSGPACK_PP_SEQ_ENUM_149
# define MSGPACK_PP_SEQ_ENUM_151(x) x, MSGPACK_PP_SEQ_ENUM_150
# define MSGPACK_PP_SEQ_ENUM_152(x) x, MSGPACK_PP_SEQ_ENUM_151
# define MSGPACK_PP_SEQ_ENUM_153(x) x, MSGPACK_PP_SEQ_ENUM_152
# define MSGPACK_PP_SEQ_ENUM_154(x) x, MSGPACK_PP_SEQ_ENUM_153
# define MSGPACK_PP_SEQ_ENUM_155(x) x, MSGPACK_PP_SEQ_ENUM_154
# define MSGPACK_PP_SEQ_ENUM_156(x) x, MSGPACK_PP_SEQ_ENUM_155
# define MSGPACK_PP_SEQ_ENUM_157(x) x, MSGPACK_PP_SEQ_ENUM_156
# define MSGPACK_PP_SEQ_ENUM_158(x) x, MSGPACK_PP_SEQ_ENUM_157
# define MSGPACK_PP_SEQ_ENUM_159(x) x, MSGPACK_PP_SEQ_ENUM_158
# define MSGPACK_PP_SEQ_ENUM_160(x) x, MSGPACK_PP_SEQ_ENUM_159
# define MSGPACK_PP_SEQ_ENUM_161(x) x, MSGPACK_PP_SEQ_ENUM_160
# define MSGPACK_PP_SEQ_ENUM_162(x) x, MSGPACK_PP_SEQ_ENUM_161
# define MSGPACK_PP_SEQ_ENUM_163(x) x, MSGPACK_PP_SEQ_ENUM_162
# define MSGPACK_PP_SEQ_ENUM_164(x) x, MSGPACK_PP_SEQ_ENUM_163
# define MSGPACK_PP_SEQ_ENUM_165(x) x, MSGPACK_PP_SEQ_ENUM_164
# define MSGPACK_PP_SEQ_ENUM_166(x) x, MSGPACK_PP_SEQ_ENUM_165
# define MSGPACK_PP_SEQ_ENUM_167(x) x, MSGPACK_PP_SEQ_ENUM_166
# define MSGPACK_PP_SEQ_ENUM_168(x) x, MSGPACK_PP_SEQ_ENUM_167
# define MSGPACK_PP_SEQ_ENUM_169(x) x, MSGPACK_PP_SEQ_ENUM_168
# define MSGPACK_PP_SEQ_ENUM_170(x) x, MSGPACK_PP_SEQ_ENUM_169
# define MSGPACK_PP_SEQ_ENUM_171(x) x, MSGPACK_PP_SEQ_ENUM_170
# define MSGPACK_PP_SEQ_ENUM_172(x) x, MSGPACK_PP_SEQ_ENUM_171
# define MSGPACK_PP_SEQ_ENUM_173(x) x, MSGPACK_PP_SEQ_ENUM_172
# define MSGPACK_PP_SEQ_ENUM_174(x) x, MSGPACK_PP_SEQ_ENUM_173
# define MSGPACK_PP_SEQ_ENUM_175(x) x, MSGPACK_PP_SEQ_ENUM_174
# define MSGPACK_PP_SEQ_ENUM_176(x) x, MSGPACK_PP_SEQ_ENUM_175
# define MSGPACK_PP_SEQ_ENUM_177(x) x, MSGPACK_PP_SEQ_ENUM_176
# define MSGPACK_PP_SEQ_ENUM_178(x) x, MSGPACK_PP_SEQ_ENUM_177
# define MSGPACK_PP_SEQ_ENUM_179(x) x, MSGPACK_PP_SEQ_ENUM_178
# define MSGPACK_PP_SEQ_ENUM_180(x) x, MSGPACK_PP_SEQ_ENUM_179
# define MSGPACK_PP_SEQ_ENUM_181(x) x, MSGPACK_PP_SEQ_ENUM_180
# define MSGPACK_PP_SEQ_ENUM_182(x) x, MSGPACK_PP_SEQ_ENUM_181
# define MSGPACK_PP_SEQ_ENUM_183(x) x, MSGPACK_PP_SEQ_ENUM_182
# define MSGPACK_PP_SEQ_ENUM_184(x) x, MSGPACK_PP_SEQ_ENUM_183
# define MSGPACK_PP_SEQ_ENUM_185(x) x, MSGPACK_PP_SEQ_ENUM_184
# define MSGPACK_PP_SEQ_ENUM_186(x) x, MSGPACK_PP_SEQ_ENUM_185
# define MSGPACK_PP_SEQ_ENUM_187(x) x, MSGPACK_PP_SEQ_ENUM_186
# define MSGPACK_PP_SEQ_ENUM_188(x) x, MSGPACK_PP_SEQ_ENUM_187
# define MSGPACK_PP_SEQ_ENUM_189(x) x, MSGPACK_PP_SEQ_ENUM_188
# define MSGPACK_PP_SEQ_ENUM_190(x) x, MSGPACK_PP_SEQ_ENUM_189
# define MSGPACK_PP_SEQ_ENUM_191(x) x, MSGPACK_PP_SEQ_ENUM_190
# define MSGPACK_PP_SEQ_ENUM_192(x) x, MSGPACK_PP_SEQ_ENUM_191
# define MSGPACK_PP_SEQ_ENUM_193(x) x, MSGPACK_PP_SEQ_ENUM_192
# define MSGPACK_PP_SEQ_ENUM_194(x) x, MSGPACK_PP_SEQ_ENUM_193
# define MSGPACK_PP_SEQ_ENUM_195(x) x, MSGPACK_PP_SEQ_ENUM_194
# define MSGPACK_PP_SEQ_ENUM_196(x) x, MSGPACK_PP_SEQ_ENUM_195
# define MSGPACK_PP_SEQ_ENUM_197(x) x, MSGPACK_PP_SEQ_ENUM_196
# define MSGPACK_PP_SEQ_ENUM_198(x) x, MSGPACK_PP_SEQ_ENUM_197
# define MSGPACK_PP_SEQ_ENUM_199(x) x, MSGPACK_PP_SEQ_ENUM_198
# define MSGPACK_PP_SEQ_ENUM_200(x) x, MSGPACK_PP_SEQ_ENUM_199
# define MSGPACK_PP_SEQ_ENUM_201(x) x, MSGPACK_PP_SEQ_ENUM_200
# define MSGPACK_PP_SEQ_ENUM_202(x) x, MSGPACK_PP_SEQ_ENUM_201
# define MSGPACK_PP_SEQ_ENUM_203(x) x, MSGPACK_PP_SEQ_ENUM_202
# define MSGPACK_PP_SEQ_ENUM_204(x) x, MSGPACK_PP_SEQ_ENUM_203
# define MSGPACK_PP_SEQ_ENUM_205(x) x, MSGPACK_PP_SEQ_ENUM_204
# define MSGPACK_PP_SEQ_ENUM_206(x) x, MSGPACK_PP_SEQ_ENUM_205
# define MSGPACK_PP_SEQ_ENUM_207(x) x, MSGPACK_PP_SEQ_ENUM_206
# define MSGPACK_PP_SEQ_ENUM_208(x) x, MSGPACK_PP_SEQ_ENUM_207
# define MSGPACK_PP_SEQ_ENUM_209(x) x, MSGPACK_PP_SEQ_ENUM_208
# define MSGPACK_PP_SEQ_ENUM_210(x) x, MSGPACK_PP_SEQ_ENUM_209
# define MSGPACK_PP_SEQ_ENUM_211(x) x, MSGPACK_PP_SEQ_ENUM_210
# define MSGPACK_PP_SEQ_ENUM_212(x) x, MSGPACK_PP_SEQ_ENUM_211
# define MSGPACK_PP_SEQ_ENUM_213(x) x, MSGPACK_PP_SEQ_ENUM_212
# define MSGPACK_PP_SEQ_ENUM_214(x) x, MSGPACK_PP_SEQ_ENUM_213
# define MSGPACK_PP_SEQ_ENUM_215(x) x, MSGPACK_PP_SEQ_ENUM_214
# define MSGPACK_PP_SEQ_ENUM_216(x) x, MSGPACK_PP_SEQ_ENUM_215
# define MSGPACK_PP_SEQ_ENUM_217(x) x, MSGPACK_PP_SEQ_ENUM_216
# define MSGPACK_PP_SEQ_ENUM_218(x) x, MSGPACK_PP_SEQ_ENUM_217
# define MSGPACK_PP_SEQ_ENUM_219(x) x, MSGPACK_PP_SEQ_ENUM_218
# define MSGPACK_PP_SEQ_ENUM_220(x) x, MSGPACK_PP_SEQ_ENUM_219
# define MSGPACK_PP_SEQ_ENUM_221(x) x, MSGPACK_PP_SEQ_ENUM_220
# define MSGPACK_PP_SEQ_ENUM_222(x) x, MSGPACK_PP_SEQ_ENUM_221
# define MSGPACK_PP_SEQ_ENUM_223(x) x, MSGPACK_PP_SEQ_ENUM_222
# define MSGPACK_PP_SEQ_ENUM_224(x) x, MSGPACK_PP_SEQ_ENUM_223
# define MSGPACK_PP_SEQ_ENUM_225(x) x, MSGPACK_PP_SEQ_ENUM_224
# define MSGPACK_PP_SEQ_ENUM_226(x) x, MSGPACK_PP_SEQ_ENUM_225
# define MSGPACK_PP_SEQ_ENUM_227(x) x, MSGPACK_PP_SEQ_ENUM_226
# define MSGPACK_PP_SEQ_ENUM_228(x) x, MSGPACK_PP_SEQ_ENUM_227
# define MSGPACK_PP_SEQ_ENUM_229(x) x, MSGPACK_PP_SEQ_ENUM_228
# define MSGPACK_PP_SEQ_ENUM_230(x) x, MSGPACK_PP_SEQ_ENUM_229
# define MSGPACK_PP_SEQ_ENUM_231(x) x, MSGPACK_PP_SEQ_ENUM_230
# define MSGPACK_PP_SEQ_ENUM_232(x) x, MSGPACK_PP_SEQ_ENUM_231
# define MSGPACK_PP_SEQ_ENUM_233(x) x, MSGPACK_PP_SEQ_ENUM_232
# define MSGPACK_PP_SEQ_ENUM_234(x) x, MSGPACK_PP_SEQ_ENUM_233
# define MSGPACK_PP_SEQ_ENUM_235(x) x, MSGPACK_PP_SEQ_ENUM_234
# define MSGPACK_PP_SEQ_ENUM_236(x) x, MSGPACK_PP_SEQ_ENUM_235
# define MSGPACK_PP_SEQ_ENUM_237(x) x, MSGPACK_PP_SEQ_ENUM_236
# define MSGPACK_PP_SEQ_ENUM_238(x) x, MSGPACK_PP_SEQ_ENUM_237
# define MSGPACK_PP_SEQ_ENUM_239(x) x, MSGPACK_PP_SEQ_ENUM_238
# define MSGPACK_PP_SEQ_ENUM_240(x) x, MSGPACK_PP_SEQ_ENUM_239
# define MSGPACK_PP_SEQ_ENUM_241(x) x, MSGPACK_PP_SEQ_ENUM_240
# define MSGPACK_PP_SEQ_ENUM_242(x) x, MSGPACK_PP_SEQ_ENUM_241
# define MSGPACK_PP_SEQ_ENUM_243(x) x, MSGPACK_PP_SEQ_ENUM_242
# define MSGPACK_PP_SEQ_ENUM_244(x) x, MSGPACK_PP_SEQ_ENUM_243
# define MSGPACK_PP_SEQ_ENUM_245(x) x, MSGPACK_PP_SEQ_ENUM_244
# define MSGPACK_PP_SEQ_ENUM_246(x) x, MSGPACK_PP_SEQ_ENUM_245
# define MSGPACK_PP_SEQ_ENUM_247(x) x, MSGPACK_PP_SEQ_ENUM_246
# define MSGPACK_PP_SEQ_ENUM_248(x) x, MSGPACK_PP_SEQ_ENUM_247
# define MSGPACK_PP_SEQ_ENUM_249(x) x, MSGPACK_PP_SEQ_ENUM_248
# define MSGPACK_PP_SEQ_ENUM_250(x) x, MSGPACK_PP_SEQ_ENUM_249
# define MSGPACK_PP_SEQ_ENUM_251(x) x, MSGPACK_PP_SEQ_ENUM_250
# define MSGPACK_PP_SEQ_ENUM_252(x) x, MSGPACK_PP_SEQ_ENUM_251
# define MSGPACK_PP_SEQ_ENUM_253(x) x, MSGPACK_PP_SEQ_ENUM_252
# define MSGPACK_PP_SEQ_ENUM_254(x) x, MSGPACK_PP_SEQ_ENUM_253
# define MSGPACK_PP_SEQ_ENUM_255(x) x, MSGPACK_PP_SEQ_ENUM_254
# define MSGPACK_PP_SEQ_ENUM_256(x) x, MSGPACK_PP_SEQ_ENUM_255
#
# endif
