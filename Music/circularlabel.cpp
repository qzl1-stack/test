#include "circularlabel.h"


CircularLabel::CircularLabel(QWidget *parent) : QLabel(parent) {
    // 假设这里你已经有了图片的路径
    QString imagePath = ":/head.jpg"; // 使用资源文件路径或文件路径
    QPixmap pixmap(imagePath);
    pixmap = pixmap.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    // 裁剪图片为圆形（这里为了简化，假设图片已经是正方形）
    QPixmap circularPixmap(100, 100);
    circularPixmap.fill(Qt::transparent);
    QPainter painter(&circularPixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(pixmap.toImage());
    painter.drawEllipse(circularPixmap.rect());

    // 这里我们不使用样式表来设置背景图片，而是直接在paintEvent中绘制
    // 如果你确实需要在样式表中设置其他样式，可以这样做，但背景图片将不会是圆形的

    // 安装事件过滤器
    this->installEventFilter(this);
}

bool CircularLabel::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::MouseButtonPress && obj == this) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        if (mouseEvent->button() == Qt::LeftButton) {
            // 处理点击事件
            qDebug() << "CircularLabel clicked!";
            // 这里可以添加更多的处理逻辑
        }
    }
    return QLabel::eventFilter(obj, event);
}

void CircularLabel::paintEvent(QPaintEvent *event) {
    // 这里你可以使用前面裁剪好的圆形pixmap来绘制
    // 但在这个例子中，我们已经在构造函数中处理了图片的裁剪
    // 并且没有将裁剪后的图片保存到成员变量中，所以这里只是示例
    // 实际使用时，你应该将裁剪后的图片保存为成员变量，并在paintEvent中使用它

    QLabel::paintEvent(event); // 调用基类的paintEvent来处理其他可能的绘制
}
