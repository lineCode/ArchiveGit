/*
// $Header: /PM8/App/CFIXED.CPP 1     3/03/99 6:04p Gbeddow $
//
// $Workfile: CFIXED.CPP $
// $Revision: 1 $
// $Date: 3/03/99 6:04p $
*/

/*
// Revision History:
//
// $Log: /PM8/App/CFIXED.CPP $
// 
// 1     3/03/99 6:04p Gbeddow
// 
// 2     1/27/99 1:35p Lwilson
// Added rounding to MakeFixed call to provide more consistent results.
// 
//    Rev 1.0   14 Aug 1997 15:18:56   Fred
// Initial revision.
// 
//    Rev 1.0   14 Aug 1997 09:37:32   Fred
// Initial revision.
// 
//    Rev 1.7   17 Jul 1997 17:46:30   Fred
// Added SafeMulDivLong
// 
//    Rev 1.6   29 May 1997 08:43:24   Fred
// Commented out MakeFixed() asserts
// 
//    Rev 1.5   12 May 1997 11:09:04   Fred
// Added MakeFixed() validation to debug build
// 
//    Rev 1.4   12 Mar 1997 15:52:10   Jay
// Rotation, etc.
// 
//    Rev 1.3   21 Jan 1997 09:22:00   Fred
//  
// 
//    Rev 1.2   20 Jan 1997 10:02:26   Fred
// Speedups
// 
//    Rev 1.1   01 Nov 1996 17:11:36   Jay
// Got rid of a lot of warnings.
// 
//    Rev 1.0   14 Mar 1996 13:43:32   Jay
// Initial revision.
// 
//    Rev 1.2   08 Mar 1996 13:53:24   VSI
// fixed stuff for MAC build
// 
//    Rev 1.1   27 Feb 1996 17:04:54   VSI
// _NEWPORTABLE 'C' code replaces .asm -- not yet working
// 
//    Rev 1.0   16 Feb 1996 12:10:32   FRED
// Initial revision.
// 
//    Rev 1.15   22 Sep 1995 16:27:02   JAY
// Fixed inline (32-bit) version of MidPointFixed().
// 
//    Rev 1.14   19 Sep 1995 11:31:56   JAY
// Fixed a bug in the 32-bit in-line assembly.
// 
//    Rev 1.13   14 Sep 1995 09:32:28   JAY
// 32-bit changes
// 
//    Rev 1.12   13 Sep 1995 11:33:30   JAY
// New 32-bit stuff
// 
//    Rev 1.11   28 Jul 1995 12:27:56   JAY
// Added == and != operators to CFixedRect.
// 
//    Rev 1.10   22 Jun 1995 16:06:02   JAY
// Fixed some bugs.
// 
//    Rev 1.9   05 Jun 1995 11:31:30   JAY
// Got rid of DGROUP objects.
// 
//    Rev 1.8   26 May 1995 11:46:22   JAY
// Replaced bsearch() with local binary search code (faster!).
// 
//    Rev 1.7   24 May 1995 12:53:00   JAY
// Fixed ArcTangent extreme case checking.
// 
//    Rev 1.6   13 May 1995 11:14:16   JAY
//  
// 
//    Rev 1.5   10 May 1995 13:12:42   JAY
// Re-instated Macduff's Sine() code that I took out since it works better.
// 
//    Rev 1.4   09 May 1995 08:13:46   JAY
// Fixed a divide-by-zero problem in ArcTangent.
// 
//    Rev 1.3   09 May 1995 08:03:06   JAY
// Added Tangent().
// 
//    Rev 1.2   08 May 1995 07:42:00   FRED
// CFixed == now compares proper number of bytes.
// 
// 
//    Rev 1.1   05 May 1995 16:49:28   JAY
// Exciting new additions to OutlinePaths!
// 
//    Rev 1.0   04 May 1995 17:14:30   JAY
// Initial revision.
*/

#include "stdafx.h"
#include "cfixed.h"
#include "utils.h"
#include <math.h>

// Some useful constants.

static const long D360 = MakeFixed(360);
static const long D270 = MakeFixed(270);
static const long D180 = MakeFixed(180);
static const long D90 = MakeFixed(90);
static const long DOneTenth = MakeFixed(1)/10;

#ifdef _DEBUG
CFixed MakeFixed(long v)
{
	if ((v > 32767) || (v < -32767))
	{
		TRACE(">>>>>>>>>> MakeFixed overflow! %lx\n", v);
		v = (v < 0) ? -32000 : 32000;
//		ASSERT(FALSE);
	}
	return MAKELONG(0, (int)v);
}
CFixed MakeFixed(int v)
{
	if ((v > 32767) || (v < -32767))
	{
		TRACE(">>>>>>>>>> MakeFixed overflow! %x\n", v);
		v = (v < 0) ? -32000 : 32000;
//		ASSERT(FALSE);
	}
	return MAKELONG(0, v);
}
CFixed MakeFixed(int v, unsigned int f)
{
	if ((v > 32767) || (v < -32767))
	{
		TRACE(">>>>>>>>>> MakeFixed overflow! %x\n", v);
		v = (v < 0) ? -32000 : 32000;
//		ASSERT(FALSE);
	}
	return MAKELONG(f, (int)v);
}
CFixed MakeFixed(double v)
{
	if ((v > 32767.0) || (v < -32767.0))
	{
		TRACE(">>>>>>>>>> MakeFixed overflow! %x\n", (int)v);
		v = (v < 0.0) ? -32000.0 : 32000.0;
//		ASSERT(FALSE);
	}
	return (long) ::Round(v*65536.0);
}
double MakeDouble(CFixed f)
{
	return double(f)/65536.0;
}
unsigned FixedFraction(CFixed f)
{
	return (unsigned)LOWORD(f);
}
int FixedInteger(CFixed f)
{
	return (int)(short int)HIWORD(f);
}
int FixedRound(CFixed f)
{
	return (int)(short int)HIWORD((f)+0x8000L);
}
#endif

// The tangent of 89.95.
// This is half way between the largest tangent in our table (89.9) and
// 1 (90.0). We assume we can linearly interpolate, so this is the lower
// cut off for 90 degrees.

static const long DTan8995 = 0x0479ea4d;

static short const BASED_CODE Cosines[901] =
{
	0x4000,0x4000,0x4000,0x4000,0x4000,0x3fff,0x3fff,0x3fff,0x3ffe,0x3ffe,
	0x3ffe,0x3ffd,0x3ffc,0x3ffc,0x3ffb,0x3ffa,0x3ffa,0x3ff9,0x3ff8,0x3ff7,
	0x3ff6,0x3ff5,0x3ff4,0x3ff3,0x3ff2,0x3ff0,0x3fef,0x3fee,0x3fec,0x3feb,
	0x3fea,0x3fe8,0x3fe6,0x3fe5,0x3fe3,0x3fe1,0x3fe0,0x3fde,0x3fdc,0x3fda,
	0x3fd8,0x3fd6,0x3fd4,0x3fd2,0x3fd0,0x3fcd,0x3fcb,0x3fc9,0x3fc7,0x3fc4,
	0x3fc2,0x3fbf,0x3fbd,0x3fba,0x3fb7,0x3fb5,0x3fb2,0x3faf,0x3fac,0x3fa9,
	0x3fa6,0x3fa3,0x3fa0,0x3f9d,0x3f9a,0x3f97,0x3f93,0x3f90,0x3f8d,0x3f89,
	0x3f86,0x3f82,0x3f7f,0x3f7b,0x3f78,0x3f74,0x3f70,0x3f6c,0x3f68,0x3f65,
	0x3f61,0x3f5d,0x3f58,0x3f54,0x3f50,0x3f4c,0x3f48,0x3f43,0x3f3f,0x3f3b,
	0x3f36,0x3f32,0x3f2d,0x3f29,0x3f24,0x3f1f,0x3f1b,0x3f16,0x3f11,0x3f0c,
	0x3f07,0x3f02,0x3efd,0x3ef8,0x3ef3,0x3eee,0x3ee8,0x3ee3,0x3ede,0x3ed8,
	0x3ed3,0x3ecd,0x3ec8,0x3ec2,0x3ebd,0x3eb7,0x3eb1,0x3eac,0x3ea6,0x3ea0,
	0x3e9a,0x3e94,0x3e8e,0x3e88,0x3e82,0x3e7c,0x3e75,0x3e6f,0x3e69,0x3e62,
	0x3e5c,0x3e56,0x3e4f,0x3e49,0x3e42,0x3e3b,0x3e35,0x3e2e,0x3e27,0x3e20,
	0x3e19,0x3e12,0x3e0b,0x3e04,0x3dfd,0x3df6,0x3def,0x3de8,0x3de0,0x3dd9,
	0x3dd2,0x3dca,0x3dc3,0x3dbb,0x3db4,0x3dac,0x3da4,0x3d9d,0x3d95,0x3d8d,
	0x3d85,0x3d7d,0x3d75,0x3d6d,0x3d65,0x3d5d,0x3d55,0x3d4d,0x3d45,0x3d3c,
	0x3d34,0x3d2c,0x3d23,0x3d1b,0x3d12,0x3d0a,0x3d01,0x3cf8,0x3cf0,0x3ce7,
	0x3cde,0x3cd5,0x3ccc,0x3cc3,0x3cba,0x3cb1,0x3ca8,0x3c9f,0x3c96,0x3c8d,
	0x3c83,0x3c7a,0x3c71,0x3c67,0x3c5e,0x3c54,0x3c4b,0x3c41,0x3c37,0x3c2e,
	0x3c24,0x3c1a,0x3c10,0x3c06,0x3bfc,0x3bf2,0x3be8,0x3bde,0x3bd4,0x3bca,
	0x3bc0,0x3bb6,0x3bab,0x3ba1,0x3b96,0x3b8c,0x3b81,0x3b77,0x3b6c,0x3b62,
	0x3b57,0x3b4c,0x3b41,0x3b37,0x3b2c,0x3b21,0x3b16,0x3b0b,0x3b00,0x3af5,
	0x3aea,0x3ade,0x3ad3,0x3ac8,0x3abc,0x3ab1,0x3aa6,0x3a9a,0x3a8f,0x3a83,
	0x3a78,0x3a6c,0x3a60,0x3a54,0x3a49,0x3a3d,0x3a31,0x3a25,0x3a19,0x3a0d,
	0x3a01,0x39f5,0x39e9,0x39dc,0x39d0,0x39c4,0x39b8,0x39ab,0x399f,0x3992,
	0x3986,0x3979,0x396d,0x3960,0x3953,0x3947,0x393a,0x392d,0x3920,0x3913,
	0x3906,0x38f9,0x38ec,0x38df,0x38d2,0x38c5,0x38b8,0x38aa,0x389d,0x3890,
	0x3882,0x3875,0x3867,0x385a,0x384c,0x383f,0x3831,0x3823,0x3815,0x3808,
	0x37fa,0x37ec,0x37de,0x37d0,0x37c2,0x37b4,0x37a6,0x3798,0x3789,0x377b,
	0x376d,0x375f,0x3750,0x3742,0x3733,0x3725,0x3716,0x3708,0x36f9,0x36eb,
	0x36dc,0x36cd,0x36be,0x36af,0x36a1,0x3692,0x3683,0x3674,0x3665,0x3656,
	0x3646,0x3637,0x3628,0x3619,0x3609,0x35fa,0x35eb,0x35db,0x35cc,0x35bc,
	0x35ad,0x359d,0x358e,0x357e,0x356e,0x355e,0x354f,0x353f,0x352f,0x351f,
	0x350f,0x34ff,0x34ef,0x34df,0x34cf,0x34be,0x34ae,0x349e,0x348e,0x347d,
	0x346d,0x345d,0x344c,0x343c,0x342b,0x341a,0x340a,0x33f9,0x33e8,0x33d8,
	0x33c7,0x33b6,0x33a5,0x3394,0x3383,0x3372,0x3361,0x3350,0x333f,0x332e,
	0x331d,0x330c,0x32fa,0x32e9,0x32d8,0x32c6,0x32b5,0x32a3,0x3292,0x3280,
	0x326f,0x325d,0x324b,0x323a,0x3228,0x3216,0x3204,0x31f3,0x31e1,0x31cf,
	0x31bd,0x31ab,0x3199,0x3187,0x3174,0x3162,0x3150,0x313e,0x312c,0x3119,
	0x3107,0x30f4,0x30e2,0x30d0,0x30bd,0x30aa,0x3098,0x3085,0x3073,0x3060,
	0x304d,0x303a,0x3028,0x3015,0x3002,0x2fef,0x2fdc,0x2fc9,0x2fb6,0x2fa3,
	0x2f90,0x2f7d,0x2f69,0x2f56,0x2f43,0x2f30,0x2f1c,0x2f09,0x2ef5,0x2ee2,
	0x2ece,0x2ebb,0x2ea7,0x2e94,0x2e80,0x2e6d,0x2e59,0x2e45,0x2e31,0x2e1e,
	0x2e0a,0x2df6,0x2de2,0x2dce,0x2dba,0x2da6,0x2d92,0x2d7e,0x2d6a,0x2d55,
	0x2d41,0x2d2d,0x2d19,0x2d04,0x2cf0,0x2cdc,0x2cc7,0x2cb3,0x2c9e,0x2c8a,
	0x2c75,0x2c61,0x2c4c,0x2c37,0x2c23,0x2c0e,0x2bf9,0x2be4,0x2bd0,0x2bbb,
	0x2ba6,0x2b91,0x2b7c,0x2b67,0x2b52,0x2b3d,0x2b28,0x2b13,0x2afd,0x2ae8,
	0x2ad3,0x2abe,0x2aa8,0x2a93,0x2a7e,0x2a68,0x2a53,0x2a3d,0x2a28,0x2a12,
	0x29fd,0x29e7,0x29d2,0x29bc,0x29a6,0x2991,0x297b,0x2965,0x294f,0x2939,
	0x2923,0x290e,0x28f8,0x28e2,0x28cc,0x28b6,0x289f,0x2889,0x2873,0x285d,
	0x2847,0x2831,0x281a,0x2804,0x27ee,0x27d7,0x27c1,0x27aa,0x2794,0x277e,
	0x2767,0x2750,0x273a,0x2723,0x270d,0x26f6,0x26df,0x26c9,0x26b2,0x269b,
	0x2684,0x266d,0x2656,0x263f,0x2629,0x2612,0x25fb,0x25e4,0x25cc,0x25b5,
	0x259e,0x2587,0x2570,0x2559,0x2542,0x252a,0x2513,0x24fc,0x24e4,0x24cd,
	0x24b5,0x249e,0x2487,0x246f,0x2458,0x2440,0x2428,0x2411,0x23f9,0x23e2,
	0x23ca,0x23b2,0x239a,0x2383,0x236b,0x2353,0x233b,0x2323,0x230b,0x22f3,
	0x22db,0x22c3,0x22ab,0x2293,0x227b,0x2263,0x224b,0x2233,0x221b,0x2202,
	0x21ea,0x21d2,0x21ba,0x21a1,0x2189,0x2171,0x2158,0x2140,0x2127,0x210f,
	0x20f6,0x20de,0x20c5,0x20ad,0x2094,0x207c,0x2063,0x204a,0x2031,0x2019,
	0x2000,0x1fe7,0x1fce,0x1fb6,0x1f9d,0x1f84,0x1f6b,0x1f52,0x1f39,0x1f20,
	0x1f07,0x1eee,0x1ed5,0x1ebc,0x1ea3,0x1e8a,0x1e71,0x1e57,0x1e3e,0x1e25,
	0x1e0c,0x1df3,0x1dd9,0x1dc0,0x1da7,0x1d8d,0x1d74,0x1d5b,0x1d41,0x1d28,
	0x1d0e,0x1cf5,0x1cdb,0x1cc2,0x1ca8,0x1c8f,0x1c75,0x1c5b,0x1c42,0x1c28,
	0x1c0e,0x1bf5,0x1bdb,0x1bc1,0x1ba7,0x1b8d,0x1b74,0x1b5a,0x1b40,0x1b26,
	0x1b0c,0x1af2,0x1ad8,0x1abe,0x1aa4,0x1a8a,0x1a70,0x1a56,0x1a3c,0x1a22,
	0x1a08,0x19ee,0x19d4,0x19ba,0x199f,0x1985,0x196b,0x1951,0x1936,0x191c,
	0x1902,0x18e7,0x18cd,0x18b3,0x1898,0x187e,0x1863,0x1849,0x182f,0x1814,
	0x17fa,0x17df,0x17c4,0x17aa,0x178f,0x1775,0x175a,0x1740,0x1725,0x170a,
	0x16f0,0x16d5,0x16ba,0x169f,0x1685,0x166a,0x164f,0x1634,0x1619,0x15ff,
	0x15e4,0x15c9,0x15ae,0x1593,0x1578,0x155d,0x1542,0x1527,0x150c,0x14f1,
	0x14d6,0x14bb,0x14a0,0x1485,0x146a,0x144f,0x1434,0x1418,0x13fd,0x13e2,
	0x13c7,0x13ac,0x1391,0x1375,0x135a,0x133f,0x1323,0x1308,0x12ed,0x12d2,
	0x12b6,0x129b,0x127f,0x1264,0x1249,0x122d,0x1212,0x11f6,0x11db,0x11c0,
	0x11a4,0x1189,0x116d,0x1152,0x1136,0x111a,0x10ff,0x10e3,0x10c8,0x10ac,
	0x1090,0x1075,0x1059,0x103e,0x1022,0x1006,0x0feb,0x0fcf,0x0fb3,0x0f97,
	0x0f7c,0x0f60,0x0f44,0x0f28,0x0f0d,0x0ef1,0x0ed5,0x0eb9,0x0e9d,0x0e81,
	0x0e66,0x0e4a,0x0e2e,0x0e12,0x0df6,0x0dda,0x0dbe,0x0da2,0x0d86,0x0d6a,
	0x0d4e,0x0d32,0x0d16,0x0cfa,0x0cde,0x0cc2,0x0ca6,0x0c8a,0x0c6e,0x0c52,
	0x0c36,0x0c1a,0x0bfe,0x0be2,0x0bc6,0x0baa,0x0b8e,0x0b71,0x0b55,0x0b39,
	0x0b1d,0x0b01,0x0ae5,0x0ac9,0x0aac,0x0a90,0x0a74,0x0a58,0x0a3b,0x0a1f,
	0x0a03,0x09e7,0x09cb,0x09ae,0x0992,0x0976,0x0959,0x093d,0x0921,0x0905,
	0x08e8,0x08cc,0x08b0,0x0893,0x0877,0x085b,0x083e,0x0822,0x0805,0x07e9,
	0x07cd,0x07b0,0x0794,0x0778,0x075b,0x073f,0x0722,0x0706,0x06e9,0x06cd,
	0x06b1,0x0694,0x0678,0x065b,0x063f,0x0622,0x0606,0x05e9,0x05cd,0x05b0,
	0x0594,0x0577,0x055b,0x053e,0x0522,0x0505,0x04e9,0x04cc,0x04b0,0x0493,
	0x0477,0x045a,0x043e,0x0421,0x0405,0x03e8,0x03cc,0x03af,0x0393,0x0376,
	0x0359,0x033d,0x0320,0x0304,0x02e7,0x02cb,0x02ae,0x0292,0x0275,0x0258,
	0x023c,0x021f,0x0203,0x01e6,0x01c9,0x01ad,0x0190,0x0174,0x0157,0x013b,
	0x011e,0x0101,0x00e5,0x00c8,0x00ac,0x008f,0x0072,0x0056,0x0039,0x001d,
	0x0000
};

static CFixed const BASED_CODE Tangents[901] =
{
	0x00000000, 0x00000072, 0x000000e4, 0x00000157, 0x000001c9, 0x0000023b,
	0x000002ae, 0x00000320, 0x00000393, 0x00000405, 0x00000477, 0x000004ea,
	0x0000055c, 0x000005cf, 0x00000641, 0x000006b4, 0x00000726, 0x00000799,
	0x0000080b, 0x0000087e, 0x000008f0, 0x00000963, 0x000009d5, 0x00000a48,
	0x00000aba, 0x00000b2d, 0x00000b9f, 0x00000c12, 0x00000c85, 0x00000cf7,
	0x00000d6a, 0x00000ddd, 0x00000e50, 0x00000ec2, 0x00000f35, 0x00000fa8,
	0x0000101b, 0x0000108e, 0x00001100, 0x00001173, 0x000011e6, 0x00001259,
	0x000012cc, 0x0000133f, 0x000013b2, 0x00001425, 0x00001498, 0x0000150c,
	0x0000157f, 0x000015f2, 0x00001665, 0x000016d8, 0x0000174c, 0x000017bf,
	0x00001832, 0x000018a6, 0x00001919, 0x0000198d, 0x00001a00, 0x00001a74,
	0x00001ae8, 0x00001b5b, 0x00001bcf, 0x00001c43, 0x00001cb7, 0x00001d2a,
	0x00001d9e, 0x00001e12, 0x00001e86, 0x00001efa, 0x00001f6e, 0x00001fe2,
	0x00002057, 0x000020cb, 0x0000213f, 0x000021b3, 0x00002228, 0x0000229c,
	0x00002311, 0x00002385, 0x000023fa, 0x0000246f, 0x000024e3, 0x00002558,
	0x000025cd, 0x00002642, 0x000026b7, 0x0000272c, 0x000027a1, 0x00002816,
	0x0000288b, 0x00002901, 0x00002976, 0x000029eb, 0x00002a61, 0x00002ad6,
	0x00002b4c, 0x00002bc2, 0x00002c38, 0x00002cad, 0x00002d23, 0x00002d99,
	0x00002e0f, 0x00002e85, 0x00002efc, 0x00002f72, 0x00002fe8, 0x0000305f,
	0x000030d5, 0x0000314c, 0x000031c2, 0x00003239, 0x000032b0, 0x00003327,
	0x0000339e, 0x00003415, 0x0000348c, 0x00003503, 0x0000357b, 0x000035f2,
	0x0000366a, 0x000036e1, 0x00003759, 0x000037d1, 0x00003849, 0x000038c0,
	0x00003939, 0x000039b1, 0x00003a29, 0x00003aa1, 0x00003b1a, 0x00003b92,
	0x00003c0b, 0x00003c84, 0x00003cfc, 0x00003d75, 0x00003dee, 0x00003e67,
	0x00003ee1, 0x00003f5a, 0x00003fd3, 0x0000404d, 0x000040c7, 0x00004140,
	0x000041ba, 0x00004234, 0x000042ae, 0x00004329, 0x000043a3, 0x0000441d,
	0x00004498, 0x00004512, 0x0000458d, 0x00004608, 0x00004683, 0x000046fe,
	0x00004779, 0x000047f5, 0x00004870, 0x000048ec, 0x00004968, 0x000049e3,
	0x00004a5f, 0x00004adc, 0x00004b58, 0x00004bd4, 0x00004c51, 0x00004ccd,
	0x00004d4a, 0x00004dc7, 0x00004e44, 0x00004ec1, 0x00004f3e, 0x00004fbc,
	0x00005039, 0x000050b7, 0x00005135, 0x000051b3, 0x00005231, 0x000052af,
	0x0000532d, 0x000053ac, 0x0000542b, 0x000054a9, 0x00005528, 0x000055a8,
	0x00005627, 0x000056a6, 0x00005726, 0x000057a5, 0x00005825, 0x000058a5,
	0x00005926, 0x000059a6, 0x00005a26, 0x00005aa7, 0x00005b28, 0x00005ba9,
	0x00005c2a, 0x00005cab, 0x00005d2d, 0x00005dae, 0x00005e30, 0x00005eb2,
	0x00005f34, 0x00005fb6, 0x00006039, 0x000060bb, 0x0000613e, 0x000061c1,
	0x00006244, 0x000062c8, 0x0000634b, 0x000063cf, 0x00006453, 0x000064d7,
	0x0000655b, 0x000065df, 0x00006664, 0x000066e9, 0x0000676e, 0x000067f3,
	0x00006878, 0x000068fe, 0x00006983, 0x00006a09, 0x00006a90, 0x00006b16,
	0x00006b9c, 0x00006c23, 0x00006caa, 0x00006d31, 0x00006db8, 0x00006e40,
	0x00006ec7, 0x00006f4f, 0x00006fd7, 0x00007060, 0x000070e8, 0x00007171,
	0x000071fa, 0x00007283, 0x0000730d, 0x00007396, 0x00007420, 0x000074aa,
	0x00007534, 0x000075bf, 0x00007649, 0x000076d4, 0x0000775f, 0x000077eb,
	0x00007876, 0x00007902, 0x0000798e, 0x00007a1b, 0x00007aa7, 0x00007b34,
	0x00007bc1, 0x00007c4e, 0x00007cdc, 0x00007d69, 0x00007df7, 0x00007e85,
	0x00007f14, 0x00007fa3, 0x00008031, 0x000080c1, 0x00008150, 0x000081e0,
	0x00008270, 0x00008300, 0x00008390, 0x00008421, 0x000084b2, 0x00008543,
	0x000085d5, 0x00008667, 0x000086f9, 0x0000878b, 0x0000881e, 0x000088b0,
	0x00008944, 0x000089d7, 0x00008a6b, 0x00008aff, 0x00008b93, 0x00008c27,
	0x00008cbc, 0x00008d51, 0x00008de7, 0x00008e7c, 0x00008f12, 0x00008fa9,
	0x0000903f, 0x000090d6, 0x0000916d, 0x00009205, 0x0000929c, 0x00009334,
	0x000093cd, 0x00009465, 0x000094fe, 0x00009598, 0x00009631, 0x000096cb,
	0x00009765, 0x00009800, 0x0000989b, 0x00009936, 0x000099d2, 0x00009a6d,
	0x00009b0a, 0x00009ba6, 0x00009c43, 0x00009ce0, 0x00009d7e, 0x00009e1b,
	0x00009eba, 0x00009f58, 0x00009ff7, 0x0000a096, 0x0000a136, 0x0000a1d6,
	0x0000a276, 0x0000a317, 0x0000a3b8, 0x0000a459, 0x0000a4fb, 0x0000a59d,
	0x0000a63f, 0x0000a6e2, 0x0000a785, 0x0000a829, 0x0000a8cd, 0x0000a971,
	0x0000aa16, 0x0000aabb, 0x0000ab60, 0x0000ac06, 0x0000acac, 0x0000ad53,
	0x0000adfa, 0x0000aea1, 0x0000af49, 0x0000aff1, 0x0000b09a, 0x0000b143,
	0x0000b1ec, 0x0000b296, 0x0000b340, 0x0000b3eb, 0x0000b496, 0x0000b542,
	0x0000b5ee, 0x0000b69a, 0x0000b747, 0x0000b7f4, 0x0000b8a2, 0x0000b950,
	0x0000b9fe, 0x0000baad, 0x0000bb5d, 0x0000bc0c, 0x0000bcbd, 0x0000bd6e,
	0x0000be1f, 0x0000bed1, 0x0000bf83, 0x0000c035, 0x0000c0e8, 0x0000c19c,
	0x0000c250, 0x0000c305, 0x0000c3ba, 0x0000c46f, 0x0000c525, 0x0000c5db,
	0x0000c692, 0x0000c74a, 0x0000c802, 0x0000c8ba, 0x0000c973, 0x0000ca2d,
	0x0000cae7, 0x0000cba1, 0x0000cc5c, 0x0000cd18, 0x0000cdd4, 0x0000ce90,
	0x0000cf4e, 0x0000d00b, 0x0000d0c9, 0x0000d188, 0x0000d247, 0x0000d307,
	0x0000d3c8, 0x0000d489, 0x0000d54a, 0x0000d60c, 0x0000d6cf, 0x0000d792,
	0x0000d856, 0x0000d91a, 0x0000d9df, 0x0000daa5, 0x0000db6b, 0x0000dc31,
	0x0000dcf9, 0x0000ddc1, 0x0000de89, 0x0000df52, 0x0000e01c, 0x0000e0e6,
	0x0000e1b1, 0x0000e27d, 0x0000e349, 0x0000e416, 0x0000e4e3, 0x0000e5b2,
	0x0000e680, 0x0000e750, 0x0000e820, 0x0000e8f1, 0x0000e9c2, 0x0000ea94,
	0x0000eb67, 0x0000ec3a, 0x0000ed0f, 0x0000ede3, 0x0000eeb9, 0x0000ef8f,
	0x0000f066, 0x0000f13e, 0x0000f216, 0x0000f2ef, 0x0000f3c9, 0x0000f4a3,
	0x0000f57e, 0x0000f65a, 0x0000f737, 0x0000f814, 0x0000f8f2, 0x0000f9d1,
	0x0000fab1, 0x0000fb92, 0x0000fc73, 0x0000fd55, 0x0000fe38, 0x0000ff1b,
	0x0000ffff, 0x000100e5, 0x000101cb, 0x000102b1, 0x00010399, 0x00010481,
	0x0001056b, 0x00010655, 0x00010740, 0x0001082b, 0x00010918, 0x00010a05,
	0x00010af4, 0x00010be3, 0x00010cd3, 0x00010dc4, 0x00010eb6, 0x00010fa9,
	0x0001109c, 0x00011191, 0x00011286, 0x0001137d, 0x00011474, 0x0001156c,
	0x00011665, 0x0001175f, 0x0001185b, 0x00011957, 0x00011a54, 0x00011b52,
	0x00011c51, 0x00011d51, 0x00011e52, 0x00011f53, 0x00012056, 0x0001215a,
	0x00012260, 0x00012366, 0x0001246d, 0x00012575, 0x0001267e, 0x00012788,
	0x00012894, 0x000129a0, 0x00012aae, 0x00012bbc, 0x00012ccc, 0x00012ddd,
	0x00012eef, 0x00013002, 0x00013116, 0x0001322c, 0x00013342, 0x0001345a,
	0x00013573, 0x0001368d, 0x000137a8, 0x000138c5, 0x000139e3, 0x00013b02,
	0x00013c22, 0x00013d43, 0x00013e66, 0x00013f8a, 0x000140af, 0x000141d6,
	0x000142fd, 0x00014426, 0x00014551, 0x0001467d, 0x000147aa, 0x000148d8,
	0x00014a08, 0x00014b39, 0x00014c6c, 0x00014da0, 0x00014ed5, 0x0001500c,
	0x00015144, 0x0001527e, 0x000153b9, 0x000154f5, 0x00015633, 0x00015773,
	0x000158b4, 0x000159f6, 0x00015b3a, 0x00015c80, 0x00015dc7, 0x00015f10,
	0x0001605a, 0x000161a6, 0x000162f3, 0x00016442, 0x00016593, 0x000166e6,
	0x0001683a, 0x0001698f, 0x00016ae7, 0x00016c40, 0x00016d9b, 0x00016ef7,
	0x00017055, 0x000171b6, 0x00017317, 0x0001747b, 0x000175e0, 0x00017748,
	0x000178b1, 0x00017a1c, 0x00017b89, 0x00017cf7, 0x00017e68, 0x00017fdb,
	0x0001814f, 0x000182c6, 0x0001843e, 0x000185b9, 0x00018735, 0x000188b4,
	0x00018a34, 0x00018bb7, 0x00018d3b, 0x00018ec2, 0x0001904b, 0x000191d6,
	0x00019364, 0x000194f3, 0x00019685, 0x00019819, 0x000199af, 0x00019b47,
	0x00019ce2, 0x00019e7f, 0x0001a01f, 0x0001a1c1, 0x0001a365, 0x0001a50b,
	0x0001a6b4, 0x0001a860, 0x0001aa0e, 0x0001abbe, 0x0001ad71, 0x0001af27,
	0x0001b0df, 0x0001b29a, 0x0001b457, 0x0001b617, 0x0001b7da, 0x0001b99f,
	0x0001bb67, 0x0001bd32, 0x0001bf00, 0x0001c0d0, 0x0001c2a4, 0x0001c47a,
	0x0001c653, 0x0001c82f, 0x0001ca0e, 0x0001cbf0, 0x0001cdd6, 0x0001cfbe,
	0x0001d1a9, 0x0001d397, 0x0001d589, 0x0001d77e, 0x0001d976, 0x0001db71,
	0x0001dd70, 0x0001df72, 0x0001e177, 0x0001e37f, 0x0001e58c, 0x0001e79b,
	0x0001e9ae, 0x0001ebc5, 0x0001eddf, 0x0001effd, 0x0001f21f, 0x0001f444,
	0x0001f66d, 0x0001f89a, 0x0001facb, 0x0001fcff, 0x0001ff38, 0x00020174,
	0x000203b5, 0x000205fa, 0x00020842, 0x00020a8f, 0x00020ce0, 0x00020f36,
	0x0002118f, 0x000213ed, 0x00021650, 0x000218b7, 0x00021b22, 0x00021d92,
	0x00022007, 0x00022280, 0x000224fe, 0x00022781, 0x00022a08, 0x00022c95,
	0x00022f27, 0x000231bd, 0x00023459, 0x000236fa, 0x000239a0, 0x00023c4b,
	0x00023efc, 0x000241b2, 0x0002446e, 0x0002472f, 0x000249f5, 0x00024cc2,
	0x00024f94, 0x0002526c, 0x0002554a, 0x0002582f, 0x00025b19, 0x00025e09,
	0x000260ff, 0x000263fc, 0x00026700, 0x00026a09, 0x00026d1a, 0x00027031,
	0x0002734f, 0x00027673, 0x0002799f, 0x00027cd1, 0x0002800b, 0x0002834c,
	0x00028695, 0x000289e4, 0x00028d3c, 0x0002909b, 0x00029401, 0x00029770,
	0x00029ae7, 0x00029e65, 0x0002a1ec, 0x0002a57b, 0x0002a913, 0x0002acb3,
	0x0002b05c, 0x0002b40e, 0x0002b7c9, 0x0002bb8d, 0x0002bf5a, 0x0002c331,
	0x0002c711, 0x0002cafa, 0x0002ceee, 0x0002d2eb, 0x0002d6f3, 0x0002db05,
	0x0002df21, 0x0002e348, 0x0002e77a, 0x0002ebb6, 0x0002effe, 0x0002f451,
	0x0002f8b0, 0x0002fd1a, 0x00030190, 0x00030612, 0x00030aa0, 0x00030f3b,
	0x000313e3, 0x00031897, 0x00031d58, 0x00032227, 0x00032703, 0x00032bed,
	0x000330e5, 0x000335eb, 0x00033b00, 0x00034024, 0x00034556, 0x00034a98,
	0x00034fe9, 0x0003554a, 0x00035abc, 0x0003603d, 0x000365d0, 0x00036b73,
	0x00037128, 0x000376ee, 0x00037cc7, 0x000382b1, 0x000388af, 0x00038ebf,
	0x000394e3, 0x00039b1b, 0x0003a166, 0x0003a7c7, 0x0003ae3c, 0x0003b4c7,
	0x0003bb67, 0x0003c21e, 0x0003c8eb, 0x0003cfd0, 0x0003d6cc, 0x0003dde0,
	0x0003e50d, 0x0003ec53, 0x0003f3b3, 0x0003fb2d, 0x000402c2, 0x00040a72,
	0x0004123e, 0x00041a27, 0x0004222d, 0x00042a51, 0x00043293, 0x00043af4,
	0x00044376, 0x00044c18, 0x000454db, 0x00045dc1, 0x000466c9, 0x00046ff6,
	0x00047947, 0x000482bd, 0x00048c5a, 0x0004961f, 0x0004a00c, 0x0004aa22,
	0x0004b462, 0x0004bece, 0x0004c967, 0x0004d42d, 0x0004df22, 0x0004ea47,
	0x0004f59e, 0x00050127, 0x00050ce4, 0x000518d7, 0x00052501, 0x00053163,
	0x00053dff, 0x00054ad7, 0x000557ec, 0x00056540, 0x000572d5, 0x000580ad,
	0x00058ec9, 0x00059d2d, 0x0005abd9, 0x0005bad0, 0x0005ca14, 0x0005d9a9,
	0x0005e990, 0x0005f9cb, 0x00060a5e, 0x00061b4c, 0x00062c97, 0x00063e42,
	0x00065052, 0x000662c8, 0x000675a8, 0x000688f7, 0x00069cb8, 0x0006b0ef,
	0x0006c5a0, 0x0006dad0, 0x0006f084, 0x000706bf, 0x00071d88, 0x000734e4,
	0x00074cd8, 0x0007656a, 0x00077ea1, 0x00079883, 0x0007b317, 0x0007ce64,
	0x0007ea72, 0x0008074a, 0x000824f3, 0x00084378, 0x000862e1, 0x00088339,
	0x0008a48b, 0x0008c6e2, 0x0008ea4a, 0x00090ed1, 0x00093485, 0x00095b73,
	0x000983ad, 0x0009ad42, 0x0009d845, 0x000a04ca, 0x000a32e3, 0x000a62a9,
	0x000a9432, 0x000ac798, 0x000afcf7, 0x000b346c, 0x000b6e17, 0x000baa1c,
	0x000be89f, 0x000c29c9, 0x000c6dc7, 0x000cb4c9, 0x000cff04, 0x000d4cb0,
	0x000d9e0e, 0x000df362, 0x000e4cf8, 0x000eab24, 0x000f0e43, 0x000f76bc,
	0x000fe500, 0x00105990, 0x0010d4f8, 0x001157d8, 0x0011e2e5, 0x001276e7,
	0x001314c5, 0x0013bd83, 0x0014724c, 0x00153477, 0x0016058e, 0x0016e75d,
	0x0017dbf9, 0x0018e5d7, 0x001a07de, 0x001b4580, 0x001ca2e1, 0x001e2505,
	0x001fd20d, 0x0021b189, 0x0023ccf1, 0x0026303e, 0x0028eadb, 0x002c10ec,
	0x002fbd4f, 0x003414a6, 0x00394a3a, 0x003fa820, 0x00479d75, 0x0051d8d7,
	0x005f7d4e, 0x007296b1, 0x008f3cb3, 0x00befbf3, 0x011e7a4c, 0x023cf50a,
	0x7fffffff
};

static inline CFixed TANGENT(int n)
{
//	ASSERT(n >= 0 && n < 900);
	return Tangents[(n)];
}

static inline CFixed COSINE(int n)
{
	return (MakeFixed(0, Cosines[(n)])<<2);
}

/*
// Return the sine of the passed angle.
*/

CFixed Sine(CFixed lDegrees)
{
	while (lDegrees < 0)
	{
		lDegrees += D360;
	}
	while (lDegrees >= D360)
	{
		lDegrees -= D360;
	}

	ASSERT(lDegrees >= 0 && lDegrees < D360);
	
	if (lDegrees > D270)
	{
		return -Cosine(lDegrees - D270);
	}
	else if (lDegrees > D180)
	{
		return -Cosine(D270 - lDegrees);
	}
	else if (lDegrees > D90)
	{
		return Cosine(lDegrees - D90);
	}
	else
	{
		return Cosine(D90 - lDegrees);
	}
}

/*
// Return the cosine of the passed angle.
*/

CFixed Cosine(CFixed lDegrees)
{
	while (lDegrees < 0)
	{
		lDegrees += D360;
	}

	while (lDegrees >= D360)
	{
		lDegrees -= D360;
	}

	ASSERT(lDegrees >= 0 && lDegrees < D360);

	unsigned short nDegrees = FixedInteger(lDegrees*10);

	if (nDegrees > 270*10)
	{
		return COSINE(360*10-nDegrees);
	}
	else if (nDegrees > 180*10)
	{
		return -COSINE(nDegrees-180*10);
	}
	else if (nDegrees > 90*10)
	{
		return -COSINE(180*10-nDegrees);
	}
	else
	{
		return COSINE(nDegrees);
	}
}

/*
// Return the tangent of the passed angle.
*/

CFixed Tangent(CFixed lDegrees)
{
#if 0
	double d = MakeDouble(lDegrees);
	double t = tan(d/572.958);
	return MakeFixed(t);
#else
	while (lDegrees < 0)
	{
		lDegrees += D360;
	}

	while (lDegrees >= D360)
	{
		lDegrees -= D360;
	}

	ASSERT(lDegrees >= 0 && lDegrees < D360);

	unsigned short nDegrees = FixedInteger(lDegrees*10);

	if (nDegrees > 270*10)
	{
		return -TANGENT(360*10-nDegrees);
	}
	else if (nDegrees > 180*10)
	{
		return TANGENT(nDegrees-180*10);
	}
	else if (nDegrees > 90*10)
	{
		return -TANGENT(180*10-nDegrees);
	}
	else
	{
		return TANGENT(nDegrees);
	}
#endif
}

/*
// Compute the arctangent of (dy/dx) given dy and dx. The signs of dy and
// dx are used to determine the quadrant of the angle.
*/

CFixed ArcTangent2(CFixed dy, CFixed dx)
{
	// FF - Doing this in floating point profiles as being faster
	// on a machine with a FP processor.
	double y = MakeDouble(dy);
	double x = MakeDouble(dx);
	double a = atan2(y,x);
	return MakeFixed(a*RAD2DEGC);
#if 0
/*
// Compute the quadrant and make all numbers positive.
*/

	int nQuadrant = 0;

	if (dx < 0)
	{
		nQuadrant ^= 1;
		dx = -dx;
	}
	if (dy < 0)
	{
		nQuadrant ^= 3;
		dy = -dy;
	}

/*
// Check for the 90 degree case. We don't want to do any division because
// we could end up with the dreaded "divide-by-zero" (either for dx being 0
// or simply for dy/dx being larger than will fit in 32 bits).
// So, we go at it the other way.
*/

	CFixed lTan;

	if (dx == 0
		 || (FixedInteger(dx) == 0
			  && (dy >= 0x3fff8000L
					|| ((unsigned)FixedInteger(dy+0x8000))*2 >= FixedFraction(dx)))
		 || ((lTan = DivFixed(dy, dx)) > DTan8995))
//	if (dx == 0 || long(FixedInteger(dx))*long(FixedInteger(DTan8995)) <= long(FixedInteger(dy)))
	{
		return (nQuadrant < 2) ? D90 : D270;
	}

/*
// Now it's safe to continue with our table search.
*/

	const CFixed BASED_CODE * pLastCompare;
	int nLastCompare;

	int nTangents = sizeof(Tangents)/sizeof(CFixed)-1;
	int nLeft = 0, nRight = nTangents-1;
	int nAngle;
	CFixed lLastCompare;
	do
	{
		nAngle = (nLeft + nRight) >> 1;
		pLastCompare = Tangents + nAngle;
		lLastCompare = *pLastCompare;
		if (lLastCompare == lTan)
		{
			nLastCompare = 0;
			break;
		}
		else if (lTan > lLastCompare)
		{
			nLastCompare = 1;
			nLeft = nAngle+1;
		}
		else
		{
			nLastCompare = -1;
			nRight = nAngle-1;
		}
	} while (nLeft <= nRight);

/* Compute the base angle. */

	CFixed lATan = MakeFixed(nAngle)/10;

	if (nLastCompare < 0)
	{
	/* Value is less than last element compared. */
		ASSERT(nAngle != 0);
		lATan -= MulDivFixed(DOneTenth,
									lLastCompare - lTan,
									lLastCompare - *(pLastCompare-1)); 
	}
	else
	{
	/* Value is greater than last element compared. */
		if (nAngle != sizeof(Tangents)/sizeof(CFixed)-2)
		{
			lATan += MulDivFixed(DOneTenth,
										lTan - lLastCompare,
										*(pLastCompare+1) - lLastCompare); 
		}
	}

	switch (nQuadrant)
	{
		case 1:
		{
			lATan = D180 - lATan;
			break;
		}
		case 2:
		{
			lATan = D180 + lATan;
			break;
		}
		case 3:
		{
			lATan = D360 - lATan;
			break;
		}
		default:
		{
			break;
		}
	}
	return lATan;
#endif
}

#ifdef WIN32

extern "C"
{

// The asm code below was converted to C code by Roger House 96/02/25

long pascal MulFixed(long A, long B)
{
#ifdef _NEWPORTABLE

   LONGLONG X;

#ifdef _MAC
   // Mac compiler doesn't support 64-bit integers at this time
   // LONGLONG is a double, and you cannot shift it
   X  = Int32x32To64(A, B) / (65536L); // (>>16) same as  (/2^16)
#else // _MAC
   X = Int64ShraMod32(Int32x32To64(A, B), 16);
#endif // !_MAC

   return (LONG(X));

#else // _NEWPORTABLE

   long lResult;
   _asm
   {
          mov     eax,A
          imul    B
          shrd    eax,edx,16              // Result to eax
          mov     lResult,eax
   }
   return lResult;

#endif  // _NEWPORTABLE
} // end MulFixed


long pascal DivFixed(long A, long B)
{
#ifdef _NEWPORTABLE

   LONGLONG X;

#ifdef _MAC
   // Mac compiler doesn't support 64-bit integers at this time
   // LONGLONG is a double, and you cannot shift it
   X = ((LONGLONG)A * (LONGLONG)(65536)) / (LONGLONG)B; // (<<16) same as (* 2^16)
#else // _MAC
   X = ((LONGLONG)Int64ShllMod32(A, 16)) / B;
#endif // !_MAC

   return (LONG(X));

#else // _NEWPORTABLE

   long lResult;
   _asm
   {
          mov     edx,A
          sub     eax,eax
          shrd    eax,edx,16
          sar     edx,16
          idiv    B
          mov     lResult,eax
   }

   return lResult;

#endif  // _NEWPORTABLE

} // end DivFixed


long pascal ModFixed(long A, long B)
{
#ifdef _NEWPORTABLE

   return(A % B);

#else

   long lResult;
   _asm
   {
       mov     eax,A
       cdq
       idiv    B
       mov     lResult,edx                     ; Get remainder!
   }
   return lResult;

#endif  // _NEWPORTABLE

} // end ModFixed


long pascal MulDivFixed(long A, long B, long C)
{
	if (!C) //j
		return 0; //j

#ifdef _NEWPORTABLE

   return(MulDiv(A, B, C));

#else

   long lResult;
   _asm
   {
      mov     eax,A
      imul    B
      idiv    C
      mov     lResult,eax
   }
   return lResult;

#endif  // _NEWPORTABLE

} // end MulDivFixed


// Compute (2*A+B)/3.
long pascal OneThirdFixed(long A, long B)
{
#ifdef _NEWPORTABLE

   LONGLONG X;

   X = 2*LONGLONG(A) + LONGLONG(B);

   return(LONG(X / 3L));

#else // _NEWPORTABLE

   long lResult;
   _asm
   {
      mov     eax, B
      cdq
      mov     ecx, edx        ; Save high part of Number2 in ecx

      mov     eax, A
      cdq

      add     eax, eax        ; Number1 * 2
      adc     edx, edx        ;
      add     eax, B          ; + Number2
      adc     edx, ecx

      mov     ebx, 3
      idiv    ebx
      mov     lResult, eax
   }

   return lResult;

#endif  // _NEWPORTABLE

} // end OneThirdFixed

long pascal MidPointFixed(long A, long B)
{
#ifdef _NEWPORTABLE

   LONGLONG X;

#ifdef _MAC
   // Mac compiler doesn't support 64-bit integers at this time
   // LONGLONG is a double, and you cannot shift it
   X = (LONGLONG(A) + LONGLONG(B)) / 2; // (>>1) same as (/ 2^1)
#else // _MAC
   X = Int64ShraMod32(LONGLONG(A) + LONGLONG(B), 1);
#endif // !_MAC

   return(LONG(X));

#else // _NEWPORTABLE

   long lResult;

   _asm
   {
      mov     eax, A
      cdq
      mov     ecx, edx

      mov     eax, B
      cdq

      add     eax, A
      adc     edx, ecx

      shrd    eax,edx,1                       // Result to eax
      mov     lResult, eax
   }

   return lResult;

#endif  // _NEWPORTABLE

} // end MidPointFixed

long pascal MulDivLong(long A, long B, long C)
{
#ifdef _NEWPORTABLE

   return(MulDiv(A, B, C));

#else

   long lResult;
   _asm
   {
      mov     eax,A
      imul    B
      idiv    C
      mov     lResult, eax
   }
   return lResult;

#endif  // _NEWPORTABLE

} // end MulDivLong


long pascal SafeMulDivLong(long A, long B, long C)
{
#ifdef _NEWPORTABLE

   return(MulDiv(A, B, C));

#else

   long lResult;
	_asm
	{
		xor	ecx,ecx

		mov	eax,dword ptr C
		cdq
		xor	eax,edx
		sub	eax,edx
		push	eax
		xor	ecx,edx

		mov	eax,dword ptr B
		cdq
		xor	eax,edx
		sub	eax,edx
		mov	ebx,eax
		xor	ecx,edx

		mov	eax,dword ptr A
		cdq
		xor	eax,edx
		sub	eax,edx
		xor	ecx,edx

		mul	ebx

		pop	ebx

		cmp	ebx,edx
		ja		no_overflow

		mov	eax,0x7fffffff

		jmp	adjust_sign

no_overflow:

		div	ebx

adjust_sign:

		xor	eax,ecx
		sub	eax,ecx

		mov	lResult,eax
	}
   return lResult;

#endif  // _NEWPORTABLE

} // end SafeMulDivLong


}       // extern "C"

#endif  // WIN32
// end cfixed.cpp
