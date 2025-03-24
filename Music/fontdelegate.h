#ifndef FONTDELEGATE_H
#define FONTDELEGATE_H
#include <QApplication>
#include <QListWidget>
#include <QStyledItemDelegate>
#include <QPainter>


class FontDelegate : public QStyledItemDelegate
{
public:
    FontDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // FONTDELEGATE_H
