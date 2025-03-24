#ifndef MYLABLE_H
#define MYLABLE_H

#include <QLabel>
#include <QPainter>
#include <QPainterPath>
#include <QPixmap>
//重新Qlable 自定义圆形控件 裁剪背景图片是圆形
class mylable : public QLabel {
    Q_OBJECT

public:
    explicit mylable(QWidget *parent = nullptr) : QLabel(parent) {
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
               setAlignment(Qt::AlignCenter);

           }

           void setPixmap(const QPixmap &pixmap) {
               this->pixmap = pixmap;
               // 确保在图片变化时重新绘制
               update();
           }

protected:
    void paintEvent(QPaintEvent *event) override {
        QLabel::paintEvent(event); // 调用基类的paintEvent（通常这一步是不必要的，因为我们会完全覆盖绘制逻辑）

        QPen pen;
        QColor color(Qt::red); // 初始颜色为红色
        color.setAlpha(0);   // 设置颜色的alpha值为128（半透明）
        pen.setColor(color);

        QPainter painter(this);
        painter.setPen(pen);
        painter.setRenderHint(QPainter::Antialiasing, true); // 抗锯齿

        // 计算圆形的半径（取宽高较小值的一半）
        int radius = qMin(width(), height()) / 2;

        // 创建一个圆形路径
        QPainterPath path;
        path.addEllipse(rect().center(), radius, radius);


        // 绘制圆形遮罩（可选，但在这个例子中我们不需要它，因为我们直接绘制圆形区域）
        // 如果需要背景透明，可以在这里设置

        // 绘制圆形区域内的pixmap
        // 注意：我们不会裁剪pixmap，而是直接在圆形区域内绘制它的一部分
        // 为了实现这一点，我们需要计算pixmap中应该被绘制到圆形区域内的部分
        // 但更简单的方法是使用QPainter的clipPath功能，但这需要一些额外的步骤
        // 这里我们使用QPainterPath的fillRule来绘制圆形内的pixmap部分

        // 绘制圆形背景（可选，如果你想要一个圆形背景）
        painter.setBrush(Qt::NoBrush);
        painter.drawPath(path); // 绘制圆形背景

        // 绘制圆形内的pixmap
        // 注意：这里我们不会裁剪pixmap，而是使用QPainter的clipPath来限制绘制区域
        painter.setClipPath(path); // 设置裁剪路径为圆形

        // 计算pixmap应该绘制的起始点，以确保它居中于圆形内
        int pixmapX = rect().center().x() - pixmap.width() / 2;
        int pixmapY = rect().center().y() - pixmap.height() / 2;

        // 但由于我们设置了clipPath，所以只需要考虑pixmap如何适应圆形
        // 我们可以通过缩放pixmap来确保它完全适合圆形，但这里我们保持原始大小
        // 并让pixmap的一部分可能超出圆形边界（这取决于pixmap的宽高比和圆形的尺寸）

        // 绘制pixmap（它将被裁剪到圆形区域内）
        painter.drawPixmap(pixmapX, pixmapY, pixmap);

        // 取消裁剪路径（如果你之后还需要在widget上绘制其他内容）
        painter.setClipping(false);
    }

private:
    QPixmap pixmap;
};


#endif // MYLABLE_H
