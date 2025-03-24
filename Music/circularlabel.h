#ifndef CIRCULARLABEL_H
#define CIRCULARLABEL_H

#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QPixmap>
#include <QPainter>
#include <QTimer>
#include <QDebug>
class CircularLabel : public QLabel {
    Q_OBJECT

public:
    CircularLabel(QWidget *parent = nullptr);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

    // 如果你需要在QLabel上绘制圆形图片，可以重写paintEvent
    void paintEvent(QPaintEvent *event) override;
};

// CircularLabel *circularLabel = new CircularLabel(this); // 假设这是你的主窗口或对话框的一个成员函数
// layout->addWidget(circularLabel); // 假设你已经有了一个布局管理器
#endif // CIRCULARLABEL_H
