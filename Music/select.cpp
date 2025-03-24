#include "select.h"
#include "ui_select.h"

select::select(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::select)
{
    ui->setupUi(this);
    move(100, 100);
}

select::~select()
{
    delete ui;
}

void select::setInitialPosition(int x, int y)
{
    move(x, y); // 将对话框移动到指定的(x, y)位置
}
