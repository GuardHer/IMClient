#pragma once

#include "base/StringPiece.h"
#include "base/Types.h"
#include "base/Endian.h"

#include <algorithm>
#include <vector>

#include <assert.h>
#include <string.h>

/// A buffer class modeled after org.jboss.netty.buffer.ChannelBuffer
///
/// @code
/// +-------------------+------------------+------------------+
/// | prependable bytes |  readable bytes  |  writable bytes  |
/// |                   |     (CONTENT)    |                  |
/// +-------------------+------------------+------------------+
/// |                   |                  |                  |
/// 0      <=      readerIndex   <=   writerIndex    <=     size
/// @endcode
/// 

class ByteArray {
public:
    static const size_t kCheapPrepend = 8;      // 预留8字节的空间
    static const size_t kInitialSize = 1024;    // 初始大小为1KB

    explicit ByteArray(size_t initialSize = kInitialSize)
        : buffer_(kCheapPrepend + initialSize),
        readerIndex_(kCheapPrepend),
        writerIndex_(kCheapPrepend)
    {
        assert(readableBytes() == 0);
        assert(writableBytes() == initialSize);
        assert(prependableBytes() == kCheapPrepend);
    }

    // implicit copy-ctor, move-ctor, dtor and assignment are fine
    // NOTE: implicit move-ctor is added in g++ 4.6

    void swap(ByteArray& rhs) {
        buffer_.swap(rhs.buffer_);
        std::swap(readerIndex_, rhs.readerIndex_);
        std::swap(writerIndex_, rhs.writerIndex_);
    }

    size_t readableBytes() const {
        return writerIndex_ - readerIndex_;
    }

    size_t writableBytes() const {
        return buffer_.size() - writerIndex_;
    }

    size_t prependableBytes() const {
        return readerIndex_;
    }

    const char* peek() const {
        return begin() + readerIndex_;
    }

    const char* findCRLF() const {
        // FIXME: replace with memmem()?
        const char* crlf = std::search(peek(), beginWrite(), kCRLF, kCRLF + 2);
        return crlf == beginWrite() ? NULL : crlf;
    }

    const char* findCRLF(const char* start) const {
        assert(peek() <= start);
        assert(start <= beginWrite());
        // FIXME: replace with memmem()?
        const char* crlf = std::search(start, beginWrite(), kCRLF, kCRLF + 2);
        return crlf == beginWrite() ? NULL : crlf;
    }

    const char* findEOL() const {
        const void* eol = memchr(peek(), '\n', readableBytes());
        return static_cast<const char*>(eol);
    }

    const char* findEOL(const char* start) const {
        assert(peek() <= start);
        assert(start <= beginWrite());
        const void* eol = memchr(start, '\n', beginWrite() - start);
        return static_cast<const char*>(eol);
    }

    // retrieve returns void, to prevent
    // string str(retrieve(readableBytes()), readableBytes());
    // the evaluation of two functions are unspecified
    void retrieve(size_t len) {
        assert(len <= readableBytes());
        if (len < readableBytes()) {
            readerIndex_ += len;
        }
        else {
            retrieveAll();
        }
    }

    void retrieveUntil(const char* end) {
        assert(peek() <= end);
        assert(end <= beginWrite());
        retrieve(end - peek());
    }

    void retrieveInt64() {
        retrieve(sizeof(int64_t));
    }
    void retrieveUint64() {
		retrieve(sizeof(uint64_t));
	}

    void retrieveInt32() {
        retrieve(sizeof(int32_t));
    }
    void retrieveUint32() {
		retrieve(sizeof(uint32_t));
	}

    void retrieveInt16() {
        retrieve(sizeof(int16_t));
    }

    void retrieveInt8() {
        retrieve(sizeof(int8_t));
    }

    void retrieveAll() {
        readerIndex_ = kCheapPrepend;
        writerIndex_ = kCheapPrepend;
    }

    string retrieveAllAsString() {
        return retrieveAsString(readableBytes());
    }

    string retrieveAsString(size_t len) {
        assert(len <= readableBytes());
        string result(peek(), len);
        retrieve(len);
        return result;
    }

    StringPiece toStringPiece() const {
        return StringPiece(peek(), static_cast<int>(readableBytes()));
    }

    void append(const StringPiece& str) {
        append(str.data(), str.size());
    }

    void append(const char* /*restrict*/ data, size_t len) {
        ensureWritableBytes(len);
        std::copy(data, data + len, beginWrite());
        hasWritten(len);
    }

    void append(const void* /*restrict*/ data, size_t len) {
        append(static_cast<const char*>(data), len);
    }

    void ensureWritableBytes(size_t len) {
        if (writableBytes() < len) {
            makeSpace(len);
        }
        assert(writableBytes() >= len);
    }

    char* beginWrite() {
        return begin() + writerIndex_;
    }

    const char* beginWrite() const {
        return begin() + writerIndex_;
    }

    void hasWritten(size_t len) {
        assert(len <= writableBytes());
        writerIndex_ += len;
    }

    void unwrite(size_t len) {
        assert(len <= readableBytes());
        writerIndex_ -= len;
    }

    ///
    /// Append int64_t using network endian
    ///
    void appendInt64(int64_t x) {
        int64_t be64 = hostToNetwork64(x);
        append(&be64, sizeof be64);
    }
    void appendUint64(uint64_t x) {
        uint64_t be64 = hostToNetwork64(x);
        append(&be64, sizeof be64);
    }

    ///
    /// Append int32_t using network endian
    ///
    void appendInt32(int32_t x) {
        int32_t be32 = hostToNetwork32(x);
        append(&be32, sizeof be32);
    }
    void appendUint32(uint32_t x) {
        uint32_t be32 = hostToNetwork32(x);
        append(&be32, sizeof be32);
    }

    void appendInt16(int16_t x) {
        int16_t be16 = hostToNetwork16(x);
        append(&be16, sizeof be16);
    }

    void appendInt8(int8_t x) {
        append(&x, sizeof x);
    }

    ///
    /// Read int64_t from network endian
    ///
    /// Require: buf->readableBytes() >= sizeof(int32_t)
    int64_t readInt64() {
        int64_t result = peekInt<int64_t>();
        retrieveInt64();
        return result;
    }
    uint64_t readUint64() {
        int64_t result = peekInt<uint64_t>();
        retrieveUint64();
        return result;
    }
    

    ///
    /// Read int32_t from network endian
    ///
    /// Require: buf->readableBytes() >= sizeof(int32_t)
    int32_t readInt32() {
        int32_t result = peekInt<int32_t>();
        retrieveInt32();
        return result;
    }
    uint32_t readUint32() {
		uint32_t result = peekInt<uint32_t>();
		retrieveUint32();
		return result;
	}

    int16_t readInt16() {
        int16_t result = peekInt<int16_t>();
        retrieveInt16();
        return result;
    }

    int8_t readInt8() {
        int8_t result = peekInt<int8_t>();
        retrieveInt8();
        return result;
    }

    template<typename T>
    T peekInt() const {
        static_assert(std::is_integral<T>::value, "T must be an integral type");  // T必须是一个整数类型: 
        assert(readableBytes() >= sizeof(T));
        T value = 0;
        ::memcpy(&value, peek(), sizeof(T));
        return networkToHost<T>(value);
    }

    ///
    /// Prepend int64_t using network endian
    ///
    void prependInt64(int64_t x) {
        int64_t be64 = hostToNetwork64(x);
        prepend(&be64, sizeof be64);
    }

    ///
    /// Prepend int32_t using network endian
    ///
    void prependInt32(int32_t x) {
        int32_t be32 = hostToNetwork32(x);
        prepend(&be32, sizeof be32);
    }

    void prependInt16(int16_t x) {
        int16_t be16 = hostToNetwork16(x);
        prepend(&be16, sizeof be16);
    }

    void prependInt8(int8_t x) {
        prepend(&x, sizeof x);
    }

    void prepend(const void* /*restrict*/ data, size_t len) {
        assert(len <= prependableBytes());
        readerIndex_ -= len;
        const char* d = static_cast<const char*>(data);
        std::copy(d, d + len, begin() + readerIndex_);
    }

    void shrink(size_t reserve) {
        // FIXME: use vector::shrink_to_fit() in C++ 11 if possible.
        ByteArray other;
        other.ensureWritableBytes(readableBytes() + reserve);
        other.append(toStringPiece());
        swap(other);
    }

    size_t internalCapacity() const {
        return buffer_.capacity();
    }


private:

    char* begin() {
        return &*buffer_.begin();
    }

    const char* begin() const {
        return &*buffer_.begin();
    }

    void makeSpace(size_t len) {
        if (writableBytes() + prependableBytes() < len + kCheapPrepend) {
            // FIXME: move readable data
            buffer_.resize(writerIndex_ + len);
        }
        else {
            // move readable data to the front, make space inside buffer
            assert(kCheapPrepend < readerIndex_);
            size_t readable = readableBytes();
            std::copy(begin() + readerIndex_,
                begin() + writerIndex_,
                begin() + kCheapPrepend);
            readerIndex_ = kCheapPrepend;
            writerIndex_ = readerIndex_ + readable;
            assert(readable == readableBytes());
        }
    }

private:
    std::vector<char> buffer_;
    size_t readerIndex_;
    size_t writerIndex_;

    static const char kCRLF[];
};