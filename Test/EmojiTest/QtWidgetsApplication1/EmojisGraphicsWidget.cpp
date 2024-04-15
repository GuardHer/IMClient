/*
 * Copyright (C) 2019 Philippe Proulx <eepp.ca>
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QScrollArea>
#include <QScrollBar>
#include <QListWidget>
#include <QLabel>
#include <QGraphicsTextItem>
#include <QKeyEvent>
//#include <boost/algorithm/string.hpp>

#include "EmojisGraphicsWidget.hpp"


QEmojisWidget::QEmojisWidget(QWidget * const parent, const EmojiDb& emojiDb, const bool darkBg) :
    QGraphicsView {parent},
    _emojiDb {&emojiDb},
    _emojiImages {emojiDb},
    _darkBg {darkBg}
{
    _allEmojisGraphicsSceneSelectedItem = this->_createSelectedGraphicsItem();   // 创建所有表情符号图形场景选中项
    _findEmojisGraphicsSceneSelectedItem = this->_createSelectedGraphicsItem();  // 创建查找表情符号图形场景选中项
    this->_setGraphicsSceneStyle(_allEmojisGraphicsScene);   // 设置所有表情符号图形场景样式
    this->_setGraphicsSceneStyle(_findEmojisGraphicsScene);  // 设置查找表情符号图形场景样式
    this->setAlignment(Qt::AlignLeft | Qt::AlignTop);        // 设置对齐方式
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);  // 设置垂直滚动条策略
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);  // 设置水平滚动条策略

    // margins, 6 emojis, and scrollbar
    this->setMinimumWidth(8 + (_emojiDb->emojiSizeInt() + 8) * 7 + 8 + 1);

    // 设置固定大小 560x360
    this->setFixedSize(560, 360);
}

QEmojisWidget::~QEmojisWidget()
{
    /*
     * Those "selected" graphics scene items could be out of the
     * scene currently, therefore owned by this.
     */
    if (!_findEmojisGraphicsSceneSelectedItem->scene()) {
        delete _findEmojisGraphicsSceneSelectedItem;
    }

    if (!_allEmojisGraphicsSceneSelectedItem->scene()) {
        delete _allEmojisGraphicsSceneSelectedItem;
    }
}

/**
 * @function _setGraphicsSceneStyle
 * @brief 设置图形场景样式
 * @param gs: 图形场景
 */
void QEmojisWidget::_setGraphicsSceneStyle(QGraphicsScene& gs)
{
    QColor bgColor;

    if (_darkBg) { 
        bgColor = "#202020";  // 背景颜色 
    } else {
        bgColor = "#f8f8f8";  // 背景颜色
    }

    gs.setBackgroundBrush(bgColor);  // 设置背景画刷
}

QGraphicsPixmapItem *QEmojisWidget::_createSelectedGraphicsItem()
{
    // 选中图形项框路径
    const auto path = std::string {"C:\\Users\\12910\\Desktop\\EmojiTest\\jome\\assets"} + "/sel-" +
                      std::to_string(_emojiDb->emojiSizeInt()) + ".png";

    QImage image {QString::fromStdString(path)};
    auto graphicsItem = new QGraphicsPixmapItem {
        QPixmap::fromImage(std::move(image))
    };

    graphicsItem->hide();
    graphicsItem->setEnabled(false);  // 禁用
    graphicsItem->setZValue(1000.);   // Z 值
    return graphicsItem;
}

/**
 * @function rebuild
 * @brief 重建
 */
void QEmojisWidget::rebuild()
{
    if (_allEmojisGraphicsSceneSelectedItem->scene()) {  // 如果所有表情符号图形场景选中项在场景中
        _allEmojisGraphicsScene.removeItem(_allEmojisGraphicsSceneSelectedItem);  // 移除所有表情符号图形场景选中项
    }

    _allEmojisGraphicsScene.clear(); // 清空所有表情符号图形场景
    _allEmojisGraphicsScene.addItem(_allEmojisGraphicsSceneSelectedItem); // 添加所有表情符号图形场景选中项
    _allEmojiGraphicsItems.clear(); // 清空所有表情符号图形项
    _catVertPositions.clear(); // 清空分类垂直位置

    qreal y = 8.;

    _allEmojisGraphicsScene.setSceneRect(0., 0., static_cast<qreal>(this->width()) - 8., 0.);  // 设置所有表情符号图形场景矩形
    QFont font {"Hack, DejaVu Sans Mono, monospace", 10, QFont::Bold};
    const auto rowFirstEmojiX = this->_rowFirstEmojiX(_allEmojisGraphicsScene);  // 行第一个表情符号 X 坐标
    const QColor textColor {_darkBg ? "#f8f8f8" : "#202020"};

    for (const auto& cat : _emojiDb->cats()) {
        auto item = _allEmojisGraphicsScene.addText(QString::fromStdString(cat->name()), font);

        item->setDefaultTextColor(textColor);  // 设置默认文本颜色
        item->setPos(rowFirstEmojiX, y);  // 设置位置
        _catVertPositions[cat.get()] = y; // 设置分类垂直位置
        y += 24.; // Y 坐标
        this->_addEmojisToGraphicsScene(cat->emojis(), _allEmojiGraphicsItems, // 添加表情符号到所有表情符号图形场景
                                        _allEmojisGraphicsScene, y);
        y += 8.; // Y 坐标
    }

    y -= 8.;
    _allEmojisGraphicsScene.setSceneRect(0., 0., static_cast<qreal>(this->width()) - 8., y);
}

/**
 * @function showAllEmojis
 * @brief 显示所有表情符号
 */
void QEmojisWidget::showAllEmojis()
{
    _curEmojiGraphicsItems = _allEmojiGraphicsItems;
    this->setScene(&_allEmojisGraphicsScene);
    this->_selectEmojiGraphicsItem(0);
}

/**
 * @function showFindResults
 * @brief 显示查找结果
 * @param results: 结果
 */
void QEmojisWidget::showFindResults(const std::vector<const Emoji *>& results)
{
    if (_findEmojisGraphicsSceneSelectedItem->scene()) {
        _findEmojisGraphicsScene.removeItem(_findEmojisGraphicsSceneSelectedItem);
    }

    _findEmojisGraphicsScene.clear();
    _findEmojisGraphicsScene.addItem(_findEmojisGraphicsSceneSelectedItem);
    _curEmojiGraphicsItems.clear();

    qreal y = 0.;

    _findEmojisGraphicsScene.setSceneRect(0., 0., static_cast<qreal>(this->width()) - 8., 0.);

    if (!results.empty()) {
        y = 8.;
        this->_addEmojisToGraphicsScene(results, _curEmojiGraphicsItems,
                                        _findEmojisGraphicsScene, y);
    }

    _findEmojisGraphicsScene.setSceneRect(0., 0., static_cast<qreal>(this->width()) - 8., y);
    this->setScene(&_findEmojisGraphicsScene);

    if (results.empty()) {
        this->_selectEmojiGraphicsItem(std::nullopt);
    } else {
        this->_selectEmojiGraphicsItem(0);
    }
}

/**
 * @function _emojiGraphicsItemHoverEntered
 * @brief 表情符号图形项鼠标进入事件
 * @param item: 
 */
void QEmojisWidget::_emojiGraphicsItemHoverEntered(const QEmojiGraphicsItem& item)
{
    emit this->emojiHoverEntered(item.emoji());  // 发送表情符号鼠标进入信号
}

/**
 * @function _emojiGraphicsItemHoverLeaved
 * @brief 表情符号图形项鼠标离开事件
 * @param item: 
 */
void QEmojisWidget::_emojiGraphicsItemHoverLeaved(const QEmojiGraphicsItem& item)
{
    emit this->emojiHoverLeaved(item.emoji());   // 发送表情符号鼠标离开信号
}

/**
 * @function _emojiGraphicsItemClicked
 * @brief 表情符号图形项点击事件
 * @param item:
 */
void QEmojisWidget::_emojiGraphicsItemClicked(const QEmojiGraphicsItem& item)
{
    emit this->emojiClicked(item.emoji());  // 发送表情符号点击信号
}

/**
 * @function _selectEmojiGraphicsItem
 * @brief 选择表情符号图形项
 * @param index: 索引
 */
void QEmojisWidget::_selectEmojiGraphicsItem(const std::optional<unsigned int>& index)
{
    QGraphicsPixmapItem *selectedItem = nullptr;

    if (this->showingAllEmojis()) {  // 如果显示所有表情符号
        selectedItem = _allEmojisGraphicsSceneSelectedItem;
    } else {
        selectedItem = _findEmojisGraphicsSceneSelectedItem;
    }

    assert(selectedItem);  
    assert(selectedItem->scene()); 
    _selectedEmojiGraphicsItemIndex = index;  // 选中表情符号图形项索引

    if (!index) {  // 如果索引为空
        selectedItem->hide();
        emit this->selectionChanged(nullptr);  // 发送选择改变信号
        return;
    }

    assert(*index < _curEmojiGraphicsItems.size());

    const auto& emojiGraphicsItem = *_curEmojiGraphicsItems[*index] ;  // 表情符号图形项

    //std::cout << "emojiGraphicsItem.pos().x() = " << emojiGraphicsItem.pos().x() << std::endl;
    selectedItem->show(); // 显示
    selectedItem->setPos(emojiGraphicsItem.pos().x() - 4., 
                         emojiGraphicsItem.pos().y() - 4.);

    if (*index == 0) {
        this->verticalScrollBar()->setValue(0);
    } else {
        const auto candY = selectedItem->pos().y() + 16. -
                           static_cast<qreal>(this->height()) / 2.;
        const auto y = std::max(0., candY);

        this->verticalScrollBar()->setValue(static_cast<int>(y));
    }

    emit this->selectionChanged(&emojiGraphicsItem.emoji());
}

/**
 * @function scrollToCat
 * @brief 滚动到分类
 * @param cat: 分类
 */
void QEmojisWidget::scrollToCat(const EmojiCat& cat)
{
    if (_catVertPositions.empty()) {
        return;
    }

    const auto y = std::max(0., _catVertPositions[&cat] - 8);

    this->verticalScrollBar()->setValue(static_cast<int>(y));
}

/**
 * @function selectNext
 * @brief 选择下一个
 * @param count: 计数
 */
void QEmojisWidget::selectNext(const unsigned int count)
{
    if (!_selectedEmojiGraphicsItemIndex) {
        return;
    }

    for (auto i = 0U; i < count; i++) {
        if (*_selectedEmojiGraphicsItemIndex + 1 == _curEmojiGraphicsItems.size()) {
            return;
        }

        this->_selectEmojiGraphicsItem(*_selectedEmojiGraphicsItemIndex + 1);
    }
}

/**
 * @function selectPrevious
 * @brief 选择上一个
 * @param count: 计数
 */
void QEmojisWidget::selectPrevious(const unsigned int count)
{
    if (!_selectedEmojiGraphicsItemIndex) {
        return;
    }

    for (auto i = 0U; i < count; i++) {
        if (*_selectedEmojiGraphicsItemIndex == 0) {
            return;
        }

        this->_selectEmojiGraphicsItem(*_selectedEmojiGraphicsItemIndex - 1);
    }
}

/**
 * @function selectPreviousRow
 * @brief 选择上一行
 * @param count: 计数
 */
void QEmojisWidget::selectPreviousRow(const unsigned int count)
{
    if (!_selectedEmojiGraphicsItemIndex) {
        return;
    }

    const auto& selectedEmojiGraphicsItem = *_curEmojiGraphicsItems[*_selectedEmojiGraphicsItemIndex];
    const auto curX = selectedEmojiGraphicsItem.pos().x();
    auto index = *_selectedEmojiGraphicsItemIndex;

    for (auto i = 0U; i < count; i++) {
        for (auto eI = static_cast<int>(index) - 1; eI >= 0; --eI) {
            const auto& emojiGraphicsItem = *_curEmojiGraphicsItems[eI];

            if (emojiGraphicsItem.pos().x() == curX) {
                index = static_cast<unsigned int>(eI);
                break;
            }
        }
    }

    this->_selectEmojiGraphicsItem(index);
}

/**
 * @function selectNextRow
 * @brief 选择下一行
 * @param count:
 */
void QEmojisWidget::selectNextRow(const unsigned int count)
{
    if (!_selectedEmojiGraphicsItemIndex) {
        return;
    }

    const auto& selectedEmojiGraphicsItem = *_curEmojiGraphicsItems[*_selectedEmojiGraphicsItemIndex];
    const auto curX = selectedEmojiGraphicsItem.pos().x();
    auto index = *_selectedEmojiGraphicsItemIndex;

    for (auto i = 0U; i < count; i++) {
        for (auto eI = index + 1; eI < _curEmojiGraphicsItems.size(); ++eI) {
            const auto& emojiGraphicsItem = *_curEmojiGraphicsItems[eI];

            if (emojiGraphicsItem.pos().x() == curX) {
                index = eI;
                break;
            }
        }
    }

    this->_selectEmojiGraphicsItem(index);
}

/**
 * @function selectFirst
 * @brief 选择第一个
 */
void QEmojisWidget::selectFirst()
{
    if (_curEmojiGraphicsItems.empty()) {
        return;
    }

    this->_selectEmojiGraphicsItem(0);
}

/**
 * @function selectLast
 * @brief 选择最后一个
 */
void QEmojisWidget::selectLast()
{
    if (_curEmojiGraphicsItems.empty()) {
        return;
    }

    this->_selectEmojiGraphicsItem(_curEmojiGraphicsItems.size() - 1);
}

/**
 * @function showingAllEmojis
 * @brief 是否显示所有表情符号
 * @return :
 */
bool QEmojisWidget::showingAllEmojis()
{
    return this->scene() == &_allEmojisGraphicsScene;
}


/**
 * @function resizeEvent
 * @brief 重置事件
 * @param event:
 */
void QEmojisWidget::resizeEvent(QResizeEvent * const event)
{
    QGraphicsView::resizeEvent(event);

    // save current index
    const auto selectedItemIndex = _selectedEmojiGraphicsItemIndex;  // 选中图形项索引

    // rebuild the "all emojis" view
    this->rebuild();  // 重建

    if (this->showingAllEmojis()) {
        this->showAllEmojis();  // 显示所有表情符号
    } else {
        std::vector<const Emoji *> results;

        for (const auto item : _curEmojiGraphicsItems) {
            results.push_back(&item->emoji());
        }

        this->showFindResults(results);
    }

    this->_selectEmojiGraphicsItem(selectedItemIndex);
}

