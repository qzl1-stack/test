#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QMovie>

#include "customdelegete.h"
#include "mainwindow.h"
#include "ui_love.h"
#include "ui_song.h"
#include "myhorizontalslider.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    pRegist = new Regist();
    pLove = new Love(ui->frame);
    pLove->hide();
    pLove->move(0,50);

    pSong = new Song(ui->frame);
    pSong->close();
    pSong->move(150,150);

    sqliteOP = new SqliteOperator;//创建数据连接
    sqliteOP->openDb();

    //tableview表格制作歌单
    pModel = new QStandardItemModel(ui->tableView);//实例化模型，并且指定模型依赖的控件
    ui->tableView->setModel(pModel);

    //customDelegete代理tableview
    ui->tableView->setItemDelegateForColumn(4,new customDelegete);
    ui->tableView->setItemDelegate(new customDelegete);

    titles << "歌名" << "歌手" << "专辑" << "时长" <<"收藏";


    QString buttonStyleSheet = R"(
                               QPushButton#pushButton_3 {
                               background-image: url(:/play.png);
                               background-repeat: no-repeat;
                               background-position: center;
                               border: 0px solid;
                               border-radius: 16px;
                               padding: 0; /* 移除内边距，确保图片不被遮挡 */
                               }
                               )";


    ui->pushButton_3->setStyleSheet(buttonStyleSheet);

    ui->pushButton_2->setStyleSheet("border-image: url(:/Edit_close.png); border:0px solid black; border-radius: 0px; ");
    ui->pushButton->setStyleSheet("border-image: url(:/OP_Search.png); border:0px solid black; border-radius: 0px; ");
    ui->pushButton_4->setStyleSheet("border-image: url(:/pre.png); border:0px solid black; border-radius: 0px; ");
    ui->pushButton_5->setStyleSheet("border-image: url(:/next.png); border:0px solid black; border-radius: 0px; ");
    ui->pushButton_6->setStyleSheet("border-image: url(:/love.png); border:0px solid black; border-radius: 0px; ");
    ui->label_10->setStyleSheet("border-image: url(:/logo.png); border:0px solid black; border-radius: 0px; ");
    ui->label_13->setStyleSheet("border-image: url(:/reg2.png);border:0px;");
    ui->label_14->setStyleSheet("border-image: url(:/exit.png);border:0px;");
    ui->pushButton_20->setStyleSheet("border-image: url(:/shunxu.svg); border:0px solid black; border-radius: 0px; ");
    ui->pushButton_21->setStyleSheet("border-image: url(:/voice.svg); border:0px solid black; border-radius: 0px; ");
    ui->pushButton_22->setStyleSheet("border-image: url(:/24gl-playlist.svg); border:0px solid black; border-radius: 0px; ");

    //登录图标头像
    QPixmap pixmap(":/login_User .png");
//    pixmap = pixmap.scaled(ui->label_18->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

    ui->label_18->setPixmap(pixmap);
    ui->label->setScaledContents(true);
    //将表头透明
    ui->tableView->horizontalHeader()->setStyleSheet("QHeaderView::section { background: transparent; color: rgb(110, 110, 110);};");

    //左上角不能设置为圆角
    ui->mywidget11->setStyleSheet("#mywidget11 { border-left: 1px solid gray;border-top: 1px solid gray; border-radius: 10px ;background-color: rgb(39, 39, 39);}");
    ui->frame->setStyleSheet("#frame {border-top: 1px solid gray;border-right: 1px solid gray; border-radius: 10px; background-color: rgb(15, 15, 15);}");
    ui->centralwidget->setStyleSheet("#centralwidget { border: 1px solid grey; border-radius: 10px; background-color: rgb(39, 39, 39); }");

    this->setWindowFlag(Qt::FramelessWindowHint);//把主窗口边框去掉
    this->setAttribute(Qt::WA_TranslucentBackground);//把窗口设置为透明
    QFile file(":/style.qss");//构建一个QFILE对象
    file.open(QIODevice::ReadOnly);//打开构造函数中指定的文件
    QTextStream in(&file);//创建一个文本流，参数为打开的文件
    //    in.setCodec("gbk");//设置读取编码为GBK的文件
    //如果是UTF8的文件则需要设置为
    in.setCodec("UTF-8");//设置读取编码为GBK的文件
    QString qs = in.readAll();//读取所有的文件内容
    file.close();
    this->setStyleSheet(qs);



    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(replyFinished(QNetworkReply*)));





    playlist = new QMediaPlaylist;
    playlist->setPlaybackMode(QMediaPlaylist::Loop);
    player = new QMediaPlayer;
    player->setPlaylist(playlist);



    connect(player,SIGNAL(positionChanged(qint64)),this,SLOT(slot_positionChanged(qint64)));//滑块进度和播放歌曲进度匹配

    connect(player,SIGNAL(durationChanged(qint64)),this,SLOT(slot_durationChanged(qint64))); //总进度变化信号连接槽函数

    QSlider * progress = ui->horizontalSlider;

    connect(progress,SIGNAL(sliderMoved(int)),this,SLOT(slot_sliderMoved(int))); //滑块移动的信号 （即拖动滑块）

    connect(progress, &QSlider::valueChanged, this, &MainWindow::onSliderValueChanged);

    ui->mywidget2->hide();
    ui->label_20->close();

//    QMovie *movie = new QMovie(":/20240918214033.gif");
//    ui->giflable->setMovie(movie);
//    movie->start();



}

MainWindow::~MainWindow()
{
    delete ui;
}


//收藏
void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    if(index.column() != 4)//如果不是第四列就不要改变
    {
        return;
    }

    pRegist->onReadyRead();




    QVariant ret = pModel->data(index);//获得索引标识的模型数据
    QStandardItem * item = pModel->item(index.row(),index.column());
    delete item;//释放掉原有的选项
    //    以下代码要实际开发的时候要结合数据库的插入和删除操作
    if(ret == "未收藏")
    {
        pModel->setItem(index.row(),index.column(),new QStandardItem("已收藏"));
        song _song = song_List[index.row()];
        qDebug()<<_song.album_name<<" "<<_song.singer<<" "<<_song.songname<<" "<<_song.duration2<<" "<<_song.music_id<<" "<<_song.picture_id<<endl;

        //获取当前登录者的用户名
        QString username = pRegist->ui->lineEdit_3->text();

        const char *tableName = username.toStdString().c_str(); // 你可以根据需要更改这个值
        QSqlQuery sqlQuery;//这个结构体用来存放查询的结果
        sqliteOP->normalQuery(sqlQuery, QString("SELECT * FROM %1 WHERE music_id = %2").arg(tableName, QString::number(_song.music_id)));
        bool exists = sqlQuery.next();
        if(exists)
        {
            qDebug()<<"歌曲已存在";
        }

        if (!exists)
        {
            char sql[256];
            snprintf(sql, sizeof(sql), "CREATE TABLE %s (songname VARCHAR(128), singer VARCHAR(128), album_name VARCHAR(128), duration2 INT, music_id INT, picture_id VARCHAR(128))", tableName);
            sqliteOP->normalExec(sql);
            //将buf中的内容写入数据库
            QString sqlTemplate = "INSERT INTO %1 (songname, singer, album_name, duration2, music_id, picture_id) VALUES ('%2', '%3', '%4', %5, %6, '%7')";
            QString sql1 = sqlTemplate
                    .arg(tableName) // 替换表名
                    .arg(_song.songname)
                    .arg(_song.singer)
                    .arg(_song.album_name)
                    .arg(_song.duration2)
                    .arg(_song.music_id)
                    .arg(_song.picture_id);
            sqliteOP->normalExec(sql1);

        }


    }
    else if(ret == "已收藏")
    {
        pModel->setItem(index.row(),index.column(),new QStandardItem("未收藏"));

        song _song = song_List[index.row()];

        qDebug()<<_song.album_name<<" "<<_song.singer<<" "<<_song.songname<<" "<<_song.duration2<<" "<<_song.music_id<<" "<<_song.picture_id<<endl;

        //获取当前登录者的用户名
        QString username = pRegist->ui->lineEdit_3->text();



        const char *tableName = username.toStdString().c_str();

        int musicIdToDelete = _song.music_id; // 假设这是你想要删除的音乐ID

        QSqlQuery sqlQuery;//这个结构体用来存放查询的结果
        //删除操作
        sqliteOP->normalQuery(sqlQuery, QString("DELETE FROM %1 WHERE music_id = %2").arg(tableName, QString::number(musicIdToDelete)));


    }

}


//双击播放
void MainWindow::on_tableView_doubleClicked(const QModelIndex &index)
{
    qDebug()<<index.row()<<"111111111";
    _song_Now = song_List[index.row()];

    playlist->setCurrentIndex(index.row());//双击的行数就是播放列表中歌曲的下标
    if(state == true)
    {
        qDebug()<<"tttttttttttttttttt";
        qDebug()<<index.row();
        qDebug()<<playlist->currentIndex();
//        player->setPlaylist(playlist);
        qDebug()<<playlist->currentIndex();
//        QMediaPlaylist * playlist_while = new QMediaPlaylist;
//        playlist_while = playlist;
//        playlist_while->setPlaybackMode(QMediaPlaylist::Loop);
//        player->setPlaylist(playlist_while);
        player->play();
    }
    else if(state == false)
    {
        qDebug()<<"ffffffffffffff";
        QString play_url = QString("http://music.163.com/song/media/outer/url?id=%1.mp3").arg(_song_Now.music_id);
        playlist_One = new QMediaPlaylist;
        playlist_One->addMedia(QUrl(play_url));

        // 设置播放模式为循环
        playlist_One->setPlaybackMode(QMediaPlaylist::Loop);
        player->setPlaylist(playlist_One);
        player->play();

    }




    qDebug()<<_song_Now.album_name<<" "<<_song_Now.singer<<" "<<_song_Now.songname<<" "<<_song_Now.duration2<<" "<<_song_Now.music_id<<endl;

    //设置歌曲图片
    QString encryptedString = encryptedId(_song_Now.picture_id);
    QString url = QString("http://p1.music.126.net/%1/%2.jpg").arg(encryptedString, _song_Now.picture_id);
    QNetworkAccessManager * manager2 = new QNetworkAccessManager;
    manager2->get(QNetworkRequest(QUrl(url)));
    connect(manager2,&QNetworkAccessManager::finished,this,&MainWindow::replyFinished_2);

    //设置歌词
//    QString url_4 = QString("https://music.163.com/api/song/lyric?id=%1&lv=1&kv=1&tv=-1").arg(_song.music_id);
//    QNetworkAccessManager * manager4 = new QNetworkAccessManager;
//    manager4->get(QNetworkRequest(QUrl(url_4)));
//    connect(manager4,&QNetworkAccessManager::finished,this,&MainWindow::replyFinished_4);


    songname = song_List.at(index.row()).songname;
    singer = song_List.at(index.row()).singer;
    ui->label_3->setText(songname);
    ui->label_8->setText(singer);


    QString buttonStyleSheet = R"(
                               QPushButton#pushButton_3 {
                               background-image: url(:/stop.png);
                               background-repeat: no-repeat;
                               background-position: center;
                               border: 0px solid;
                               border-radius: 16px;
                               padding: 0; /* 移除内边距，确保图片不被遮挡 */
                               }
                               )";
    ui->pushButton_3->setStyleSheet(buttonStyleSheet);
}

//搜索按钮
void MainWindow::on_pushButton_clicked()
{
    ui->widget_4->show();
    ui->widget_3->show();
//    ui->mywidget->show();
    ui->label_17->show();
    ui->mywidget->show();
    ui->label_3->show();
    ui->label_8->show();
    ui->widget->show();
    ui->pushButton_6->show();
    pLove->close();
    pSong->close();
    ui->mywidget11->setStyleSheet("#mywidget11 { border-left: 1px solid gray;border-top: 1px solid gray; border-radius: 10px ;background-color: rgb(39, 39, 39);}");
    ui->frame->setStyleSheet("#frame {border-top: 1px solid gray;border-right: 1px solid gray; border-radius: 10px; background-color: rgb(15, 15, 15);}");
    ui->mywidget->setStyleSheet("");
    ui->tableView->move(40,60);

    //重新搜索 需清空播放列表 并删除tableview中所有内容
    playlist->clear();
    song_List.clear();
    int rowCount = pModel->rowCount();
    if (rowCount > 0) {
        // 从第0行开始，删除所有行
        pModel->removeRows(0, rowCount);
    }

    QString key = ui->lineEdit->text();

    ui->label_2->setText(ui->lineEdit->text());

    QString url = QString("http://music.163.com/api/search/get/web?csrf_token=hlpretag=&hlposttag=&s=%1&type=1&offset=0&total=true&limit=10").arg(key);

    manager->get(QNetworkRequest(QUrl(url)));

}

void MainWindow::replyFinished(QNetworkReply* reply)
{
    qDebug()<<"网页响应已完成"<<endl;
    QVariant ret = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    qDebug()<<"网页返回状态为"<<ret<<endl;
    if(ret == 200)//200说明页面访问成功
    {
        QByteArray data = reply->readAll();//读取所有的网络数据
        parseJson(data);
    }

}

void MainWindow::replyFinished_2(QNetworkReply* reply)
{
    qDebug()<<"网页响应已完成"<<endl;
    QVariant ret = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    qDebug()<<"网页返回状态为"<<ret<<endl;
    if(ret == 200)//200说明页面访问成功
    {
        QByteArray jsonByte = reply->readAll();//读取所有的网络数据
        QPixmap pixmap;
        pixmap.loadFromData(jsonByte);

        // 设置pixmap到label（如果你还需要的话）
        ui->label_19->setPixmap(pixmap);

        pSong->ui->label->setPixmap(pixmap);

        // 将QPixmap转换为QImage
        QImage image = pixmap.toImage();

        // 检查图像是否有效
        if (image.isNull()) {
            qDebug() << "Image conversion failed or image is null";
            return;
        }

        // 初始化颜色累加器
        int totalRed = 0, totalGreen = 0, totalBlue = 0;

        // 遍历图像的每一个像素
        for (int y = 0; y < image.height(); ++y) {
            for (int x = 0; x < image.width(); ++x) {
                QRgb pixel = image.pixel(x, y);

                // 提取RGB值
                int red = qRed(pixel);
                int green = qGreen(pixel);
                int blue = qBlue(pixel);

                // 累加RGB值
                totalRed += red;
                totalGreen += green;
                totalBlue += blue;
            }
        }

        // 计算平均颜色
        int averageRed = totalRed / (image.width() * image.height());
        int averageGreen = totalGreen / (image.width() * image.height());
        int averageBlue = totalBlue / (image.width() * image.height());

        // 创建一个表示平均颜色的QRgb值
        averageColor = qRgb(averageRed, averageGreen, averageBlue);

//        int newRed = qMin(static_cast<int>(averageRed + 50), 200);
//        int newGreen = qMin(static_cast<int>(averageGreen + 50), 200);
//        int newBlue = qMin(static_cast<int>(averageBlue + 50), 200);


        lighterColor = qRgb(averageRed, averageGreen, averageBlue);

//        qDebug() << "lighterColor Color:" << newRed << newGreen << newBlue;
        // 输出结果
        qDebug() << "Average Color:" << averageRed << averageGreen << averageBlue;
        // 如果你需要将QRgb转换为更通用的颜色格式（如QColor），可以使用QColor(averageColor)

        qDebug() << "Image downloaded successfully and average color calculated";


        QColor averageColorQColor(averageColor);

        // 创建一个 QPalette 对象
        QPalette palette;

        // 设置背景色为计算出的平均颜色
        palette.setColor(QPalette::Background, averageColorQColor);

        // 将 QPalette 应用于 centralwidget
        ui->centralwidget->setPalette(palette);
        ui->centralwidget->setAutoFillBackground(true);

        // 使用 setStyleSheet 来设置 centralwidget 的边框
        QString borderStyle = QString("border: 1px solid grey; border-radius: 10px;");
        ui->centralwidget->setStyleSheet(QString("#centralwidget { %1 background-color: %2; }")
                                         .arg(borderStyle)
                                         .arg(averageColorQColor.name()));


        qDebug() << "Current Background Color:" << ui->mywidget11->palette().color(QPalette::Background);


    }
}

void MainWindow::replyFinished_3(QNetworkReply *reply)
{
    qDebug()<<"网页响应已完成"<<endl;
    QVariant ret = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    qDebug()<<"网页返回状态为"<<ret<<endl;
    if(ret == 200)//200说明页面访问成功
    {
        QByteArray jsonByte = reply->readAll();//读取所有的网络数据
        QPixmap pixmap;
        pixmap.loadFromData(jsonByte);

        pixmap = pixmap.scaled(pLove->ui->label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        pLove->ui->label->setPixmap(pixmap);
    }

}

void MainWindow::replyFinished_4(QNetworkReply *reply)
{
    qDebug()<<"网页响应已完成"<<endl;
    QVariant ret = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    qDebug()<<"网页返回状态为"<<ret<<endl;
    if(ret == 200)//200说明页面访问成功
    {
        qDebug()<<"aaaaaaaaaaaaaaaa";
        QByteArray jsonData = reply->readAll();//读取所有的网络数据
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
         QJsonObject jsonObject = jsonDoc.object();

         // 假设歌词存储在名为"lrc"的键下，具体键名需要根据你的JSON结构来
         // 注意：这里的"lrc"是直接从你提供的JSON中假设的，但你的实际JSON中使用的是嵌套在"lrc"对象中的"lyric"
         QJsonObject lrcObject = jsonObject["lrc"].toObject();
         QString lyrics = lrcObject["lyric"].toString();
         pSong->ui->textEdit->setText(lyrics);

//        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
//        if (json_error.error == QJsonParseError::NoError)//判断解析过程是否有错误
//        {
//            if (jsonDoc.isObject())//判断文档是否是一个json对象
//            {
//                QJsonObject rootObj = jsonDoc.object();//获取文档JSON对象
//                if (rootObj.contains("lrc"))//对象中是否包含键result
//                {
//                    QJsonValue valuedata = rootObj.value("lrc");//获得result对应的值
//                    if (valuedata.isObject())//result对应的值是否是一个对象
//                    {
//                        QJsonObject valuedataObject = valuedata.toObject();//把QJsonValue转为JsonObject对象
//                        if (valuedataObject.contains("lyric"))//歌词
//                        {
//                            QString lyrics = valuedataObject["lyric"].toString();
//                            qDebug()<<lyrics;
//                           pSong->ui->textEdit->setText(lyrics);
//                        }
//                    }
//                }
//            }
//        }
//        else
//        {
//            qDebug() << json_error.errorString();
//        }
    }
}




void MainWindow::parseJson(QByteArray jsonByte)
{
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); //表格适应tableview宽度


    QString songname_original2; //歌曲名
    QString singername2;        //歌手
    QString album_name2;        //专辑
    int duration2;          	//时间
    int music_id;               //音乐id
    QString picid;              //图片id


    QJsonParseError json_error;//JSON解析错误对象
    QJsonDocument parse_doucment = QJsonDocument::fromJson(jsonByte, &json_error);

    if (json_error.error == QJsonParseError::NoError)//判断解析过程是否有错误
    {
        if (parse_doucment.isObject())//判断文档是否是一个json对象
        {
            QJsonObject rootObj = parse_doucment.object();//获取文档JSON对象
            if (rootObj.contains("result"))//对象中是否包含键result
            {
                QJsonValue valuedata = rootObj.value("result");//获得result对应的值
                if (valuedata.isObject())//result对应的值是否是一个对象
                {
                    QJsonObject valuedataObject = valuedata.toObject();//把QJsonValue转为JsonObject对象
                    if (valuedataObject.contains("songs"))
                    {
                        QJsonValue valueArray = valuedataObject.value("songs");
                        if (valueArray.isArray())
                        {

                            //把结果转为QJsonArray数组才能遍历
                            QJsonArray array = valueArray.toArray();
                            int size = array.size();//获取数组大小
                            for (int i = 0; i <size; i++)
                            {
                                QJsonValue value = array.at(i);
                                if (value.isObject())
                                {
                                    //把数组的第i个元素变成一个JSON对象
                                    QJsonObject object = value.toObject();
                                    if (object.contains("album"))//专辑1
                                    {
                                        QJsonValue album_Object_1 = object.value("album");
                                        if (album_Object_1.isObject())
                                        {

                                            QJsonObject album_Object_1_tmp=album_Object_1.toObject();
                                            if (album_Object_1_tmp.contains("picId")) {
                                                QJsonValue PID_value = album_Object_1_tmp.value("picId");
                                                if (PID_value.isDouble())
                                                {

                                                    bool ok;
                                                    qint64 picidInt = PID_value.toVariant().toLongLong(&ok);
                                                    if (ok) {
                                                        // 转换成功
                                                        picid = QString::number(picidInt); // 转换为QString，如果你需要
                                                        qDebug() << "picId (qint64):" << picid;
                                                    } else {
                                                        // 转换失败，可能需要处理错误
                                                        qDebug() << "Failed to convert picId to qint64";
                                                    }

                                                }
                                            }

                                            if(album_Object_1_tmp.contains("name"))
                                            {

                                                QJsonValue ID_value = album_Object_1_tmp.value("name");
                                                if (ID_value.isString())
                                                {
                                                    album_name2 = ID_value.toString(); //专辑名的保存

                                                }
                                            }
                                        }
                                    }

                                    if (object.contains("artists"))//歌手
                                    {
                                        QJsonValue artists_value_tmp = object.value("artists");
                                        if(artists_value_tmp.isArray())
                                        {
                                            QJsonArray artists_array = artists_value_tmp.toArray();
                                            int artists_size=artists_array.size();
                                            for(int k=0;k<artists_size;k++)
                                            {
                                                QJsonValue artists_name=artists_array.at(k);
                                                if(artists_name.isObject())
                                                {
                                                    QJsonObject artists_name_object=artists_name.toObject();
                                                    if(artists_name_object.contains("name"))
                                                    {
                                                        QJsonValue artistsname = artists_name_object.take("name");
                                                        if (artistsname.isString())
                                                        {
                                                            if(k==0)
                                                            {
                                                                singername2 = artistsname.toString();
                                                            }
                                                            else
                                                            {
                                                                singername2 = singername2 + "/" +artistsname.toString();
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }

                                    if (object.contains("name"))//歌曲名
                                    {
                                        QJsonValue Album_ID_value = object.take("name");
                                        if (Album_ID_value.isString())
                                        {
                                            songname_original2 = Album_ID_value.toString();
                                        }
                                    }
                                    if (object.contains("duration"))//时长
                                    {
                                        QJsonValue AlbumID_value = object.take("duration").toInt();
                                        duration2 = AlbumID_value.toInt();
                                        duration2=duration2/1000;
                                    }
                                    if (object.contains("id"))//歌曲id
                                    {
                                        QJsonValue FileHash_value = object.take("id");
                                        if (!FileHash_value.isNull())
                                        {
                                            //用Vector保存每首歌曲的album_id
                                            music_id=FileHash_value.toInt();
                                        }
                                    }

                                    song _song;//定义一个歌曲结构体
                                    _song.album_name = album_name2;
                                    _song.singer = singername2;
                                    _song.songname = songname_original2;
                                    _song.duration2 = duration2;
                                    _song.music_id = music_id;
                                    _song.picture_id = picid;
                                    song_List.push_back(_song);//保存歌曲信息到链表


                                    //构建歌曲播放链接
                                    QString play_url = QString("http://music.163.com/song/media/outer/url?id=%1.mp3").arg(music_id);
                                    playlist->addMedia(QUrl(play_url));

                                    //设置表头
                                    pModel->setHorizontalHeaderLabels(titles);

                                    //把歌曲加入到表格显示列表
                                    QList<QStandardItem *> sti;

                                    QStandardItem *item1 = new QStandardItem(songname_original2);
                                    item1->setTextAlignment(Qt::AlignCenter);//表格居中显示
                                    sti << item1;
                                    QStandardItem *item2 = new QStandardItem(_song.singer);
                                    item2->setTextAlignment(Qt::AlignCenter);
                                    sti << item2;
                                    QStandardItem *item3 = new QStandardItem(album_name2);
                                    item3->setTextAlignment(Qt::AlignCenter);
                                    sti << item3;
                                    int minutes = duration2 / 60;
                                    int seconds = duration2 % 60;
                                    QString timeString = QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0'));
                                    QStandardItem *item4 = new QStandardItem(timeString);
                                    item4->setTextAlignment(Qt::AlignCenter);
                                    sti << item4;
                                    if(pRegist->reg == true)
                                    {
                                        QSqlQuery sqlQuery;//这个结构体用来存放查询的结果
                                        sqliteOP->normalQuery(sqlQuery, QString("SELECT * FROM %1 ").arg(pRegist->ui->lineEdit_3->text()));

                                        bool love = false;

                                        while(sqlQuery.next())
                                        {
                                            int music_id = sqlQuery.value("music_id").toInt();
                                            qDebug()<<"music_id:"<<music_id;
                                            if(music_id == _song.music_id)
                                            {
                                                love = true;
                                                QStandardItem *item5 = new QStandardItem("已收藏");
                                                item5->setTextAlignment(Qt::AlignCenter);
                                                sti << item5;
                                                break;
                                            }
                                        }
                                        if(love == false)
                                        {
                                            QStandardItem *item5 = new QStandardItem("未收藏");
                                            item5->setTextAlignment(Qt::AlignCenter);
                                            sti << item5;
                                        }

                                    }
                                    else
                                    {
                                        QStandardItem *item5 = new QStandardItem("未收藏");
                                        item5->setTextAlignment(Qt::AlignCenter);
                                        sti << item5;
                                    }


                                    pModel->appendRow(sti);
                                }
                            } 
                            ui->mywidget2->close();

                            QPixmap pixmap(":/singer.jpg");
                            pixmap = pixmap.scaled(ui->label_16->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
                            ui->label_16->setPixmap(pixmap);

                            ui->pushButton_18->setText(singername2);
                        }
                    }
                }
            }
        }
    }
    else
    {
        qDebug() << json_error.errorString();
    }

}




//退出按键
void MainWindow::on_pushButton_2_clicked()
{
    close();
}

//设置进度
void MainWindow::slot_positionChanged(qint64 position)
{
    ui->horizontalSlider->setValue(position);
}


void MainWindow::onSliderValueChanged(int value)
{
    if(ui->horizontalSlider->press == true)
    {
        int sliderWidth = ui->horizontalSlider->width();
        int posX = ui->horizontalSlider->x() + (sliderWidth * value / ui->horizontalSlider->maximum()) - ui->label_20->width() / 2;
        QPoint labelPosition =QPoint(posX, ui->horizontalSlider->y() - ui->label_20->height() - 10);

        // 更新QLabel的位置
        ui->label_20->move(labelPosition);

        ui->label_20->setText(QString("%1:%2/%3:%4").arg(value/1000/60, 2, 10, QChar('0')).arg(value/1000%60, 2, 10, QChar('0')).arg(duration_all/1000/60, 2, 10, QChar('0')).arg(duration_all/1000%60, 2, 10, QChar('0')));

        // 如果之前QLabel是隐藏的，这里可以显示它
        ui->label_20->show();
    }
    else
    {
      ui->label_20->close();
    }

}

//设置歌曲总时长
void MainWindow::slot_durationChanged(qint64 duration)
{
    ui->horizontalSlider->setRange(0, duration);
    duration_all = duration;
}

void MainWindow::slot_sliderMoved(int position)
{
    qDebug()<<position<<endl;
    player->setPosition(position);
}


//暂停和开始播放
void MainWindow::on_pushButton_3_clicked()
{
    // 如果正在播放，则暂停；如果已暂停，则恢复播放
    if (player->state() == QMediaPlayer::PlayingState) {
        player->pause();
        QString buttonStyleSheet = R"(
                                   QPushButton#pushButton_3 {
                                   background-image: url(:/play.png);
                                   background-repeat: no-repeat;
                                   background-position: center;
                                   border: 0px solid;
                                   border-radius: 16px;
                                   padding: 0; /* 移除内边距，确保图片不被遮挡 */
                                   }
                                   )";
        ui->pushButton_3->setStyleSheet(buttonStyleSheet);
    } else {
        player->play();
        QString buttonStyleSheet = R"(
                                   QPushButton#pushButton_3 {
                                   background-image: url(:/stop.png);
                                   background-repeat: no-repeat;
                                   background-position: center;
                                   border: 0px solid;
                                   border-radius: 16px;
                                   padding: 0; /* 移除内边距，确保图片不被遮挡 */
                                   }
                                   )";
        ui->pushButton_3->setStyleSheet(buttonStyleSheet);
    }
}

void MainWindow::on_pushButton_5_clicked()
{
    playlist->next();

    songname = song_List.at(playlist->currentIndex()).songname;
    singer = song_List.at(playlist->currentIndex()).singer;
    ui->label_3->setText(songname);
    ui->label_8->setText(singer);

    _song_Now = song_List[playlist->currentIndex()];
    //设置歌曲图片
    QString encryptedString = encryptedId(_song_Now.picture_id);
    QString url = QString("http://p1.music.126.net/%1/%2.jpg").arg(encryptedString, _song_Now.picture_id);
    QNetworkAccessManager * manager2 = new QNetworkAccessManager;
    manager2->get(QNetworkRequest(QUrl(url)));
    connect(manager2,&QNetworkAccessManager::finished,this,&MainWindow::replyFinished_2);

}

void MainWindow::on_pushButton_4_clicked()
{
    playlist->previous();

    songname = song_List.at(playlist->currentIndex()).songname;
    singer = song_List.at(playlist->currentIndex()).singer;
    ui->label_3->setText(songname);
    ui->label_8->setText(singer);

    _song_Now = song_List[playlist->currentIndex()];
    //设置歌曲图片
    QString encryptedString = encryptedId(_song_Now.picture_id);
    QString url = QString("http://p1.music.126.net/%1/%2.jpg").arg(encryptedString, _song_Now.picture_id);
    QNetworkAccessManager * manager2 = new QNetworkAccessManager;
    manager2->get(QNetworkRequest(QUrl(url)));
    connect(manager2,&QNetworkAccessManager::finished,this,&MainWindow::replyFinished_2);
}

//登录按钮
void MainWindow::on_pushButton_7_clicked()
{
    ui->mywidget2->setVisible(!ui->mywidget2->isVisible());
    if(pRegist->reg == true)
    {
    QPixmap pixmap(":/head.jpg");
    pixmap = pixmap.scaled(ui->label_18->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->label_18->setPixmap(pixmap);
    pixmap = pixmap.scaled(pLove->ui->label_18->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    pLove->ui->label_18->setPixmap(pixmap);
    pLove->ui->label_3->setText(pRegist->ui->lineEdit_3->text());

    }


}

void MainWindow::on_pushButton_15_clicked()
{
    pRegist->setVisible(!pRegist->isVisible());
}



QString MainWindow::encryptedId(const QString& id)
{
    QByteArray magic = "3go8&\$8*3*3h0k(2)2";
    QByteArray idBytes = id.toUtf8();
    int magicLen = magic.length();

    for (int i = 0; i < idBytes.length(); ++i) {
        idBytes[i] = idBytes[i] ^ magic[i % magicLen];
    }

    QByteArray md5 = QCryptographicHash::hash(idBytes, QCryptographicHash::Md5);
    QByteArray base64 = md5.toBase64();
    QString result = QString::fromLatin1(base64);
    result.replace("/", "_").replace("+", "-");

    return result;
}

//点击歌曲图片
void MainWindow::on_pushButton_24_clicked()
{
    ui->widget_4->close();
    ui->widget_3->close();
    ui->mywidget->close();
    ui->label_3->close();
    ui->label_8->close();
    ui->widget->close();
    pLove->close();
    ui->pushButton_6->close();
    pSong->ui->label_3->setText(songname);
    pSong->ui->label_8->setText(singer);

    pSong->show();

    QColor averageColorQColor(averageColor);

    // 创建一个 QPalette 对象
    QPalette palette;

    // 设置背景色为计算出的平均颜色
    palette.setColor(QPalette::Background, averageColorQColor);

    // 将 QPalette 应用于 centralwidget
    ui->mywidget11->setPalette(palette);
    ui->mywidget11->setAutoFillBackground(true);


    QString borderStyle = QString("border-top: 1px solid gray;border-left: 1px solid gray; border-radius: 10px;");
    ui->mywidget11->setStyleSheet(QString("#mywidget11 { %1 background-color: %2; }")
                                     .arg(borderStyle)
                                     .arg(averageColorQColor.name()));

    qDebug() << "Current Background Color:" << ui->mywidget11->palette().color(QPalette::Background);



    QColor averagelighterColor(lighterColor);
//    averagelighterColor.setAlpha(128);
//    QPalette palette1;
//    palette1.setColor(QPalette::Background, averagelighterColor);

//    ui->frame->setPalette(palette1);
//    ui->frame->setAutoFillBackground(true);

    borderStyle = QString("border-top: 1px solid gray;border-right: 1px solid gray; border-radius: 10px;");
    QString colorStr = QString("rgba(%1, %2, %3, %4)")
                       .arg(averagelighterColor.red())
                       .arg(averagelighterColor.green())
                       .arg(averagelighterColor.blue())
                       .arg(160);
    ui->frame->setStyleSheet(QString("#frame { %1; background-color: %2;}")
                                     .arg(borderStyle)
                                     .arg(colorStr));


    qDebug()<<averagelighterColor.name();

}

//点击喜欢
void MainWindow::on_pushButton_11_clicked()
{
    ui->widget_4->close();
    ui->widget_3->close();
    ui->label_17->close();
    ui->mywidget->show();
    ui->label_3->show();
    ui->label_8->show();
    ui->widget->show();
    ui->pushButton_6->show();
    pSong->close();
    ui->tableView->move(40,0);

    ui->mywidget11->setStyleSheet("#mywidget11 { border-left: 1px solid gray;border-top: 1px solid gray; border-radius: 10px ;background-color: rgb(39, 39, 39);}");
    ui->frame->setStyleSheet("#frame {border-top: 1px solid gray;border-right: 1px solid gray; border-radius: 10px; background-color: rgb(15, 15, 15);}");
    ui->mywidget->setStyleSheet("background-color: rgba(39, 39, 39,0);color: rgb(255, 255, 255);");
    playlist->clear();
    song_List.clear();
    int rowCount = pModel->rowCount();
    if (rowCount > 0) {
        // 从第0行开始，删除所有行
        pModel->removeRows(0, rowCount);
    }
    pLove->show();

    QSqlQuery sqlQuery;

    sqliteOP->normalQuery(sqlQuery, QString("SELECT * FROM %1 ").arg(pRegist->ui->lineEdit_3->text()));

    //设置表头
    pModel->setHorizontalHeaderLabels(titles);

    song _song;//定义一个歌曲结构体

    int num = 0;
    while(sqlQuery.next())
    {
        QString songname = sqlQuery.value("songname").toString();
        QString singer = sqlQuery.value("singer").toString();
        QString album_name = sqlQuery.value("album_name").toString();
        int duration2 = sqlQuery.value("duration2").toInt();
        int music_id = sqlQuery.value("music_id").toInt();
        QString picture_id = sqlQuery.value("picture_id").toString();


        _song.album_name = album_name;
        _song.singer = singer;
        _song.songname = songname;
        _song.duration2 = duration2;
        _song.music_id = music_id;
        _song.picture_id = picture_id;
        song_List.push_back(_song);
        qDebug()<<songname<<singer<<songname<<duration2<<music_id;


        //构建歌曲播放链接
        QString play_url = QString("http://music.163.com/song/media/outer/url?id=%1.mp3").arg(_song.music_id);
        playlist->addMedia(QUrl(play_url));
        qDebug()<<play_url;

        QList<QStandardItem *> sti;

        QStandardItem *item1 = new QStandardItem(_song.songname);
        item1->setTextAlignment(Qt::AlignCenter);//表格居中显示
        sti << item1;
        QStandardItem *item2 = new QStandardItem(_song.singer);
        item2->setTextAlignment(Qt::AlignCenter);
        sti << item2;
        QStandardItem *item3 = new QStandardItem(_song.album_name);
        item3->setTextAlignment(Qt::AlignCenter);
        sti << item3;
        int minutes = _song.duration2 / 60;
        int seconds = _song.duration2 % 60;
        QString timeString = QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0'));
        QStandardItem *item4 = new QStandardItem(timeString);
        item4->setTextAlignment(Qt::AlignCenter);
        sti << item4;
        QStandardItem *item5 = new QStandardItem("已收藏");
        item5->setTextAlignment(Qt::AlignCenter);
        sti << item5;

//        pModel->insertRow(0,sti);
        pModel->appendRow(sti);
        num++;
    }

    QString text1 = QString("    播放全部 %1").arg(num);
    pLove->ui->pushButton->setText(text1);
    //设置歌曲图片
    qDebug()<<_song.picture_id;
    QString encryptedString = encryptedId(_song.picture_id);
    QString url = QString("http://p1.music.126.net/%1/%2.jpg").arg(encryptedString, _song.picture_id);
    QNetworkAccessManager *manager3;
    manager3 = new QNetworkAccessManager;

    manager3->get(QNetworkRequest(QUrl(url)));

    connect(manager3,&QNetworkAccessManager::finished,this,&MainWindow::replyFinished_3);


}



void MainWindow::on_pushButton_20_clicked()
{
    if(state == true)
    {
       ui->pushButton_20->setStyleSheet("border-image: url(:/one.svg); border:0px solid black; border-radius: 0px; ");
       state = false;
    }
    else if(state == false)
    {
        ui->pushButton_20->setStyleSheet("border-image: url(:/shunxu.svg); border:0px solid black; border-radius: 0px; ");
        player->setPlaylist(playlist);
        state = true;
    }
}
