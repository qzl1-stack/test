#include "fontdelegate.h"


FontDelegate::FontDelegate(QObject *parent) : QStyledItemDelegate(parent) {}

void FontDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);

    // 设置字体大小
    QFont font = opt.font;
    font.setPointSize(14); // 或者你想要的任何大小
    opt.font = font;

    // 确定文本向下移动的像素数
    int offsetY = 5; // 你可以根据需要调整这个值

    // 创建一个新的矩形用于绘制文本，基于原始项矩形但向下移动
    QRect textRect = opt.rect.adjusted(0, offsetY, 0, 0);

    // 保存painter状态（可选，但推荐）
    painter->save();

    // 设置painter的字体
    painter->setFont(font);

    // 绘制文本，注意这里我们没有使用Qt的文本对齐标志来垂直居中，因为我们已经手动设置了矩形位置
    painter->drawText(textRect, Qt::AlignLeft | Qt::AlignTop, index.model()->data(index, Qt::DisplayRole).toString());


    painter->restore();
}
