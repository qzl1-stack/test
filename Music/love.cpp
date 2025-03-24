#include "love.h"
#include "ui_love.h"


Love::Love(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Love)
{
    ui->setupUi(this);

}

Love::~Love()
{
    delete ui;
}
