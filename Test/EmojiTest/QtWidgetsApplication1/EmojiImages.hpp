#pragma once

#include <memory>
#include <unordered_map>
#include <QPixmap>

#include "EmojiDB.hpp"


class EmojiImages
{
public:
    explicit EmojiImages(const EmojiDb& db);

    /**
     * @function pixmapForEmoji
     * @brief 获取表情符号的图片
     * @param emoji: 表情符号
     * @return : QPixmap
     */
    const QPixmap& pixmapForEmoji(const Emoji& emoji) const
    {
        return *_emojiPixmaps.at(&emoji);
    }

private:
    /**
     * @function _createPixmaps
     * @brief 创建图片
     * @param db: 表情符号数据库
     */
    void _createPixmaps(const EmojiDb& db);

private:
    std::unordered_map<const Emoji *, std::unique_ptr<QPixmap>> _emojiPixmaps; // 表情符号对应的图片
};

