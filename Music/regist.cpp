#include "regist.h"
#include "ui_regist.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QVector>

Regist::Regist(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Regist)
{
    ui->setupUi(this);
    ui->pushButton->setStyleSheet("border-image: url(:/quit.jpg); border:3px solid black; border-radius: 16px; ");
    ui->pushButton_5->setStyleSheet("border-image: url(:/reg.jpg); border:3px solid black; border-radius: 20px; ");

    QPixmap pixmap(":/head.jpg");
    pixmap = pixmap.scaled(ui->label_2->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->label_2->setPixmap(pixmap);

//    ui->label_2->setStyleSheet("border:3px solid black; border-radius: 20px; ");


//   ui->label->setStyleSheet("QLabel { background-image: url(:/head.jpg); background-repeat: no-repeat; background-position: center; background-size: cover; border: 1px solid black; border-radius: 40px; }");

    QFile file(":/style.qss");//构建一个QFILE对象
    file.open(QIODevice::ReadOnly);//打开构造函数中指定的文件

    QTextStream in(&file);//创建一个文本流，参数为打开的文件
    //如果是UTF8的文件则需要设置为
    in.setCodec("UTF-8");//设置读取编码为UTF8的文件
    QString qs = in.readAll();//读取所有的文件内容
    file.close();

    this->setStyleSheet(qs);
    file.close();
    this->setWindowFlag(Qt::FramelessWindowHint);//把主窗口边框去掉
    this->setAttribute(Qt::WA_TranslucentBackground);//把窗口设置为透明

    // 创建客户端对象
    clientSocket = new QTcpSocket(this);

    // 尝试连接到服务器
    clientSocket->connectToHost("192.168.230.1", 9000);

    // 连接信号

    connect(clientSocket, &QTcpSocket::connected, this, &Regist::onConnected);
    //readyRead是有消息可读的信号
    connect(clientSocket, &QTcpSocket::readyRead, this, &Regist::onReadyRead);



}

Regist::~Regist()
{
    delete ui;
}

void Regist::onReadyRead()
{

     QMSG msg;

     clientSocket->read((char*)&msg,sizeof(msg));//读取反过来的所有数据
     if(msg.type == 1)
     {
         if(strcmp(msg.buf,"登陆成功"))
         {
             qDebug()<<"登陆成功";
             reg = true;
             close();

         }
     }

}

void Regist::onConnected()
{
    qDebug()<<"连接成功了"<<endl;
}

void Regist::on_pushButton_clicked()
{
    close();
}


//登录
void Regist::on_pushButton_5_clicked()
{
    QMSG msg;
    msg.type = 2;
    QString str = ui->lineEdit_3->text();//名字
    std::string str1 = str.toStdString();

    QString str_pw = ui->lineEdit_4->text();//密码
    std::string str_pw1 = str_pw.toStdString();//转成标准的字符串才能获得里面的char *


    //定义一个注册结构体
    register_node rn(str1.c_str(),str_pw1.c_str(),0);
    memcpy(msg.buf,&rn,sizeof(rn));//把注册结构体装入传输结构体

    clientSocket->write((char*)&msg,sizeof(msg));
}


//注册
void Regist::on_pushButton_7_clicked()
{
    QMSG msg;
    msg.type = 1;
    QString str = ui->lineEdit_3->text();//名字
    std::string str1 = str.toStdString();


    QString str_pw = ui->lineEdit_4->text();//密码
    std::string str_pw1 = str_pw.toStdString();//转成标准的字符串才能获得里面的char *



    //定义一个注册结构体
    register_node rn(str1.c_str(),str_pw1.c_str(),1);
    memcpy(msg.buf,&rn,sizeof(rn));//把注册结构体装入传输结构体

    clientSocket->write((char*)&msg,sizeof(msg));
}
