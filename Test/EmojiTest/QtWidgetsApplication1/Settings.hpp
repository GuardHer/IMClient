#pragma once

#include <QSettings>
#include <vector>

#include "EmojiDB.hpp"


void updateRecentEmojisFromSettings(EmojiDb& db);
void updateSettings(const EmojiDb& db);

