#ifndef COMMENT_H
#define COMMENT_H
#include <stdio.h>
#include <string.h>

struct QMSG
{
    char buf[1022];
    short type;
};

struct Music_ID
{
    int arr[254];
    int len;
};


struct register_node
{
    register_node(const char * name , const char * password , bool sex) {
        strcpy(this->name,name);
        strcpy(this->password,password);
        this->sex = sex;
    }
    char name[128];
    char password[128];
    bool sex;
};


struct song_loved
{
    song_loved(const char *songname,const char *singer,const char *album_name,const int duration,const int music_id,const char *picture_id,const char *username)
    {
        strcpy(this->songname,songname);
        strcpy(this->singer,singer);
        strcpy(this->album_name,album_name);
        this->duration2 = duration;
        this->music_id = music_id;
        strcpy(this->picture_id,picture_id);
        strcpy(this->username,username);
    }


    char songname[128];   //歌曲名
    char singer[128];     //歌手
    char album_name[128]; //专辑
    int duration2;      //时间
    int music_id;       //音乐id
    char picture_id[128]; //图片id
    char username[128];
};

#endif // COMMENT_H
