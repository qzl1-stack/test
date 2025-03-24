#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QStandardItemModel>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QTcpSocket>
#include <QVBoxLayout>
#include "regist.h"
#include "ui_regist.h"
#include "love.h"
#include "song.h"
#include "sqliteoperator.h"



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct song
{
    QString songname;   //歌曲名
    QString singer;     //歌手
    QString album_name; //专辑
    int duration2;      //时间
    int music_id;       //音乐id
    QString picture_id; //图片id
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QVBoxLayout *layout;
    Regist * pRegist;
    Love *pLove;
    Song *pSong;
    qint64 duration_all;
    void parseJson(QByteArray jsonByte);
    QString encryptedId(const QString &id);
    QRgb averageColor;
    QRgb lighterColor;



public slots:
    void replyFinished(QNetworkReply *reply);

    void replyFinished_2(QNetworkReply *reply);

    void replyFinished_3(QNetworkReply *reply);

    void replyFinished_4(QNetworkReply *reply);
private slots:
    void on_tableView_clicked(const QModelIndex &index);

    void on_pushButton_clicked();

    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_pushButton_2_clicked();

    void slot_positionChanged(qint64 position);

    void onSliderValueChanged(int value);


    void slot_durationChanged(qint64 duration);

    void slot_sliderMoved(int position);

    void on_pushButton_3_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_24_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_20_clicked();

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager *manager;
    QMediaPlaylist * playlist;
    QMediaPlayer * player;
    QMediaPlaylist * playlist_One;
    QList<song> song_List;
    QStandardItemModel * pModel;
    QStringList titles;
    SqliteOperator * sqliteOP;
    QString songname;
    QString singer;
    song _song_Now;
    bool state = true;


};
#endif // MAINWINDOW_H
