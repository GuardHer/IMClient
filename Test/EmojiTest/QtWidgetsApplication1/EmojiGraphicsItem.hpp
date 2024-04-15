#pragma once
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <functional>

#include "EmojiDB.hpp"
#include "EmojiImages.hpp"


class QEmojisWidget;

class QEmojiGraphicsItem : public QGraphicsPixmapItem
{
public:
    using SelectEmojiFunc = std::function<void (const Emoji& emoji)>;

public:
    explicit QEmojiGraphicsItem(const Emoji& emoji, const QPixmap& pixmap,
                                QEmojisWidget& emojisWidget);

    const Emoji& emoji() const noexcept
    {
        return *_emoji;
    }

private:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

private:
    const Emoji * const _emoji;
    QEmojisWidget * const _emojisWidget;
};

