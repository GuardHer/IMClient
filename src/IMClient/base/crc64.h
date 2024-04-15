#pragma once

#include <stdint.h>
#include <string>
#include <QByteArray>
#include "crcspeed.h"
#include "ByteArray.h"

void crc64_init(void);
uint64_t crc64(uint64_t crc, const unsigned char *s, uint64_t l);
uint64_t crc64(uint64_t crc, const std::string& data);
uint64_t crc64(uint64_t crc, const QByteArray& data);
uint64_t crc64(uint64_t crc, const void* data, size_t size);
uint64_t crc64(uint64_t crc, ByteArray& data);
