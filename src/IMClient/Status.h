/*****************************************************************//**
 * @file   Status.h
 * @brief  
 * 
 * @author 12910
 * @date   January 2024 10
 *********************************************************************/

#pragma once
#include <QString>
#include <QPixmap>

enum class Status
{
    Online = 0,  // ����
    Away, 	     // �뿪
    Busy, 	     // æµ
    Offline,     // ����
    Blocked      // ��ֹ
};

/**
    * @brief ��ȡ״̬ͼ��·��
    */
QString getIconPath(Status status, bool event = false); 

/**
    * @brief ��ȡ״̬ͼ��
    */
QString getTitle(Status status); 

/**
    * @brief ��ȡ״̬ͼ��
    */
QString getAssetSuffix(Status status);

/**
    * @brief ���״̬�Ƿ�����
    */
bool isOnline(Status status); 

/**
    * @brief
    */
QString statusToString(Status status);

/**
	* @brief
	*/
Status stringToStatus(const QString& status);

/**
	* @brief ��ȡ״̬ͼ��
	*/
Status intToStatus(int status);

