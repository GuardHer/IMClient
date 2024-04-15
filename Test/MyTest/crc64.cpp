
#include "crc64.h"

static uint64_t crc64_table[8][256] = {{0}};

#define POLY UINT64_C(0xad93d23594c935a9)

static inline uint_fast64_t crc_reflect(uint_fast64_t data, size_t data_len)
{
	uint_fast64_t ret = data & 0x01;

	for (size_t i = 1; i < data_len; i++)
	{
		data >>= 1;
		ret = (ret << 1) | (data & 0x01);
	}

	return ret;
}

uint64_t _crc64(uint_fast64_t crc, const void *in_data, const uint64_t len)
{
	const uint8_t *data = (const uint8_t *)in_data;
	unsigned long long bit;

	for (uint64_t offset = 0; offset < len; offset++)
	{
		uint8_t c = data[offset];
		for (uint_fast8_t i = 0x01; i & 0xff; i <<= 1)
		{
			bit = crc & 0x8000000000000000;
			if (c & i)
			{
				bit = ~bit;
			}

			crc <<= 1;
			if (bit)
			{
				crc ^= POLY;
			}
		}

		crc &= 0xffffffffffffffff;
	}

	crc = crc & 0xffffffffffffffff;
	return crc_reflect(crc, 64) ^ 0x0000000000000000;
}

/******************** END GENERATED PYCRC FUNCTIONS ********************/

/* Initializes the 16KB lookup tables. */
void crc64_init(void)
{
	crcspeed64native_init(_crc64, crc64_table);
}

/* Compute crc64 */
uint64_t crc64(uint64_t crc, const unsigned char *s, uint64_t l)
{
	return crcspeed64native(crc64_table, crc, (void *)s, l);
}

uint64_t crc64(uint64_t crc, const std::string& data) {
	const unsigned char* s = reinterpret_cast<const unsigned char*>(data.data());
	uint64_t l = data.size();
	return crcspeed64native(crc64_table, crc, (void*)(s), l);
}

uint64_t crc64(uint64_t crc, const QByteArray& data)
{
	const unsigned char* s = reinterpret_cast<const unsigned char*>(data.constData());
	uint64_t l = data.size();
	return crcspeed64native(crc64_table, crc, (void*)(s), l);
}

uint64_t crc64(uint64_t crc, const void* data, size_t size)
{
	const unsigned char* s = reinterpret_cast<const unsigned char*>(data);
	uint64_t l = size;
	return crcspeed64native(crc64_table, crc, (void*)(s), l);
}

uint64_t crc64(uint64_t crc, ByteArray& data)
{
	std::string msgBuff;
	msgBuff = data.retrieveAsString(data.readableBytes());
	return crc64(crc, msgBuff);
}
