#ifndef SONG_H
#define SONG_H

#include <QWidget>
#include <QNetworkReply>

namespace Ui {
class Song;
}

class Song : public QWidget
{
    Q_OBJECT

public:
    explicit Song(QWidget *parent = nullptr);
    ~Song();
    Ui::Song *ui;

private:

public slots:


};

#endif // SONG_H
