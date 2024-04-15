/*****************************************************************//**
 * @file   GroupId.h
 * @brief  
 * 
 * @author 12910
 * @date   January 2024
 *********************************************************************/

#pragma once

#include "Contactid.h"
#include <QByteArray>
#include <cstdint>

/**
 * @brief GroupId 是群组标识符的类。
 */
class GroupId : public ContactId
{
public:
    /**
     * @brief 默认构造函数。
     */
    GroupId();

    /**
     * @brief 复制构造函数。
     * @param other 另一个 GroupId 对象。
     */
    GroupId(const GroupId& other);

    /**
     * @brief 通过原始 ID 构造 GroupId 对象。
     * @param rawId 原始 ID 的 QByteArray 表示。
     */
    explicit GroupId(const QByteArray& rawId);

    /**
     * @brief 通过原始 ID 数组构造 GroupId 对象。
     * @param rawId 原始 ID 数组。
     */
    explicit GroupId(const uint8_t* rawId);

    /**
     * @brief 获取 GroupId 对象的大小。
     * @return GroupId 对象的大小。
     */
    int getSize() const override;
};

