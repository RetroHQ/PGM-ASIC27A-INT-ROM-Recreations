#ifndef __TYPES_H__
#define __TYPES_H__

////////////////////////////////////////////////////////////////////////////////
// Types

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;
typedef unsigned long long u64;
typedef signed char s8;
typedef signed short s16;
typedef signed long s32;
typedef signed long long s64;

////////////////////////////////////////////////////////////////////////////////
// Latch access

extern u32 ReadLatch();
extern void WriteLatch(u32 v);

extern u16 gValueKey;
extern volatile u8 gRegion;

#endif
