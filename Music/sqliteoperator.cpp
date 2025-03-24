#include "sqliteoperator.h"

// 构造函数中初始化数据库对象，并建立数据库
SqliteOperator::SqliteOperator()
{
    //数据库是否已经连接上了
    if (QSqlDatabase::contains("qt_sql_default_connection")==true)
    {
        //直接返回数据库对象,qt_sql_default_connection代表当前数据库连接
        database = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        // 建立和SQlite数据库的连接
        database = QSqlDatabase::addDatabase("QSQLITE");
        // 设置数据库文件的名字
        database.setDatabaseName("C:/Users/28702/Desktop/music project/build-myserve-Desktop_Qt_5_9_9_MinGW_32bit-Debug/MyDataBase.db");

    }
}

// 打开数据库
bool SqliteOperator::openDb()
{
    if (!database.open())
    {
        qDebug() << "Error: Failed to connect database." << database.lastError();
    }
    else
    {
        // do something
        qDebug()<<"数据库打开"<<endl;
    }

    return true;
}

void SqliteOperator::closeDb(void)
{
    database.close();
    qDebug()<<"数据库关闭"<<endl;
}

//通用执行函数
void SqliteOperator::normalExec(QString sql)
{
    QSqlQuery sqlQuery;
    if(!sqlQuery.exec(sql))
    {
        qDebug()<<sqlQuery.lastError();
    }
    else
    {
        qDebug()<<"写入成功ok";
    }
}

//通用查询函数
void SqliteOperator::normalQuery(QSqlQuery & sqlQuery, QString sql)
{
    if(!sqlQuery.exec(sql))
    {
        qDebug()<<sqlQuery.lastError();
    }
    else
    {
        qDebug()<<"查询成功ok";
    }

}
