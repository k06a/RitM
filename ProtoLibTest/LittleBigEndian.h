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

        // Forward declaration

        template<typename T>
        class LittleEndian;

        template<typename T>
        class BigEndian;

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

            LittleEndian(const LittleEndian<T> & t)
            {
                for (unsigned i = 0; i < sizeof(T); i++)
                    bytes[i] = t.bytes[i];
            }

            LittleEndian(const BigEndian<T> & t)
            {
                for (unsigned i = 0; i < sizeof(T); i++)
                    bytes[i] = t.bytes[sizeof(T)-1-i];
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

            const T operator += (const T t)
            {
                return (*this = *this + t);
            }

            const T operator -= (const T t)
            {
                return (*this = *this - t);
            }

            const T operator *= (const T t)
            {
                return (*this = *this * t);
            }

            const T operator /= (const T t)
            {
                return (*this = *this / t);
            }

            const T operator %= (const T t)
            {
                return (*this = *this % t);
            }

            LittleEndian<T> operator ++ (int)
            {
                for (unsigned i = 0; i < sizeof(T); i++)
                {
                    bytes[i]++;
                    if (bytes[i] != 0)
                        break;
                }
                return (*this);
            }

            LittleEndian<T> & operator ++ ()
            {
                for (unsigned i = 0; i < sizeof(T); i++)
                {
                    ++bytes[i];
                    if (bytes[i] != 0)
                        break;
                }
                return (*this);
            }

            LittleEndian<T> operator -- (int)
            {
                for (unsigned i = 0; i < sizeof(T); i++)
                {
                    bytes[i]--;
                    if (bytes[i] != (T)(-1))
                        break;
                }
                return (*this);
            }

            LittleEndian<T> & operator -- ()
            {
                for (unsigned i = 0; i < sizeof(T); i++)
                {
                    --bytes[i];
                    if (bytes[i] != (T)(-1))
                        break;
                }
                return (*this);
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

            BigEndian(const BigEndian<T> & t)
            {
                for (unsigned i = 0; i < sizeof(T); i++)
                    bytes[i] = t.bytes[i];
            }

            BigEndian(const LittleEndian<T> & t)
            {
                for (unsigned i = 0; i < sizeof(T); i++)
                    bytes[i] = t.bytes[sizeof(T)-1-i];
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

            const T operator += (const T t)
            {
                return (*this = *this + t);
            }

            const T operator -= (const T t)
            {
                return (*this = *this - t);
            }

            const T operator *= (const T t)
            {
                return (*this = *this * t);
            }

            const T operator /= (const T t)
            {
                return (*this = *this / t);
            }

            const T operator %= (const T t)
            {
                return (*this = *this % t);
            }

            LittleEndian<T> operator ++ (int)
            {
                for (unsigned i = 0; i < sizeof(T); i++)
                {
                    bytes[sizeof(T) - 1 - i]++;
                    if (bytes[sizeof(T) - 1 - i] != 0)
                        break;
                }
                return (*this);
            }

            LittleEndian<T> & operator ++ ()
            {
                for (unsigned i = 0; i < sizeof(T); i++)
                {
                    ++bytes[sizeof(T) - 1 - i];
                    if (bytes[sizeof(T) - 1 - i] != 0)
                        break;
                }
                return (*this);
            }

            LittleEndian<T> operator -- (int)
            {
                for (unsigned i = 0; i < sizeof(T); i++)
                {
                    bytes[sizeof(T) - 1 - i]--;
                    if (bytes[sizeof(T) - 1 - i] != (T)(-1))
                        break;
                }
                return (*this);
            }

            LittleEndian<T> & operator -- ()
            {
                for (unsigned i = 0; i < sizeof(T); i++)
                {
                    --bytes[sizeof(T) - 1 - i];
                    if (bytes[sizeof(T) - 1 - i] != (T)(-1))
                        break;
                }
                return (*this);
            }
		};
		#pragma pack(pop)

	}
	// namespace util
}
// namespace boolib

#endif // LITTLEBIGENDIAN_H
