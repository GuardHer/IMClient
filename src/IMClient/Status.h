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
    Online = 0,  // 在线
    Away, 	     // 离开
    Busy, 	     // 忙碌
    Offline,     // 离线
    Blocked      // 阻止
};

/**
    * @brief 获取状态图标路径
    */
QString getIconPath(Status status, bool event = false); 

/**
    * @brief 获取状态图标
    */
QString getTitle(Status status); 

/**
    * @brief 获取状态图标
    */
QString getAssetSuffix(Status status);

/**
    * @brief 检查状态是否在线
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
	* @brief 获取状态图标
	*/
Status intToStatus(int status);

