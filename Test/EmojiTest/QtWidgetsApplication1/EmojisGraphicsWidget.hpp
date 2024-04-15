#pragma once
#include <QObject>
#include <QEvent>
#include <QPixmap>
#include <QGraphicsScene>
#include <QGraphicsView>
//#include <boost/optional.hpp>
#include <optional>
#include <functional>
#include <cmath>

#include "EmojiDB.hpp"
#include "EmojiImages.hpp"
#include "EmojiGraphicsItem.hpp"


class QEmojisWidget : public QGraphicsView
{
    Q_OBJECT
    friend class QEmojiGraphicsItem;

public:
    using CatVerticalPositions = std::unordered_map<const EmojiCat *, qreal>;  // 分类垂直位置

public:
    explicit QEmojisWidget(QWidget *parent, const EmojiDb& emojiDb,
                           bool darkBg);
    ~QEmojisWidget();
    void rebuild();        // 重建
    void showAllEmojis();  // 显示所有表情符号
    void showFindResults(const std::vector<const Emoji *>& results);  // 显示查找结果
    void selectNext(unsigned int count = 1);       // 选择下一个
    void selectPrevious(unsigned int count = 1);   // 选择上一个
    void selectPreviousRow(unsigned int count = 1); // 选择上一行
    void selectNextRow(unsigned int count = 1);    // 选择下一行
    void selectFirst();  // 选择第一个
    void selectLast();  // 选择最后一个
    void scrollToCat(const EmojiCat& cat); // 滚动到分类
    bool showingAllEmojis();  // 是否显示所有表情符号

signals:
    void selectionChanged(const Emoji *emoji);  // 选择改变
    void emojiHoverEntered(const Emoji& emoji); // 表情符号悬停进入
    void emojiHoverLeaved(const Emoji& emoji);  // 表情符号悬停离开
    void emojiClicked(const Emoji& emoji);     // 表情符号点击

private:
    void resizeEvent(QResizeEvent *event) override;   // 重置事件
    void _selectEmojiGraphicsItem(const std::optional<unsigned int>& index);  // 选择表情符号图形项
    QGraphicsPixmapItem *_createSelectedGraphicsItem();  // 创建选中图形项
    void _setGraphicsSceneStyle(QGraphicsScene& gs);  // 设置图形场景样式
    void _emojiGraphicsItemHoverEntered(const QEmojiGraphicsItem& item);  // 表情符号图形项悬停进入
    void _emojiGraphicsItemHoverLeaved(const QEmojiGraphicsItem& item);   // 表情符号图形项悬停离开
    void _emojiGraphicsItemClicked(const QEmojiGraphicsItem& item);      // 表情符号图形项点击
     
private:
    /**
     * @function _rowFirstEmojiX
     * @brief 计算连续第一个表情符号的 X 位置。
     * @param gs: 图形场景
     * @return : X 位置
     */
    qreal _rowFirstEmojiX(const QGraphicsScene& gs) const
    {
        const auto availWidth = gs.width() - 8. * 2.;  // 可用宽度
        const auto rowEmojiCount = std::floor((availWidth + 8.) / (_emojiDb->emojiSizeInt() + 8.));  // 行表情符号数
        const auto emojisTotalWidth = rowEmojiCount * _emojiDb->emojiSizeInt() +
                                      (rowEmojiCount - 1) * 8.;  // 表情符号总宽度

        return std::floor((availWidth - emojisTotalWidth) / 2.) + 8.;
    }

    /**
     * @function _addEmojisToGraphicsScene
     * @brief 将表情符号添加到图形场景中。
     * @param emojis: 表情符号
     * @param emojiGraphicsItems: 表情符号图形项
     * @param gs: 图形场景
     * @param y: Y 坐标
     */
    template <typename ContainerT>
    void _addEmojisToGraphicsScene(const ContainerT& emojis,
                                   std::vector<QEmojiGraphicsItem *>& emojiGraphicsItems,
                                   QGraphicsScene& gs, qreal& y)
    {
        qreal col = 0.;
        const auto availWidth = gs.width();
        const auto rowFirstEmojiX = this->_rowFirstEmojiX(gs);
        const auto emojiWidthAndMargin = _emojiDb->emojiSizeInt() + 8.;

        for (const auto& emoji : emojis) {
            namespace ph = std::placeholders;  // NOLINT

            auto emojiGraphicsItem = new QEmojiGraphicsItem {
                *emoji, _emojiImages.pixmapForEmoji(*emoji), *this
            };

            emojiGraphicsItems.push_back(emojiGraphicsItem);  // 表情符号图形项
            emojiGraphicsItem->setPos(col * emojiWidthAndMargin + rowFirstEmojiX, y);  // 表情符号位置
            gs.addItem(emojiGraphicsItem); // 添加表情符号图形项
            col += 1; // 表情符号列数

            // 换行
            if ((col + 1.) * emojiWidthAndMargin + rowFirstEmojiX >= availWidth) {
                col = 0.; // 列数
                y += emojiWidthAndMargin; // Y 坐标
            }
        }

        if (col != 0.) {
            y += emojiWidthAndMargin; // Y 坐标
        }
    }

private:
    const EmojiDb * const _emojiDb;  // 表情符号数据库
    const EmojiImages _emojiImages;  // 表情符号图片
    QGraphicsScene _allEmojisGraphicsScene;   // 所有表情符号图形场景
    QGraphicsScene _findEmojisGraphicsScene;  // 查找表情符号图形场景
    CatVerticalPositions _catVertPositions;  // 分类垂直位置
    std::vector<QEmojiGraphicsItem *> _curEmojiGraphicsItems;  // 当前表情符号图形项
    std::vector<QEmojiGraphicsItem *> _allEmojiGraphicsItems;  // 所有表情符号图形项
    std::optional<unsigned int> _selectedEmojiGraphicsItemIndex; // 选中的表情符号图形项索引
    QGraphicsPixmapItem *_allEmojisGraphicsSceneSelectedItem = nullptr;  // 所有表情符号图形场景选中项    选中框
    QGraphicsPixmapItem *_findEmojisGraphicsSceneSelectedItem = nullptr;  // 查找表情符号图形场景选中项   选中框
    bool _darkBg;  // 暗色背景
};

