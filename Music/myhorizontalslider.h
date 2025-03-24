#ifndef MYHORIZONTALSLIDER_H
#define MYHORIZONTALSLIDER_H
#include <QSlider>
#include <QEvent>
#include <QDebug>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>
#include <QMouseEvent>


class MyHorizontalSlider : public QSlider {
    Q_OBJECT
    QString normalStyleSheet; // 存储正常状态下的样式表
    QString highlightStyleSheet; // 存储高亮状态下的样式表
public:
    MyHorizontalSlider(QWidget *parent = nullptr);
//    ~MyHorizontalSlider();
    bool press = false;

protected:
    void enterEvent(QEvent *event) override;

    void leaveEvent(QEvent *event) override;



};

#endif // MYHORIZONTALSLIDER_H
