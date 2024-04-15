/*****************************************************************//**
 * @file   ContactId.h
 * @brief  
 * 
 * @author 12910
 * @date   January 2024 10
 *********************************************************************/

#pragma once
#include <QByteArray>
#include <QString>
#include <cstdint>
#include <QHash>
#include <memory>

/**
 * @brief ContactId 类是表示联系人唯一标识符的抽象基类。
 * 该类提供了管理联系人标识符的通用功能。
 */
class ContactId
{
public:
	/**
	 * @brief 析构函数。
	 */
	virtual ~ContactId() = default;

	/**
	 * @brief 复制赋值运算符。
	 * @param other 要复制的 ContactId。
	 * @return 分配的 ContactId 的引用。
	 */
	ContactId& operator=(const ContactId& other) = default;

	/**
	 * @brief 移动赋值运算符。
	 * @param other 要移动的 ContactId。
	 * @return 分配的 ContactId 的引用。
	 */
	ContactId& operator=(ContactId&& other) = default;

	/**
	 * @brief 相等运算符。
	 * @param other 要比较的 ContactId。
	 * @return 如果 ContactId 相等，则为 true，否则为 false。
	 */
	bool operator==(const ContactId& other) const;

	/**
	 * @brief 不等运算符。
	 * @param other 要比较的 ContactId。
	 * @return 如果 ContactId 不相等，则为 true，否则为 false。
	 */
	bool operator!=(const ContactId& other) const;

	/**
	 * @brief 小于运算符。
	 * @param other 要比较的 ContactId。
	 * @return 如果此 ContactId 小于其他，则为 true，否则为 false。
	 */
	bool operator<(const ContactId& other) const;

	/**
	 * @brief 将 ContactId 转换为字符串表示。
	 * @return 表示 ContactId 的 QString。
	 */
	QString toString() const;

	/**
	 * @brief 获取 ContactId 的原始字节数组表示。
	 * @return 表示 ContactId 的 QByteArray。
	 */
	QByteArray getByteArray() const;

	/**
	 * @brief 获取 ContactId 的原始数据指针。
	 * @return 指向原始数据的指针。
	 */
	const uint8_t* getData() const;

	/**
	 * @brief 检查 ContactId 是否为空。
	 * @return 如果 ContactId 为空，则为 true，否则为 false。
	 */
	bool isEmpty() const;

	/**
	 * @brief 获取 ContactId 的字节大小。
	 * @return ContactId 的大小。
	 */
	virtual int getSize() const = 0;

protected:
	/**
	 * @brief 默认构造函数。
	 */
	ContactId();

	/**
	 * @brief 构造函数，使用原始字节数组初始化 ContactId。
	 * @param rawId 表示 ContactId 的原始字节数组。
	 */
	explicit ContactId(const QByteArray& rawId);

	/** 表示 ContactId 的原始字节数组。 */
	QByteArray id;
};

/**
 * @brief ContactId 的哈希函数，使其可以在 QHash 容器中使用。
 * @param id 要哈希的 ContactId。
 * @return ContactId 的哈希值。
 */
inline uint qHash(const ContactId& id)
{
	return qHash(id.getByteArray());
}

/** 指向常量 ContactId 的共享指针。 */
using ContactIdPtr = std::shared_ptr<const ContactId>;
