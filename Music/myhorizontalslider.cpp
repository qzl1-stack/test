#include "myhorizontalslider.h"


MyHorizontalSlider::MyHorizontalSlider(QWidget *parent) : QSlider(Qt::Horizontal, parent) {
    // 设置正常状态下的样式表
    normalStyleSheet = "QSlider::groove:horizontal { height: 2px; background: transparent;}"
                       "QSlider::handle:horizontal { height: 2px;width: 10px; background: transparent;}"
                       "QSlider::add-page:horizontal {background: rgba(255, 255, 255, 0.1); }"
                       "QSlider::sub-page:horizontal { background: rgb(250, 250, 250);}";

    // 设置高亮状态下的样式表（例如，更改轨道和滑块的颜色）
    highlightStyleSheet = "QSlider::groove:horizontal { height: 8px; background: transparent;border-radius: 4px; }"
                          "QSlider::handle:horizontal { width: 8px; height: 8px; background: rgb(250, 250, 250);border-radius: 4px;}"
                          "QSlider::add-page:horizontal {background: rgba(255, 255, 255, 0.1);}"
                          "QSlider::sub-page:horizontal { background: rgb(200, 200, 200);}";
    // 初始时应用正常样式
    setStyleSheet(normalStyleSheet);


}

void MyHorizontalSlider::enterEvent(QEvent *event) {
    QSlider::enterEvent(event);
    // 当鼠标进入滑动条区域时执行的代码
    qDebug() << "Mouse entered the slider";
    // 鼠标进入时应用高亮样式
    setStyleSheet(highlightStyleSheet);
    press = true;
}

void MyHorizontalSlider::leaveEvent(QEvent *event) {
    QSlider::leaveEvent(event);
    // 当鼠标离开滑动条区域时执行的代码
    qDebug() << "Mouse left the slider";
    // 鼠标离开时恢复正常样式
    setStyleSheet(normalStyleSheet);
    press = false;
}
