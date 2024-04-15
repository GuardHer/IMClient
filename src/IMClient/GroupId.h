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
 * @brief GroupId ��Ⱥ���ʶ�����ࡣ
 */
class GroupId : public ContactId
{
public:
    /**
     * @brief Ĭ�Ϲ��캯����
     */
    GroupId();

    /**
     * @brief ���ƹ��캯����
     * @param other ��һ�� GroupId ����
     */
    GroupId(const GroupId& other);

    /**
     * @brief ͨ��ԭʼ ID ���� GroupId ����
     * @param rawId ԭʼ ID �� QByteArray ��ʾ��
     */
    explicit GroupId(const QByteArray& rawId);

    /**
     * @brief ͨ��ԭʼ ID ���鹹�� GroupId ����
     * @param rawId ԭʼ ID ���顣
     */
    explicit GroupId(const uint8_t* rawId);

    /**
     * @brief ��ȡ GroupId ����Ĵ�С��
     * @return GroupId ����Ĵ�С��
     */
    int getSize() const override;
};

