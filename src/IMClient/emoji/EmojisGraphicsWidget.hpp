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
    using CatVerticalPositions = std::unordered_map<const EmojiCat *, qreal>;  // ���ഹֱλ��

public:
    explicit QEmojisWidget(QWidget *parent, const EmojiDb& emojiDb,
                           bool darkBg);
    ~QEmojisWidget();
    void rebuild();        // �ؽ�
    void showAllEmojis();  // ��ʾ���б������
    void showFindResults(const std::vector<const Emoji *>& results);  // ��ʾ���ҽ��
    void selectNext(unsigned int count = 1);       // ѡ����һ��
    void selectPrevious(unsigned int count = 1);   // ѡ����һ��
    void selectPreviousRow(unsigned int count = 1); // ѡ����һ��
    void selectNextRow(unsigned int count = 1);    // ѡ����һ��
    void selectFirst();  // ѡ���һ��
    void selectLast();  // ѡ�����һ��
    void scrollToCat(const EmojiCat& cat); // ����������
    bool showingAllEmojis();  // �Ƿ���ʾ���б������

signals:
    void selectionChanged(const Emoji *emoji);  // ѡ��ı�
    void emojiHoverEntered(const Emoji& emoji); // ���������ͣ����
    void emojiHoverLeaved(const Emoji& emoji);  // ���������ͣ�뿪
    void emojiClicked(const Emoji& emoji);     // ������ŵ��

private:
    void resizeEvent(QResizeEvent *event) override;   // �����¼�
    void _selectEmojiGraphicsItem(const std::optional<unsigned int>& index);  // ѡ��������ͼ����
    QGraphicsPixmapItem *_createSelectedGraphicsItem();  // ����ѡ��ͼ����
    void _setGraphicsSceneStyle(QGraphicsScene& gs);  // ����ͼ�γ�����ʽ
    void _emojiGraphicsItemHoverEntered(const QEmojiGraphicsItem& item);  // �������ͼ������ͣ����
    void _emojiGraphicsItemHoverLeaved(const QEmojiGraphicsItem& item);   // �������ͼ������ͣ�뿪
    void _emojiGraphicsItemClicked(const QEmojiGraphicsItem& item);      // �������ͼ������
     
private:
    /**
     * @function _rowFirstEmojiX
     * @brief ����������һ��������ŵ� X λ�á�
     * @param gs: ͼ�γ���
     * @return : X λ��
     */
    qreal _rowFirstEmojiX(const QGraphicsScene& gs) const
    {
        const auto availWidth = gs.width() - 8. * 2.;  // ���ÿ��
        const auto rowEmojiCount = std::floor((availWidth + 8.) / (_emojiDb->emojiSizeInt() + 8.));  // �б��������
        const auto emojisTotalWidth = rowEmojiCount * _emojiDb->emojiSizeInt() +
                                      (rowEmojiCount - 1) * 8.;  // ��������ܿ��

        return std::floor((availWidth - emojisTotalWidth) / 2.) + 8.;
    }

    /**
     * @function _addEmojisToGraphicsScene
     * @brief �����������ӵ�ͼ�γ����С�
     * @param emojis: �������
     * @param emojiGraphicsItems: �������ͼ����
     * @param gs: ͼ�γ���
     * @param y: Y ����
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

            emojiGraphicsItems.push_back(emojiGraphicsItem);  // �������ͼ����
            emojiGraphicsItem->setPos(col * emojiWidthAndMargin + rowFirstEmojiX, y);  // �������λ��
            gs.addItem(emojiGraphicsItem); // ��ӱ������ͼ����
            col += 1; // �����������

            // ����
            if ((col + 1.) * emojiWidthAndMargin + rowFirstEmojiX >= availWidth) {
                col = 0.; // ����
                y += emojiWidthAndMargin; // Y ����
            }
        }

        if (col != 0.) {
            y += emojiWidthAndMargin; // Y ����
        }
    }

private:
    const EmojiDb * const _emojiDb;  // ����������ݿ�
    const EmojiImages _emojiImages;  // �������ͼƬ
    QGraphicsScene _allEmojisGraphicsScene;   // ���б������ͼ�γ���
    QGraphicsScene _findEmojisGraphicsScene;  // ���ұ������ͼ�γ���
    CatVerticalPositions _catVertPositions;  // ���ഹֱλ��
    std::vector<QEmojiGraphicsItem *> _curEmojiGraphicsItems;  // ��ǰ�������ͼ����
    std::vector<QEmojiGraphicsItem *> _allEmojiGraphicsItems;  // ���б������ͼ����
    std::optional<unsigned int> _selectedEmojiGraphicsItemIndex; // ѡ�еı������ͼ��������
    QGraphicsPixmapItem *_allEmojisGraphicsSceneSelectedItem = nullptr;  // ���б������ͼ�γ���ѡ����    ѡ�п�
    QGraphicsPixmapItem *_findEmojisGraphicsSceneSelectedItem = nullptr;  // ���ұ������ͼ�γ���ѡ����   ѡ�п�
    bool _darkBg;  // ��ɫ����
};

