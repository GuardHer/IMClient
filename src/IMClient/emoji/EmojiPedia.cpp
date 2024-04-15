/*
 * Copyright (C) 2021 Philippe Proulx <eepp.ca>
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#include <QDesktopServices>
#include <QString>
#include <QUrl>

#include "EmojiPedia.hpp"
#include "EmojiDB.hpp"


void gotoEmojipediaPage(const Emoji& emoji)
{
    QDesktopServices::openUrl(QUrl {
        QString {"https://emojipedia.org/"} + QString::fromStdString(emoji.str())
    });
}

