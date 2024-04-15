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
 * @brief ContactId ���Ǳ�ʾ��ϵ��Ψһ��ʶ���ĳ�����ࡣ
 * �����ṩ�˹�����ϵ�˱�ʶ����ͨ�ù��ܡ�
 */
class ContactId
{
public:
	/**
	 * @brief ����������
	 */
	virtual ~ContactId() = default;

	/**
	 * @brief ���Ƹ�ֵ�������
	 * @param other Ҫ���Ƶ� ContactId��
	 * @return ����� ContactId �����á�
	 */
	ContactId& operator=(const ContactId& other) = default;

	/**
	 * @brief �ƶ���ֵ�������
	 * @param other Ҫ�ƶ��� ContactId��
	 * @return ����� ContactId �����á�
	 */
	ContactId& operator=(ContactId&& other) = default;

	/**
	 * @brief ����������
	 * @param other Ҫ�Ƚϵ� ContactId��
	 * @return ��� ContactId ��ȣ���Ϊ true������Ϊ false��
	 */
	bool operator==(const ContactId& other) const;

	/**
	 * @brief �����������
	 * @param other Ҫ�Ƚϵ� ContactId��
	 * @return ��� ContactId ����ȣ���Ϊ true������Ϊ false��
	 */
	bool operator!=(const ContactId& other) const;

	/**
	 * @brief С���������
	 * @param other Ҫ�Ƚϵ� ContactId��
	 * @return ����� ContactId С����������Ϊ true������Ϊ false��
	 */
	bool operator<(const ContactId& other) const;

	/**
	 * @brief �� ContactId ת��Ϊ�ַ�����ʾ��
	 * @return ��ʾ ContactId �� QString��
	 */
	QString toString() const;

	/**
	 * @brief ��ȡ ContactId ��ԭʼ�ֽ������ʾ��
	 * @return ��ʾ ContactId �� QByteArray��
	 */
	QByteArray getByteArray() const;

	/**
	 * @brief ��ȡ ContactId ��ԭʼ����ָ�롣
	 * @return ָ��ԭʼ���ݵ�ָ�롣
	 */
	const uint8_t* getData() const;

	/**
	 * @brief ��� ContactId �Ƿ�Ϊ�ա�
	 * @return ��� ContactId Ϊ�գ���Ϊ true������Ϊ false��
	 */
	bool isEmpty() const;

	/**
	 * @brief ��ȡ ContactId ���ֽڴ�С��
	 * @return ContactId �Ĵ�С��
	 */
	virtual int getSize() const = 0;

protected:
	/**
	 * @brief Ĭ�Ϲ��캯����
	 */
	ContactId();

	/**
	 * @brief ���캯����ʹ��ԭʼ�ֽ������ʼ�� ContactId��
	 * @param rawId ��ʾ ContactId ��ԭʼ�ֽ����顣
	 */
	explicit ContactId(const QByteArray& rawId);

	/** ��ʾ ContactId ��ԭʼ�ֽ����顣 */
	QByteArray id;
};

/**
 * @brief ContactId �Ĺ�ϣ������ʹ������� QHash ������ʹ�á�
 * @param id Ҫ��ϣ�� ContactId��
 * @return ContactId �Ĺ�ϣֵ��
 */
inline uint qHash(const ContactId& id)
{
	return qHash(id.getByteArray());
}

/** ָ���� ContactId �Ĺ���ָ�롣 */
using ContactIdPtr = std::shared_ptr<const ContactId>;
