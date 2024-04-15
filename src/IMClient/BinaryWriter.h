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

    // ���� = �����
    BinaryWriter& operator=(const std::string& buffer)
    {
        _buffer.clear();
        _buffer.assign(buffer.begin(), buffer.end());  // ��string�п������ݵ�vector
        return *this;
    }

    // �����ֽ�˳��
    void setByteOrder(bool flipBytes)
    {
        _flipBytes = flipBytes;
    }

    // ����ǰ���������
    void insertData(const void* data, std::streamsize size)
    {
        if (_flipBytes)
        {
            // �����Ҫ��ת�ֽ�˳������з�ת
            const char* ptr = static_cast<const char*>(data);
            for (std::streamsize i = size - 1; i >= 0; --i)
            {
                _buffer.insert(_buffer.begin(), ptr[i]);
            }
        }
        else
        {
            // ����ֱ��д������
            _buffer.insert(_buffer.begin(), static_cast<const char*>(data), static_cast<const char*>(data) + size);
        }
    }

    // ����ǰ���������
    template <typename T>
    void insertData(const T& value)
    {
        insertData(&value, sizeof(T));
    }

    // ����ǰ������ַ���
    void insertData(const std::string& value)
    {
        insertData(value.c_str(), (long)value.size());
        //        insertData(static_cast<uint8_t>(0));             // д���ַ���������
    }

    // д�����ݵ������
    void writeData(const void* data, std::streamsize size)
    {
        if (_flipBytes)
        {
            // �����Ҫ��ת�ֽ�˳������з�ת
            const char* ptr = static_cast<const char*>(data);
            for (std::streamsize i = size - 1; i >= 0; --i)
            {
                _buffer.push_back(ptr[i]);
            }
        }
        else
        {
            // ����ֱ��д������
            _buffer.insert(_buffer.end(), static_cast<const char*>(data), static_cast<const char*>(data) + size);
        }
    }

    // д�����ݵ������
    template <typename T>
    void writeData(const T& value)
    {
        writeData(&value, sizeof(T));
    }

    // д���ַ����������
    void writeData(const std::string& value)
    {
        writeData(value.c_str(), (long)value.size());
        //        writeData(static_cast<uint8_t>(0));             // д���ַ���������
    }

    // ��ȡ����������
    [[nodiscard]] const std::vector<uint8_t>& getBuffer() const
    {
        return _buffer;
    }

    // ��ȡ���������� string
    [[nodiscard]] std::string getBufferString() const
    {
        return { _buffer.begin(), _buffer.end() };
    }


    // ��ջ�����
    void clear()
    {
        _buffer.clear();
    }

    // ��ȡ��������С
    [[nodiscard]] std::size_t getSize() const
    {
        return _buffer.size();
    }

private:
    std::vector<uint8_t> _buffer;
    bool _flipBytes;
};
