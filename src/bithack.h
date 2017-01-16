/*
 * bithack.h
 *
 *  Created on: Dec 21, 2016
 *      Author: liaoliangyi
 */

// This file is inspired by "bitsutil.h" written by Bin Fan , David G. Andersen and Michael Kaminsky 
// https://github.com/efficient/cuckoofilter/blob/master/src/bitsutil.h
// Function "upperpower2" is moved to file dynamiccuckoofilter.h

#ifndef BITHACK_H_
#define BITHACK_H_

// inspired from
// http://www-graphics.stanford.edu/~seander/bithacks.html#ZeroInWord
#define haszero4(x) (((x) - 0x1111ULL) & (~(x)) & 0x8888ULL)
#define hasvalue4(x,n) (haszero4((x) ^ (0x1111ULL * (n))))

#define haszero8(x) (((x) - 0x01010101ULL) & (~(x)) & 0x80808080ULL)
#define hasvalue8(x,n) (haszero8((x) ^ (0x01010101ULL * (n))))

#define haszero12(x) (((x) - 0x001001001001ULL) & (~(x)) & 0x800800800800ULL)
#define hasvalue12(x,n) (haszero12((x) ^ (0x001001001001ULL * (n))))

#define haszero16(x) (((x) - 0x0001000100010001ULL) & (~(x)) & 0x8000800080008000ULL)
#define hasvalue16(x,n) (haszero16((x) ^ (0x0001000100010001ULL * (n))))

#define haszero24(x) (((x) - 0x000001000001000001000001ULL) & (~(x)) & 0x800000800000800000800000ULL)
#define hasvalue24(x,n) (haszero24((x) ^ (0x000001000001000001000001ULL * (n))))

#define haszero32(x) (((x) - 0x00000001000000010000000100000001ULL) & (~(x)) & 0x80000000800000008000000080000000ULL)
#define hasvalue32(x,n) (haszero32((x) ^ (0x00000001000000010000000100000001ULL * (n))))


#endif
