#ifndef MYLABLE2_H
#define MYLABLE2_H
#include <QLabel>
#include <QPainter>
#include <QPainterPath>
#include <QPixmap>

class mylable2 : public QLabel {
    Q_OBJECT

public:
    explicit mylable2(QWidget *parent = nullptr) : QLabel(parent) {
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

        QPen pen;
        QColor color(Qt::red); // 初始颜色为红色
        color.setAlpha(0);   // 设置颜色的alpha值为0（透明） 这里画笔透明是为了实现头像无边框
        pen.setColor(color);

        QPainter painter(this);
        painter.setPen(pen);
        painter.setRenderHint(QPainter::Antialiasing); // 抗锯齿

        // 计算圆角矩形的尺寸（这里假设你想让圆角矩形填充整个widget）
        QRect rect(0, 0, width(), height());

        // 创建一个圆角矩形路径
        QPainterPath path;
        path.addRoundedRect(rect, 10, 10); // 第二个和第三个参数是水平和垂直圆角的大小

        // 绘制圆角矩形的背景（如果需要的话）
        painter.setBrush(QBrush(Qt::NoBrush)); // 设置背景色
        painter.drawPath(path);

        // 绘制圆角矩形内的pixmap
        // 注意：这里我们需要确保pixmap被适当地缩放或裁剪以适应圆角矩形
        // 但由于我们只是绘制，而不是裁剪，所以我们将pixmap绘制在圆角矩形内，
        // 并允许它的一部分可能超出圆角矩形的边界（如果需要的话）

        // 为了使pixmap适应圆角矩形，你可能需要缩放pixmap
        // 但这里我们保持pixmap的原始大小，并可能只绘制其一部分

        // 缩放pixmap以适应圆角矩形，同时保持宽高比
        pixmap = pixmap.scaled(rect.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        // 计算pixmap的绘制位置，使其尽可能居中于圆角矩形内
        int pixmapX = (rect.width() - pixmap.width()) / 2;
        int pixmapY = (rect.height() - pixmap.height()) / 2;

        // 绘制pixmap（它可能不会被完全裁剪到圆角矩形内，但绘制区域会被限制在圆角矩形内）
        painter.setClipPath(path); // 设置裁剪路径为圆角矩形
        painter.drawPixmap(pixmapX, pixmapY, pixmap);

        // 取消裁剪路径（如果你之后还需要在widget上绘制其他内容）
        painter.setClipping(false);
    }

private:
    QPixmap pixmap;
};
#endif // MYLABLE2_H
