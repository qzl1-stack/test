#ifndef MOVABLECENTRALWIDGET_H
#define MOVABLECENTRALWIDGET_H


#include <QWidget>
#include <QMouseEvent>
#include <QTimer>

class MovableCentralWidget : public QWidget {
    Q_OBJECT

public:
    MovableCentralWidget(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

private slots:
    void startDragging();

private:
    bool isDragging;
    QPoint dragStartPosition;
    QTimer *dragStartTimer;
};

#endif // MOVABLECENTRALWIDGET_H
