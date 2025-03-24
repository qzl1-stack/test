#ifndef MYLINEEDIT_H
#define MYLINEEDIT_H
#include <QLineEdit>
#include <QFocusEvent>

class MyLineEdit : public QLineEdit {
    Q_OBJECT

public:
    MyLineEdit(QWidget *parent = nullptr) : QLineEdit(parent) {
        // 初始化代码，比如设置默认样式
        // 但注意，我们不会在这里设置焦点样式，因为那将在 focusInEvent 和 focusOutEvent 中处理
    }

protected:
    // 当控件获得焦点时调用
    void focusInEvent(QFocusEvent *event) override {
        // 调用基类的实现（通常是可选的，但在这里我们不需要它）
         QLineEdit::focusInEvent(event);

        // 改变样式以指示控件现在具有焦点

        this->setStyleSheet("border: 1px solid white;background-color: rgba(90, 90, 90, 90);border-radius: 10px;");

    }

    // 当控件失去焦点时调用
    void focusOutEvent(QFocusEvent *event) override {
        // 调用基类的实现（同样是可选的）
        // QLineEdit::focusOutEvent(event);
        this->setStyleSheet(""); // 清除样式
    }

};
#endif // MYLINEEDIT_H
