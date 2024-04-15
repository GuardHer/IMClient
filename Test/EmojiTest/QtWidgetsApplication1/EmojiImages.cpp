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
    QImage image {QString::fromStdString(db.emojisPngPath())};  // �������ͼƬ·��
    auto emojisPixmap = QPixmap::fromImage(std::move(image));   // �������ͼƬ

    for (const auto& emojiPngLocation : db.emojiPngLocations()) {
        const auto emoji = emojiPngLocation.first;     // ������� Emoji ����
        const auto& pngLoc = emojiPngLocation.second;  // �������ͼƬλ�� PngLocation ���� �������� x, y
        const auto emojiSize = db.emojiSizeInt();  // ������Ŵ�С
        auto pixmap = std::make_unique<QPixmap>(emojisPixmap.copy(pngLoc.x, pngLoc.y, emojiSize,
                                                                  emojiSize));
        _emojiPixmaps[emoji] = std::move(pixmap);
    }
}

