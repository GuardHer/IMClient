/*****************************************************************//**
 * @file   Status.cpp
 * @brief  
 * @author 12910
 * @date   January 2024 10
 *********************************************************************/

#include "Status.h"

#include <QString>
#include <QPixmap>
#include <QObject>

#include <cassert>

QString getTitle(Status status)
{
    switch (status) {
    case Status::Online:
        return QObject::tr("online", "contact status");
    case Status::Away:
        return QObject::tr("away", "contact status");
    case Status::Busy:
        return QObject::tr("busy", "contact status");
    case Status::Offline:
        return QObject::tr("offline", "contact status");
    case Status::Blocked:
        return QObject::tr("blocked", "contact status");
    }

    assert(false);
    return QStringLiteral("");
}

QString getAssetSuffix(Status status)
{
    switch (status) {
    case Status::Online:
        return "online";
    case Status::Away:
        return "away";
    case Status::Busy:
        return "busy";
    case Status::Offline:
        return "offline";
    case Status::Blocked:
        return "blocked";
    }
    assert(false);
    return QStringLiteral("");
}

QString getIconPath(Status status, bool event)
{
    const QString eventSuffix = event ? QStringLiteral("_notification") : QString();
    const QString statusSuffix = getAssetSuffix(status);
    if (status == Status::Blocked) {
        return ":/img/status/" + statusSuffix + ".svg";
    }
    else {
        return ":/img/status/" + statusSuffix + eventSuffix + ".svg";
    }
}

bool isOnline(Status status)
{
    return status != Status::Offline && status != Status::Blocked;
}

QString statusToString(Status status)
{
    switch (status) {
		case Status::Online:
			return QStringLiteral("online");
		case Status::Away:
			return QStringLiteral("away");
		case Status::Busy:
			return QStringLiteral("busy");
		case Status::Offline:
			return QStringLiteral("offline");
		case Status::Blocked:
			return QStringLiteral("blocked");
		}
	assert(false);
	return QStringLiteral("");
}
Status stringToStatus(const QString& status)
{
    if (status == QStringLiteral("online")) {
		return Status::Online;
	}
	else if (status == QStringLiteral("away")) {
		return Status::Away;
	}
	else if (status == QStringLiteral("busy")) {
		return Status::Busy;
	}
	else if (status == QStringLiteral("offline")) {
		return Status::Offline;
	}
	else if (status == QStringLiteral("blocked")) {
		return Status::Blocked;
	}
	else {
		assert(false);
		return Status::Offline;
	}
}
Status intToStatus(int status)
{
    switch (status) {
    case 0:
        return Status::Online;
    case 1:
        return Status::Away;
    case 2:
        return Status::Busy;
    case 3:
        return Status::Offline;
    case 4:
        return Status::Blocked;
    default:
        return Status::Offline;
    }
}
