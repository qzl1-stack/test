#include "movablecentralwidget.h"



MovableCentralWidget::MovableCentralWidget(QWidget *parent) : QWidget(parent), isDragging(false), dragStartTimer(new QTimer(this)) {

    setFixedSize(400, 300); // 设置一个初始大小`
    // 设置鼠标跟踪以接收鼠标移动事件，即使鼠标按钮未按下
    setMouseTracking(true);

    // 设置定时器，用于检测长按
    dragStartTimer->setSingleShot(true);
    dragStartTimer->setInterval(500); // 假设500毫秒为长按
    connect(dragStartTimer, &QTimer::timeout, this, &MovableCentralWidget::startDragging);
}

void MovableCentralWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        // 重置定时器以检测长按
        dragStartPosition = event->globalPos();
        dragStartTimer->start();
    }
    QWidget::mousePressEvent(event);
}

void MovableCentralWidget::mouseMoveEvent(QMouseEvent *event) {
    if (isDragging) {
        // 更新位置
        move(event->globalPos() - dragStartPosition);
    }
    QWidget::mouseMoveEvent(event);
}

void MovableCentralWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        // 停止拖动
        isDragging = false;
        dragStartTimer->stop();
    }
    QWidget::mouseReleaseEvent(event);
}

void MovableCentralWidget::startDragging() {
    // 长按已确认，开始拖动
    isDragging = true;
}
