#ifndef REGIST_H
#define REGIST_H

#include <QWidget>
#include <QTcpSocket>
#include "comment.h"

namespace Ui {
class Regist;
}

class Regist : public QWidget
{
    Q_OBJECT

public:
    explicit Regist(QWidget *parent = nullptr);
    ~Regist();

    Ui::Regist *ui;
    QTcpSocket *clientSocket;
    bool reg = false;

private:


public slots:
    void onReadyRead();
protected slots:
    void onConnected();
private slots:
    void on_pushButton_clicked();


    void on_pushButton_5_clicked();
    void on_pushButton_7_clicked();
};

#endif // REGIST_H
