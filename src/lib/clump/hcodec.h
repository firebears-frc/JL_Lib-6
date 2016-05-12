#ifndef HCODEC_H
#define HCODEC_H

#define MAX_SYMBOLS	(256)
#define MAX_NODES	(2 * MAX_SYMBOLS)
#define CL_HCODEC_BOOK_MAX_BITS	(12)

/** A built-in codebook is used to encode the number of bits for each symbol
 * in a canonical codebook.  Since the number of bits per symbol is skewed to
 * lower numbers, this is an efficient method of encoding a codebook.
 */
static const unsigned short CL_HCODEC_BOOK[MAX_SYMBOLS][2] = {
	[0] = { 0x000, 1 },	/* 0 */
	[1] = { 0x396, 10 },	/* 1110010110 */
	[2] = { 0x0dc, 8 },	/* 11011100 */
	[3] = { 0x06c, 7 },	/* 1101100 */
	[4] = { 0x034, 6 },	/* 110100 */
	[5] = { 0x016, 5 },	/* 10110 */
	[6] = { 0x008, 4 },	/* 1000 */
	[7] = { 0x009, 4 },	/* 1001 */
	[8] = { 0x00a, 4 },	/* 1010 */
	[9] = { 0x017, 5 },	/* 10111 */
	[10] = { 0x018, 5 },	/* 11000 */
	[11] = { 0x019, 5 },	/* 11001 */
	[12] = { 0x035, 6 },	/* 110101 */
	[13] = { 0x06d, 7 },	/* 1101101 */
	[14] = { 0x0dd, 8 },	/* 11011101 */
	[15] = { 0x0de, 8 },	/* 11011110 */
	[16] = { 0x0df, 8 },	/* 11011111 */
	[17] = { 0x0e0, 8 },	/* 11100000 */
	[18] = { 0x1c2, 9 },	/* 111000010 */
	[19] = { 0x1c3, 9 },	/* 111000011 */
	[20] = { 0x1c4, 9 },	/* 111000100 */
	[21] = { 0x1c5, 9 },	/* 111000101 */
	[22] = { 0x1c6, 9 },	/* 111000110 */
	[23] = { 0x1c7, 9 },	/* 111000111 */
	[24] = { 0x1c8, 9 },	/* 111001000 */
	[25] = { 0x1c9, 9 },	/* 111001001 */
	[26] = { 0x1ca, 9 },	/* 111001010 */
	[27] = { 0x397, 10 },	/* 1110010111 */
	[28] = { 0x398, 10 },	/* 1110011000 */
	[29] = { 0x399, 10 },	/* 1110011001 */
	[30] = { 0x734, 11 },	/* 11100110100 */
	[31] = { 0x735, 11 },	/* 11100110101 */
	[32] = { 0x736, 11 },	/* 11100110110 */
	[33] = { 0x737, 11 },	/* 11100110111 */
	[34] = { 0x738, 11 },	/* 11100111000 */
	[35] = { 0x739, 11 },	/* 11100111001 */
	[36] = { 0x73a, 11 },	/* 11100111010 */
	[37] = { 0x73b, 11 },	/* 11100111011 */
	[38] = { 0x73c, 11 },	/* 11100111100 */
	[39] = { 0x73d, 11 },	/* 11100111101 */
	[40] = { 0x73e, 11 },	/* 11100111110 */
	[41] = { 0x73f, 11 },	/* 11100111111 */
	[42] = { 0x740, 11 },	/* 11101000000 */
	[43] = { 0x741, 11 },	/* 11101000001 */
	[44] = { 0x742, 11 },	/* 11101000010 */
	[45] = { 0x743, 11 },	/* 11101000011 */
	[46] = { 0x744, 11 },	/* 11101000100 */
	[47] = { 0x745, 11 },	/* 11101000101 */
	[48] = { 0x746, 11 },	/* 11101000110 */
	[49] = { 0x747, 11 },	/* 11101000111 */
	[50] = { 0x748, 11 },	/* 11101001000 */
	[51] = { 0x749, 11 },	/* 11101001001 */
	[52] = { 0x74a, 11 },	/* 11101001010 */
	[53] = { 0x74b, 11 },	/* 11101001011 */
	[54] = { 0x74c, 11 },	/* 11101001100 */
	[55] = { 0x74d, 11 },	/* 11101001101 */
	[56] = { 0x74e, 11 },	/* 11101001110 */
	[57] = { 0x74f, 11 },	/* 11101001111 */
	[58] = { 0x750, 11 },	/* 11101010000 */
	[59] = { 0x751, 11 },	/* 11101010001 */
	[60] = { 0x752, 11 },	/* 11101010010 */
	[61] = { 0x753, 11 },	/* 11101010011 */
	[62] = { 0x754, 11 },	/* 11101010100 */
	[63] = { 0x755, 11 },	/* 11101010101 */
	[64] = { 0x756, 11 },	/* 11101010110 */
	[65] = { 0x757, 11 },	/* 11101010111 */
	[66] = { 0x758, 11 },	/* 11101011000 */
	[67] = { 0x759, 11 },	/* 11101011001 */
	[68] = { 0x75a, 11 },	/* 11101011010 */
	[69] = { 0x75b, 11 },	/* 11101011011 */
	[70] = { 0x75c, 11 },	/* 11101011100 */
	[71] = { 0x75d, 11 },	/* 11101011101 */
	[72] = { 0x75e, 11 },	/* 11101011110 */
	[73] = { 0x75f, 11 },	/* 11101011111 */
	[74] = { 0x760, 11 },	/* 11101100000 */
	[75] = { 0x761, 11 },	/* 11101100001 */
	[76] = { 0x762, 11 },	/* 11101100010 */
	[77] = { 0x763, 11 },	/* 11101100011 */
	[78] = { 0x764, 11 },	/* 11101100100 */
	[79] = { 0x765, 11 },	/* 11101100101 */
	[80] = { 0x766, 11 },	/* 11101100110 */
	[81] = { 0x767, 11 },	/* 11101100111 */
	[82] = { 0x768, 11 },	/* 11101101000 */
	[83] = { 0x769, 11 },	/* 11101101001 */
	[84] = { 0x76a, 11 },	/* 11101101010 */
	[85] = { 0x76b, 11 },	/* 11101101011 */
	[86] = { 0x76c, 11 },	/* 11101101100 */
	[87] = { 0x76d, 11 },	/* 11101101101 */
	[88] = { 0x76e, 11 },	/* 11101101110 */
	[89] = { 0x76f, 11 },	/* 11101101111 */
	[90] = { 0x770, 11 },	/* 11101110000 */
	[91] = { 0x771, 11 },	/* 11101110001 */
	[92] = { 0x772, 11 },	/* 11101110010 */
	[93] = { 0x773, 11 },	/* 11101110011 */
	[94] = { 0x774, 11 },	/* 11101110100 */
	[95] = { 0x775, 11 },	/* 11101110101 */
	[96] = { 0x776, 11 },	/* 11101110110 */
	[97] = { 0x777, 11 },	/* 11101110111 */
	[98] = { 0x778, 11 },	/* 11101111000 */
	[99] = { 0x779, 11 },	/* 11101111001 */
	[100] = { 0x77a, 11 },	/* 11101111010 */
	[101] = { 0x77b, 11 },	/* 11101111011 */
	[102] = { 0x77c, 11 },	/* 11101111100 */
	[103] = { 0x77d, 11 },	/* 11101111101 */
	[104] = { 0x77e, 11 },	/* 11101111110 */
	[105] = { 0x77f, 11 },	/* 11101111111 */
	[106] = { 0x780, 11 },	/* 11110000000 */
	[107] = { 0x781, 11 },	/* 11110000001 */
	[108] = { 0x782, 11 },	/* 11110000010 */
	[109] = { 0x783, 11 },	/* 11110000011 */
	[110] = { 0x784, 11 },	/* 11110000100 */
	[111] = { 0x785, 11 },	/* 11110000101 */
	[112] = { 0x786, 11 },	/* 11110000110 */
	[113] = { 0x787, 11 },	/* 11110000111 */
	[114] = { 0x788, 11 },	/* 11110001000 */
	[115] = { 0x789, 11 },	/* 11110001001 */
	[116] = { 0x78a, 11 },	/* 11110001010 */
	[117] = { 0x78b, 11 },	/* 11110001011 */
	[118] = { 0x78c, 11 },	/* 11110001100 */
	[119] = { 0x78d, 11 },	/* 11110001101 */
	[120] = { 0x78e, 11 },	/* 11110001110 */
	[121] = { 0x78f, 11 },	/* 11110001111 */
	[122] = { 0x790, 11 },	/* 11110010000 */
	[123] = { 0x791, 11 },	/* 11110010001 */
	[124] = { 0x792, 11 },	/* 11110010010 */
	[125] = { 0x793, 11 },	/* 11110010011 */
	[126] = { 0x794, 11 },	/* 11110010100 */
	[127] = { 0x795, 11 },	/* 11110010101 */
	[128] = { 0x796, 11 },	/* 11110010110 */
	[129] = { 0x797, 11 },	/* 11110010111 */
	[130] = { 0x798, 11 },	/* 11110011000 */
	[131] = { 0x799, 11 },	/* 11110011001 */
	[132] = { 0x79a, 11 },	/* 11110011010 */
	[133] = { 0x79b, 11 },	/* 11110011011 */
	[134] = { 0x79c, 11 },	/* 11110011100 */
	[135] = { 0x79d, 11 },	/* 11110011101 */
	[136] = { 0x79e, 11 },	/* 11110011110 */
	[137] = { 0x79f, 11 },	/* 11110011111 */
	[138] = { 0x7a0, 11 },	/* 11110100000 */
	[139] = { 0x7a1, 11 },	/* 11110100001 */
	[140] = { 0x7a2, 11 },	/* 11110100010 */
	[141] = { 0x7a3, 11 },	/* 11110100011 */
	[142] = { 0x7a4, 11 },	/* 11110100100 */
	[143] = { 0x7a5, 11 },	/* 11110100101 */
	[144] = { 0x7a6, 11 },	/* 11110100110 */
	[145] = { 0x7a7, 11 },	/* 11110100111 */
	[146] = { 0x7a8, 11 },	/* 11110101000 */
	[147] = { 0x7a9, 11 },	/* 11110101001 */
	[148] = { 0x7aa, 11 },	/* 11110101010 */
	[149] = { 0x7ab, 11 },	/* 11110101011 */
	[150] = { 0x7ac, 11 },	/* 11110101100 */
	[151] = { 0x7ad, 11 },	/* 11110101101 */
	[152] = { 0x7ae, 11 },	/* 11110101110 */
	[153] = { 0x7af, 11 },	/* 11110101111 */
	[154] = { 0x7b0, 11 },	/* 11110110000 */
	[155] = { 0x7b1, 11 },	/* 11110110001 */
	[156] = { 0x7b2, 11 },	/* 11110110010 */
	[157] = { 0x7b3, 11 },	/* 11110110011 */
	[158] = { 0x7b4, 11 },	/* 11110110100 */
	[159] = { 0x7b5, 11 },	/* 11110110101 */
	[160] = { 0x7b6, 11 },	/* 11110110110 */
	[161] = { 0x7b7, 11 },	/* 11110110111 */
	[162] = { 0x7b8, 11 },	/* 11110111000 */
	[163] = { 0x7b9, 11 },	/* 11110111001 */
	[164] = { 0x7ba, 11 },	/* 11110111010 */
	[165] = { 0x7bb, 11 },	/* 11110111011 */
	[166] = { 0x7bc, 11 },	/* 11110111100 */
	[167] = { 0x7bd, 11 },	/* 11110111101 */
	[168] = { 0x7be, 11 },	/* 11110111110 */
	[169] = { 0x7bf, 11 },	/* 11110111111 */
	[170] = { 0x7c0, 11 },	/* 11111000000 */
	[171] = { 0x7c1, 11 },	/* 11111000001 */
	[172] = { 0x7c2, 11 },	/* 11111000010 */
	[173] = { 0x7c3, 11 },	/* 11111000011 */
	[174] = { 0x7c4, 11 },	/* 11111000100 */
	[175] = { 0x7c5, 11 },	/* 11111000101 */
	[176] = { 0x7c6, 11 },	/* 11111000110 */
	[177] = { 0x7c7, 11 },	/* 11111000111 */
	[178] = { 0x7c8, 11 },	/* 11111001000 */
	[179] = { 0x7c9, 11 },	/* 11111001001 */
	[180] = { 0x7ca, 11 },	/* 11111001010 */
	[181] = { 0x7cb, 11 },	/* 11111001011 */
	[182] = { 0x7cc, 11 },	/* 11111001100 */
	[183] = { 0x7cd, 11 },	/* 11111001101 */
	[184] = { 0x7ce, 11 },	/* 11111001110 */
	[185] = { 0x7cf, 11 },	/* 11111001111 */
	[186] = { 0x7d0, 11 },	/* 11111010000 */
	[187] = { 0x7d1, 11 },	/* 11111010001 */
	[188] = { 0x7d2, 11 },	/* 11111010010 */
	[189] = { 0x7d3, 11 },	/* 11111010011 */
	[190] = { 0x7d4, 11 },	/* 11111010100 */
	[191] = { 0x7d5, 11 },	/* 11111010101 */
	[192] = { 0x7d6, 11 },	/* 11111010110 */
	[193] = { 0x7d7, 11 },	/* 11111010111 */
	[194] = { 0x7d8, 11 },	/* 11111011000 */
	[195] = { 0x7d9, 11 },	/* 11111011001 */
	[196] = { 0x7da, 11 },	/* 11111011010 */
	[197] = { 0x7db, 11 },	/* 11111011011 */
	[198] = { 0x7dc, 11 },	/* 11111011100 */
	[199] = { 0x7dd, 11 },	/* 11111011101 */
	[200] = { 0x7de, 11 },	/* 11111011110 */
	[201] = { 0x7df, 11 },	/* 11111011111 */
	[202] = { 0x7e0, 11 },	/* 11111100000 */
	[203] = { 0x7e1, 11 },	/* 11111100001 */
	[204] = { 0x7e2, 11 },	/* 11111100010 */
	[205] = { 0x7e3, 11 },	/* 11111100011 */
	[206] = { 0x7e4, 11 },	/* 11111100100 */
	[207] = { 0x7e5, 11 },	/* 11111100101 */
	[208] = { 0x7e6, 11 },	/* 11111100110 */
	[209] = { 0x7e7, 11 },	/* 11111100111 */
	[210] = { 0x7e8, 11 },	/* 11111101000 */
	[211] = { 0x7e9, 11 },	/* 11111101001 */
	[212] = { 0xfd4, 12 },	/* 111111010100 */
	[213] = { 0xfd5, 12 },	/* 111111010101 */
	[214] = { 0xfd6, 12 },	/* 111111010110 */
	[215] = { 0xfd7, 12 },	/* 111111010111 */
	[216] = { 0xfd8, 12 },	/* 111111011000 */
	[217] = { 0xfd9, 12 },	/* 111111011001 */
	[218] = { 0xfda, 12 },	/* 111111011010 */
	[219] = { 0xfdb, 12 },	/* 111111011011 */
	[220] = { 0xfdc, 12 },	/* 111111011100 */
	[221] = { 0xfdd, 12 },	/* 111111011101 */
	[222] = { 0xfde, 12 },	/* 111111011110 */
	[223] = { 0xfdf, 12 },	/* 111111011111 */
	[224] = { 0xfe0, 12 },	/* 111111100000 */
	[225] = { 0xfe1, 12 },	/* 111111100001 */
	[226] = { 0xfe2, 12 },	/* 111111100010 */
	[227] = { 0xfe3, 12 },	/* 111111100011 */
	[228] = { 0xfe4, 12 },	/* 111111100100 */
	[229] = { 0xfe5, 12 },	/* 111111100101 */
	[230] = { 0xfe6, 12 },	/* 111111100110 */
	[231] = { 0xfe7, 12 },	/* 111111100111 */
	[232] = { 0xfe8, 12 },	/* 111111101000 */
	[233] = { 0xfe9, 12 },	/* 111111101001 */
	[234] = { 0xfea, 12 },	/* 111111101010 */
	[235] = { 0xfeb, 12 },	/* 111111101011 */
	[236] = { 0xfec, 12 },	/* 111111101100 */
	[237] = { 0xfed, 12 },	/* 111111101101 */
	[238] = { 0xfee, 12 },	/* 111111101110 */
	[239] = { 0xfef, 12 },	/* 111111101111 */
	[240] = { 0xff0, 12 },	/* 111111110000 */
	[241] = { 0xff1, 12 },	/* 111111110001 */
	[242] = { 0xff2, 12 },	/* 111111110010 */
	[243] = { 0xff3, 12 },	/* 111111110011 */
	[244] = { 0xff4, 12 },	/* 111111110100 */
	[245] = { 0xff5, 12 },	/* 111111110101 */
	[246] = { 0xff6, 12 },	/* 111111110110 */
	[247] = { 0xff7, 12 },	/* 111111110111 */
	[248] = { 0xff8, 12 },	/* 111111111000 */
	[249] = { 0xff9, 12 },	/* 111111111001 */
	[250] = { 0xffa, 12 },	/* 111111111010 */
	[251] = { 0xffb, 12 },	/* 111111111011 */
	[252] = { 0xffc, 12 },	/* 111111111100 */
	[253] = { 0xffd, 12 },	/* 111111111101 */
	[254] = { 0xffe, 12 },	/* 111111111110 */
	[255] = { 0xfff, 12 },	/* 111111111111 */
};

#endif