#ifndef LITTLEBIGENDIAN_H
#define LITTLEBIGENDIAN_H

namespace boolib
{
	namespace util
	{

		// Determine Little-Endian or Big-Endian

		#define CURRENT_BYTE_ORDER       (*(int *)"\x01\x02\x03\x04")
		#define LITTLE_ENDIAN_BYTE_ORDER 0x04030201
		#define BIG_ENDIAN_BYTE_ORDER    0x01020304
		#define PDP_ENDIAN_BYTE_ORDER    0x02010403

		#define IS_LITTLE_ENDIAN (CURRENT_BYTE_ORDER == LITTLE_ENDIAN_BYTE_ORDER)
		#define IS_BIG_ENDIAN    (CURRENT_BYTE_ORDER == BIG_ENDIAN_BYTE_ORDER)
		#define IS_PDP_ENDIAN    (CURRENT_BYTE_ORDER == PDP_ENDIAN_BYTE_ORDER)

		// Little-Endian template

		#pragma pack(push,1)
		template<typename T>
		class LittleEndian
		{
		public:
			unsigned char bytes[sizeof(T)];

			LittleEndian(T t = T())
			{
				operator =(t);
			}

			operator const T() const
			{
				T t = T();
				//#pragma unroll
				for (unsigned i = 0; i < sizeof(T); i++)
					t |= bytes[i] << (i << 3);
				return t;
			}

			const T operator = (const T t)
			{
				//#pragma unroll
				for (unsigned i = 0; i < sizeof(T); i++)
					bytes[i] = t >> (i << 3);
				return t;
			}
		};
		#pragma pack(pop)

		// Big-Endian template

		#pragma pack(push,1)
		template<typename T>
		class BigEndian
		{
		public:
			unsigned char bytes[sizeof(T)];

			BigEndian(T t = T())
			{
				operator =(t);
			}

			operator const T() const
			{
				T t = T();
				//#pragma unroll
				for (unsigned i = 0; i < sizeof(T); i++)
					t |= bytes[sizeof(T) - 1 - i] << (i << 3);
				return t;
			}

			const T operator = (const T t)
			{
				//#pragma unroll
				for (unsigned i = 0; i < sizeof(T); i++)
					bytes[sizeof(T) - 1 - i] = t >> (i << 3);
				return t;
			}
		};
		#pragma pack(pop)

	}
	// namespace util
}
// namespace boolib

#endif // LITTLEBIGENDIAN_H
