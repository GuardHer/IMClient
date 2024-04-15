#pragma once

#include <stdint.h>

#ifdef _WIN32
#include <QtEndian>

inline uint64_t hostToNetwork64(uint64_t host64) {
    return qToBigEndian(host64);
}

inline uint32_t hostToNetwork32(uint32_t host32) {
    return qToBigEndian(host32);
}

inline uint16_t hostToNetwork16(uint16_t host16) {
    return qToBigEndian(host16);
}

inline uint64_t networkToHost64(uint64_t net64) {
    return qFromBigEndian(net64);
}

inline uint32_t networkToHost32(uint32_t net32) {
    return qFromBigEndian(net32);
}

inline uint16_t networkToHost16(uint16_t net16) {
    return qFromBigEndian(net16);
}

template <typename T>
T networkToHost(T net) {
	return qFromBigEndian(net);
}


#elif defined(__linux__) || defined(__APPLE__) || defined(__FreeBSD__) || defined(__unix__)
#include <endian.h>

inline uint64_t hostToNetwork64(uint64_t host64) {
    return htobe64(host64);
}

inline uint32_t hostToNetwork32(uint32_t host32) {
    return htobe32(host32);
}

inline uint16_t hostToNetwork16(uint16_t host16) {
    return htobe16(host16);
}

inline uint64_t networkToHost64(uint64_t net64) {
    return be64toh(net64);
}

inline uint32_t networkToHost32(uint32_t net32) {
    return be32toh(net32);
}

inline uint16_t networkToHost16(uint16_t net16) {
    return be16toh(net16);
}
#endif
