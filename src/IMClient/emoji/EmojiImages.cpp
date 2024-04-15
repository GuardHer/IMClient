/*
 * Copyright (C) 2019 Philippe Proulx <eepp.ca>
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#include "EmojiImages.hpp"


EmojiImages::EmojiImages(const EmojiDb& db)
{
    this->_createPixmaps(db);
}

void EmojiImages::_createPixmaps(const EmojiDb& db)
{
    QImage image {QString::fromStdString(db.emojisPngPath())};  // 表情符号图片路径
    auto emojisPixmap = QPixmap::fromImage(std::move(image));   // 表情符号图片

    for (const auto& emojiPngLocation : db.emojiPngLocations()) {
        const auto emoji = emojiPngLocation.first;     // 表情符号 Emoji 对象
        const auto& pngLoc = emojiPngLocation.second;  // 表情符号图片位置 PngLocation 对象 两个属性 x, y
        const auto emojiSize = db.emojiSizeInt();  // 表情符号大小
        auto pixmap = std::make_unique<QPixmap>(emojisPixmap.copy(pngLoc.x, pngLoc.y, emojiSize,
                                                                  emojiSize));
        _emojiPixmaps[emoji] = std::move(pixmap);
    }
}

