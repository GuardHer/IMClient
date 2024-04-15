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
     * @brief ��ȡ������ŵ�ͼƬ
     * @param emoji: �������
     * @return : QPixmap
     */
    const QPixmap& pixmapForEmoji(const Emoji& emoji) const
    {
        return *_emojiPixmaps.at(&emoji);
    }

private:
    /**
     * @function _createPixmaps
     * @brief ����ͼƬ
     * @param db: ����������ݿ�
     */
    void _createPixmaps(const EmojiDb& db);

private:
    std::unordered_map<const Emoji *, std::unique_ptr<QPixmap>> _emojiPixmaps; // ������Ŷ�Ӧ��ͼƬ
};

