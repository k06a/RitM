//#ifndef I8I64_H
//#define I8I64_H

#include "LittleBigEndian.h"

typedef signed __int8   i8;
typedef signed __int16 i16;
typedef signed __int32 i32;
typedef signed __int64 i64;

typedef unsigned __int8   u8;
typedef unsigned __int16 u16;
typedef unsigned __int32 u32;
typedef unsigned __int64 u64;

#ifdef BIG_ENDIAN

// Little Endian

typedef boolib::util::LittleEndian<i16> i16le;
typedef boolib::util::LittleEndian<i32> i32le;
typedef boolib::util::LittleEndian<i64> i64le;

typedef boolib::util::LittleEndian<u16> u16le;
typedef boolib::util::LittleEndian<u32> u32le;
typedef boolib::util::LittleEndian<u64> u64le;

// Big Endian

typedef i16 i16be;
typedef i32 i32be;
typedef i64 i64be;

typedef u16 u16be;
typedef u32 u32be;
typedef u64 u64be;

#else

//#if !defined(LITTLE_ENDIAN)
//#pragma message("Please define LITTLE_ENDIAN of BIG_ENDIAN before include " __FILE__)
//#endif

// Little Endian

typedef i16 i16le;
typedef i32 i32le;
typedef i64 i64le;

typedef u16 u16le;
typedef u32 u32le;
typedef u64 u64le;

// Big Endian

typedef boolib::util::BigEndian<i16> i16be;
typedef boolib::util::BigEndian<i32> i32be;
typedef boolib::util::BigEndian<i64> i64be;

typedef boolib::util::BigEndian<u16> u16be;
typedef boolib::util::BigEndian<u32> u32be;
typedef boolib::util::BigEndian<u64> u64be;

#endif

//#endif // I8I64_H