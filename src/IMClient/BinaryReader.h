//
// Created by 12910 on 2023-12-13.
//

#pragma once

#include <iostream>
#include <vector>
#include <cstdint>
#include <cstring>
#include <QByteArray>
//#include <arpa/inet.h>
//#include <unistd.h>

class BinaryReader
{
public:
    explicit BinaryReader(const std::vector<uint8_t>& buffer, bool flipBytes = false)
        : _buffer(buffer), _flipBytes(flipBytes), _offset(0) {}


    // �����ֽ�˳��
    void setByteOrder(bool flipBytes)
    {
        _flipBytes = flipBytes;
    }

    // ��ȡ���ݵ�Ŀ�����
    template <typename T>
    BinaryReader& readData(T& value)
    {
        if (_offset + sizeof(T) <= _buffer.size())
        {
            std::memcpy(&value, &_buffer[_offset], sizeof(T));
            if (_flipBytes)
            {
                flipByteOrder(value);
            }
            _offset += sizeof(T);
        }
        return *this;
    }

    // ��ȡ�ַ���
    BinaryReader& readData(std::string& value)
    {
        value.clear();
        while (_offset < _buffer.size() && _buffer[_offset] != 0)
        {
            value.push_back(static_cast<char>(_buffer[_offset]));
            ++_offset;
        }
        // Move past the null terminator
        ++_offset;
        return *this;
    }

    // ��ȡָ�����ȵ��ַ���
    BinaryReader& readData(std::string& value, std::size_t length)
    {
        value.clear();
        while (_offset < _buffer.size() && length > 0)
        {
            value.push_back(static_cast<char>(_buffer[_offset]));
            ++_offset;
            --length;
        }
        return *this;
    }

    // ��ȡ��ǰƫ����
    [[nodiscard]] std::size_t getOffset() const
    {
        return _offset;
    }

private:
    // ��ת�ֽ�˳��
    template <typename T>
    void flipByteOrder(T& value)
    {
        char* bytes = reinterpret_cast<char*>(&value);
        for (std::size_t i = 0; i < sizeof(T) / 2; ++i)
        {
            std::swap(bytes[i], bytes[sizeof(T) - i - 1]);
        }
    }

    const std::vector<uint8_t>& _buffer;
    bool _flipBytes;
    std::size_t _offset;
};
