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
 * @brief �����ʾ���ڳ־����ʶ����
 */

 /**
  * @brief Ĭ�Ϲ��캯��������һ���յ� Tox �� ID��
  */
GroupId::GroupId()
    : ContactId()
{
}

/**
 * @brief ���ƹ��캯����
 * @param Ҫ���Ƶ������� ID
 */
GroupId::GroupId(const GroupId& other)
    : ContactId(other.id)
{
}

/**
 * @brief ���ֽڹ���һ���� ID��
 * @param ԭʼ Id ���ڹ����� Id ���ֽڡ����ȱ���׼ȷ���� UID ��С�������� ID ��Ϊ�ա�
 */
GroupId::GroupId(const QByteArray& rawId)
    : ContactId([rawId]() {
    assert(rawId.length());
    return rawId; }())
{
}

/**
 * @brief ���ֽڹ���һ���� ID��
 * @param raw Id ���ڹ����� Id ���ֽ�
 */
GroupId::GroupId(const uint8_t* rawId)
    : ContactId(QByteArray(reinterpret_cast<const char*>(rawId), id.size()))
{
}

/**
 * @brief ��ȡ���� id �Ĵ�С�����ֽ�Ϊ��λ����
 * @return ���� ID �Ĵ�С�����ֽ�Ϊ��λ��.
 */
int GroupId::getSize() const
{
    return id.size(); 
}
