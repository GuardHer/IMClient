/*****************************************************************//**
 * @file   GroupId.cpp
 * @brief  
 * @author 12910
 * @date   January 2024 10
 *********************************************************************/

#include "GroupId.h"
#include <QByteArray>
#include <QString>
#include <cassert>

/**
 * @class GroupId
 * @brief 此类表示长期持久组标识符。
 */

 /**
  * @brief 默认构造函数。创建一个空的 Tox 组 ID。
  */
GroupId::GroupId()
    : ContactId()
{
}

/**
 * @brief 复制构造函数。
 * @param 要复制的其他组 ID
 */
GroupId::GroupId(const GroupId& other)
    : ContactId(other.id)
{
}

/**
 * @brief 从字节构造一个组 ID。
 * @param 原始 Id 用于构造组 Id 的字节。长度必须准确会议 UID 大小，否则组 ID 将为空。
 */
GroupId::GroupId(const QByteArray& rawId)
    : ContactId([rawId]() {
    assert(rawId.length());
    return rawId; }())
{
}

/**
 * @brief 从字节构造一个组 ID。
 * @param raw Id 用于构建组 Id 的字节
 */
GroupId::GroupId(const uint8_t* rawId)
    : ContactId(QByteArray(reinterpret_cast<const char*>(rawId), id.size()))
{
}

/**
 * @brief 获取公共 id 的大小（以字节为单位）。
 * @return 公共 ID 的大小（以字节为单位）.
 */
int GroupId::getSize() const
{
    return id.size(); 
}
