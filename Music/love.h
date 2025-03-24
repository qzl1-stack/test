#ifndef LOVE_H
#define LOVE_H

#include <QWidget>

namespace Ui {
class Love;
}

class Love : public QWidget
{
    Q_OBJECT

public:
    explicit Love(QWidget *parent = nullptr);
    ~Love();
    Ui::Love *ui;
private:


};

#endif // LOVE_H
