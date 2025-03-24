#include "customdelegete.h"

customDelegete::customDelegete()
{

}

void customDelegete::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QVariant ret = index.model()->data(index);//index.model()获取表格的数据模型，在通过该模型获取索引为index的数据
//    qDebug()<<ret<<endl;
    QImage image;
    if(ret == "未收藏")
    {
        image.load(":/love.png");
    }
    else if(ret == "已收藏")
    {
        image.load(":/loved.png");
    }

    if (!image.isNull()) {
        // 调整图片大小到32x32
        image = image.scaled(32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        // 获取绘制选项中的矩形
        QRect rect = option.rect;//option.rect是表格的矩形，包含了格子在整个表格的位置和矩形的宽高
        rect.setSize(QSize(32, 32));

        rect.setWidth(96);
        rect.setLeft(776);
        // 在调整后的矩形中绘制图片
        painter->drawImage(rect, image); //712  178/32 = 5
    }


//例子
//    qDebug()<<option.rect<<endl;//格子的矩形
//    QRect rect = option.rect;//option.rect是表格的矩形，包含了格子在整个表格的位置和矩形的宽高
//    rect.setWidth(132);//这个75是以400为基数，也就是矩形的右边是475
//    rect.setLeft(739);//矩形的左边是425
//    painter->drawImage(rect, image);



    //修改字体
    if (index.column() == 0) { // 修改第一列的字体
        QFont font = option.font;
        font.setBold(true);
        font.setPointSize(12);
        QStyleOptionViewItem opt = option;
        initStyleOption(&opt, index);
        opt.font = font;
        QPalette palette = opt.palette;
         palette.setColor(QPalette::Text, Qt::white); // 将Text角色的颜色设置为白色
         opt.palette = palette;
        QStyledItemDelegate::paint(painter, opt, index);
    } else if (index.column() != 4) { // 对于非第五列，使用默认绘制
        QFont font = option.font;
        font.setBold(true);
        font.setPointSize(10);

        QStyleOptionViewItem opt = option;
        initStyleOption(&opt, index);
        opt.font = font;

        QPalette palette = opt.palette;
        // 创建一个QColor对象，设置RGB值为110, 110, 110
        QColor textColor(110, 110, 110);
        palette.setColor(QPalette::Text, textColor); // 将Text角色的颜色设置为自定义的灰色
        opt.palette = palette;

        QStyledItemDelegate::paint(painter, opt, index);
    }


    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);
    // 设置文本对齐方式为居中
    Qt::Alignment alignment = Qt::AlignCenter;
    opt.displayAlignment = alignment;
    // 绘制项
    QStyledItemDelegate::paint(painter, opt, index);

}
