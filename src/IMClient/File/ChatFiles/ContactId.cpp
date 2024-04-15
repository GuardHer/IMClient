/*****************************************************************//**
 * @file   ContactId.cpp
 * @brief  
 * @author 12910
 * @date   January 2024 10
 *********************************************************************/

#include "ContactId.h"

#include <QByteArray>
#include <QString>
#include <cstdint>
#include <QHash>

/**
 * @brief 默认构造函数。创建一个空的 id。
 */
ContactId::ContactId()
	: id()
{
}

/**
 * @brief 从字节构造一个联系人 ID。
 * @param 原始 ID 用于构造联系人 ID 的字节。
 */
ContactId::ContactId(const QByteArray& rawId)
{
	id = QByteArray(rawId);
}

/**
 * @brief 比较联系人 ID 的相等性。
 * @param 其他要比较的联系人 ID。
 * @return 如果两个联系人 ID 相等，则为 true，否则为 false。
 */
bool ContactId::operator==(const ContactId& other) const
{
	return id == other.id;
}

/**
 * @brief 比较联系人 ID 的不等式。
 * @param 其他要比较的联系人 ID。
 * @return 如果两个联系人 ID 不相等，则为 true，否则为 false.
 */
bool ContactId::operator!=(const ContactId& other) const
{
	return id != other.id;
}

/**
 * @brief 比较两个联系人 ID
 * @param 其他要比较的联系人 ID。
 * @return 如果此联系人 ID 小于其他联系人 ID，则为 true，否则为 false。
 */
bool ContactId::operator<(const ContactId& other) const
{
	return id < other.id;
}

/**
 * @brief 将联系人 ID 转换为大写十六进制字符串。
 * @return 包含 id 的十六进制表示形式的 QString
 */
QString ContactId::toString() const
{
	return id.toHex().toUpper();
}

/**
 * @brief 返回指向原始 id 数据的指针。
 * @return 指向原始 id 数据的指针，即“Contact Id::get Pk Size()”
 *         字节长。如果联系人 ID 为空，则返回 nullptr。
 */
const uint8_t* ContactId::getData() const
{
	if (id.isEmpty())
	{
		return nullptr;
	}

	return reinterpret_cast<const uint8_t*>(id.constData());
}

/**
 * @brief 获取 ID 的副本
 * @return 复制的 id 字节
 */
QByteArray ContactId::getByteArray() const
{
	return QByteArray(id); // TODO: Is a copy really necessary?
}

/**
 * @brief 检查联系人 ID 是否包含 id。
 * @return 如果有 id，则为 True，否则为 False。
 */
bool ContactId::isEmpty() const
{
	return id.isEmpty();
}
