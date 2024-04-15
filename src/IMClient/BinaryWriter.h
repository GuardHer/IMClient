//
// Created by 12910 on 2023-12-13.
//

#pragma once

#include <string>
#include <iostream>
#include <memory>
#include <cstring>

#include <iostream>
#include <vector>
#include <cstdint>
#include <cstring>
//#include <arpa/inet.h>
//#include <unistd.h>

class BinaryWriter
{
public:
    BinaryWriter() : _flipBytes(false) {}

    // 重载 = 运算符
    BinaryWriter& operator=(const std::string& buffer)
    {
        _buffer.clear();
        _buffer.assign(buffer.begin(), buffer.end());  // 从string中拷贝数据到vector
        return *this;
    }

    // 设置字节顺序
    void setByteOrder(bool flipBytes)
    {
        _flipBytes = flipBytes;
    }

    // 在最前面插入数据
    void insertData(const void* data, std::streamsize size)
    {
        if (_flipBytes)
        {
            // 如果需要翻转字节顺序，则进行翻转
            const char* ptr = static_cast<const char*>(data);
            for (std::streamsize i = size - 1; i >= 0; --i)
            {
                _buffer.insert(_buffer.begin(), ptr[i]);
            }
        }
        else
        {
            // 否则直接写入数据
            _buffer.insert(_buffer.begin(), static_cast<const char*>(data), static_cast<const char*>(data) + size);
        }
    }

    // 在最前面插入数据
    template <typename T>
    void insertData(const T& value)
    {
        insertData(&value, sizeof(T));
    }

    // 在最前面插入字符串
    void insertData(const std::string& value)
    {
        insertData(value.c_str(), (long)value.size());
        //        insertData(static_cast<uint8_t>(0));             // 写入字符串结束符
    }

    // 写入数据到输出流
    void writeData(const void* data, std::streamsize size)
    {
        if (_flipBytes)
        {
            // 如果需要翻转字节顺序，则进行翻转
            const char* ptr = static_cast<const char*>(data);
            for (std::streamsize i = size - 1; i >= 0; --i)
            {
                _buffer.push_back(ptr[i]);
            }
        }
        else
        {
            // 否则直接写入数据
            _buffer.insert(_buffer.end(), static_cast<const char*>(data), static_cast<const char*>(data) + size);
        }
    }

    // 写入数据到输出流
    template <typename T>
    void writeData(const T& value)
    {
        writeData(&value, sizeof(T));
    }

    // 写入字符串到输出流
    void writeData(const std::string& value)
    {
        writeData(value.c_str(), (long)value.size());
        //        writeData(static_cast<uint8_t>(0));             // 写入字符串结束符
    }

    // 获取打包后的数据
    [[nodiscard]] const std::vector<uint8_t>& getBuffer() const
    {
        return _buffer;
    }

    // 获取打包后的数据 string
    [[nodiscard]] std::string getBufferString() const
    {
        return { _buffer.begin(), _buffer.end() };
    }


    // 清空缓冲区
    void clear()
    {
        _buffer.clear();
    }

    // 获取缓冲区大小
    [[nodiscard]] std::size_t getSize() const
    {
        return _buffer.size();
    }

private:
    std::vector<uint8_t> _buffer;
    bool _flipBytes;
};
