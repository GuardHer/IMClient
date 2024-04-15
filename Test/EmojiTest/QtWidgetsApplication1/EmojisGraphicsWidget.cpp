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
    _allEmojisGraphicsSceneSelectedItem = this->_createSelectedGraphicsItem();   // �������б������ͼ�γ���ѡ����
    _findEmojisGraphicsSceneSelectedItem = this->_createSelectedGraphicsItem();  // �������ұ������ͼ�γ���ѡ����
    this->_setGraphicsSceneStyle(_allEmojisGraphicsScene);   // �������б������ͼ�γ�����ʽ
    this->_setGraphicsSceneStyle(_findEmojisGraphicsScene);  // ���ò��ұ������ͼ�γ�����ʽ
    this->setAlignment(Qt::AlignLeft | Qt::AlignTop);        // ���ö��뷽ʽ
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);  // ���ô�ֱ����������
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);  // ����ˮƽ����������

    // margins, 6 emojis, and scrollbar
    this->setMinimumWidth(8 + (_emojiDb->emojiSizeInt() + 8) * 7 + 8 + 1);

    // ���ù̶���С 560x360
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
 * @brief ����ͼ�γ�����ʽ
 * @param gs: ͼ�γ���
 */
void QEmojisWidget::_setGraphicsSceneStyle(QGraphicsScene& gs)
{
    QColor bgColor;

    if (_darkBg) { 
        bgColor = "#202020";  // ������ɫ 
    } else {
        bgColor = "#f8f8f8";  // ������ɫ
    }

    gs.setBackgroundBrush(bgColor);  // ���ñ�����ˢ
}

QGraphicsPixmapItem *QEmojisWidget::_createSelectedGraphicsItem()
{
    // ѡ��ͼ�����·��
    const auto path = std::string {"C:\\Users\\12910\\Desktop\\EmojiTest\\jome\\assets"} + "/sel-" +
                      std::to_string(_emojiDb->emojiSizeInt()) + ".png";

    QImage image {QString::fromStdString(path)};
    auto graphicsItem = new QGraphicsPixmapItem {
        QPixmap::fromImage(std::move(image))
    };

    graphicsItem->hide();
    graphicsItem->setEnabled(false);  // ����
    graphicsItem->setZValue(1000.);   // Z ֵ
    return graphicsItem;
}

/**
 * @function rebuild
 * @brief �ؽ�
 */
void QEmojisWidget::rebuild()
{
    if (_allEmojisGraphicsSceneSelectedItem->scene()) {  // ������б������ͼ�γ���ѡ�����ڳ�����
        _allEmojisGraphicsScene.removeItem(_allEmojisGraphicsSceneSelectedItem);  // �Ƴ����б������ͼ�γ���ѡ����
    }

    _allEmojisGraphicsScene.clear(); // ������б������ͼ�γ���
    _allEmojisGraphicsScene.addItem(_allEmojisGraphicsSceneSelectedItem); // ������б������ͼ�γ���ѡ����
    _allEmojiGraphicsItems.clear(); // ������б������ͼ����
    _catVertPositions.clear(); // ��շ��ഹֱλ��

    qreal y = 8.;

    _allEmojisGraphicsScene.setSceneRect(0., 0., static_cast<qreal>(this->width()) - 8., 0.);  // �������б������ͼ�γ�������
    QFont font {"Hack, DejaVu Sans Mono, monospace", 10, QFont::Bold};
    const auto rowFirstEmojiX = this->_rowFirstEmojiX(_allEmojisGraphicsScene);  // �е�һ��������� X ����
    const QColor textColor {_darkBg ? "#f8f8f8" : "#202020"};

    for (const auto& cat : _emojiDb->cats()) {
        auto item = _allEmojisGraphicsScene.addText(QString::fromStdString(cat->name()), font);

        item->setDefaultTextColor(textColor);  // ����Ĭ���ı���ɫ
        item->setPos(rowFirstEmojiX, y);  // ����λ��
        _catVertPositions[cat.get()] = y; // ���÷��ഹֱλ��
        y += 24.; // Y ����
        this->_addEmojisToGraphicsScene(cat->emojis(), _allEmojiGraphicsItems, // ��ӱ�����ŵ����б������ͼ�γ���
                                        _allEmojisGraphicsScene, y);
        y += 8.; // Y ����
    }

    y -= 8.;
    _allEmojisGraphicsScene.setSceneRect(0., 0., static_cast<qreal>(this->width()) - 8., y);
}

/**
 * @function showAllEmojis
 * @brief ��ʾ���б������
 */
void QEmojisWidget::showAllEmojis()
{
    _curEmojiGraphicsItems = _allEmojiGraphicsItems;
    this->setScene(&_allEmojisGraphicsScene);
    this->_selectEmojiGraphicsItem(0);
}

/**
 * @function showFindResults
 * @brief ��ʾ���ҽ��
 * @param results: ���
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
 * @brief �������ͼ�����������¼�
 * @param item: 
 */
void QEmojisWidget::_emojiGraphicsItemHoverEntered(const QEmojiGraphicsItem& item)
{
    emit this->emojiHoverEntered(item.emoji());  // ���ͱ�������������ź�
}

/**
 * @function _emojiGraphicsItemHoverLeaved
 * @brief �������ͼ��������뿪�¼�
 * @param item: 
 */
void QEmojisWidget::_emojiGraphicsItemHoverLeaved(const QEmojiGraphicsItem& item)
{
    emit this->emojiHoverLeaved(item.emoji());   // ���ͱ����������뿪�ź�
}

/**
 * @function _emojiGraphicsItemClicked
 * @brief �������ͼ�������¼�
 * @param item:
 */
void QEmojisWidget::_emojiGraphicsItemClicked(const QEmojiGraphicsItem& item)
{
    emit this->emojiClicked(item.emoji());  // ���ͱ�����ŵ���ź�
}

/**
 * @function _selectEmojiGraphicsItem
 * @brief ѡ��������ͼ����
 * @param index: ����
 */
void QEmojisWidget::_selectEmojiGraphicsItem(const std::optional<unsigned int>& index)
{
    QGraphicsPixmapItem *selectedItem = nullptr;

    if (this->showingAllEmojis()) {  // �����ʾ���б������
        selectedItem = _allEmojisGraphicsSceneSelectedItem;
    } else {
        selectedItem = _findEmojisGraphicsSceneSelectedItem;
    }

    assert(selectedItem);  
    assert(selectedItem->scene()); 
    _selectedEmojiGraphicsItemIndex = index;  // ѡ�б������ͼ��������

    if (!index) {  // �������Ϊ��
        selectedItem->hide();
        emit this->selectionChanged(nullptr);  // ����ѡ��ı��ź�
        return;
    }

    assert(*index < _curEmojiGraphicsItems.size());

    const auto& emojiGraphicsItem = *_curEmojiGraphicsItems[*index] ;  // �������ͼ����

    //std::cout << "emojiGraphicsItem.pos().x() = " << emojiGraphicsItem.pos().x() << std::endl;
    selectedItem->show(); // ��ʾ
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
 * @brief ����������
 * @param cat: ����
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
 * @brief ѡ����һ��
 * @param count: ����
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
 * @brief ѡ����һ��
 * @param count: ����
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
 * @brief ѡ����һ��
 * @param count: ����
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
 * @brief ѡ����һ��
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
 * @brief ѡ���һ��
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
 * @brief ѡ�����һ��
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
 * @brief �Ƿ���ʾ���б������
 * @return :
 */
bool QEmojisWidget::showingAllEmojis()
{
    return this->scene() == &_allEmojisGraphicsScene;
}


/**
 * @function resizeEvent
 * @brief �����¼�
 * @param event:
 */
void QEmojisWidget::resizeEvent(QResizeEvent * const event)
{
    QGraphicsView::resizeEvent(event);

    // save current index
    const auto selectedItemIndex = _selectedEmojiGraphicsItemIndex;  // ѡ��ͼ��������

    // rebuild the "all emojis" view
    this->rebuild();  // �ؽ�

    if (this->showingAllEmojis()) {
        this->showAllEmojis();  // ��ʾ���б������
    } else {
        std::vector<const Emoji *> results;

        for (const auto item : _curEmojiGraphicsItems) {
            results.push_back(&item->emoji());
        }

        this->showFindResults(results);
    }

    this->_selectEmojiGraphicsItem(selectedItemIndex);
}

