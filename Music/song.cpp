#include "song.h"
#include "ui_song.h"

Song::Song(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Song)
{
    ui->setupUi(this);
}

Song::~Song()
{
    delete ui;
}


