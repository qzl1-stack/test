#ifndef SELECT_H
#define SELECT_H

#include <QDialog>

namespace Ui {
class select;
}

class select : public QDialog
{
    Q_OBJECT

public:
    explicit select(QWidget *parent = nullptr);
    ~select();

    void setInitialPosition(int x, int y);
private:
    Ui::select *ui;
};

#endif // SELECT_H
