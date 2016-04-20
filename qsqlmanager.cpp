#include "qsqlmanager.h"
#include "global.h"
#include "qfile.h"
#include "qdir.h"

QSQLManager* QSQLManager::instance = nullptr;
QString QSQLManager::dbname="db";
QSqlDatabase QSQLManager::db;
QString QSQLManager::tablename="dbtable";

QSQLManager::QSQLManager()
{

}

QSQLManager* QSQLManager::getInstance()
{
    if(instance == nullptr){
        instance = new QSQLManager();
        QString path;
        QDir dir;
        path=dir.currentPath();
        bool isCreate = QFile::exists(path+"/"+dbname);
        if(!instance->openDatabase()){
            addConsoleInfo("打开数据库失败");
        }
        if(!isCreate){
            if(!instance->createTable()){
                addConsoleInfo("创建数据表失败");
            }
        }
    }
    return instance;
}

bool QSQLManager::openDatabase()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbname);
    if(!db.open()){
        return false;
    }
    return true;
}

bool QSQLManager::createTable()
{
    QSqlQuery query(db);
    query.exec("DROP table " + tablename);
    bool result = query.exec("create table " + tablename + " (id int primary key, " +
                               "openDir varchar(255), generateDir varchar(255)" +
                             ", filterFile varchar(255)" + ")");

    if(!result)return false;
    QString insert_table = "insert into " + tablename + " values (1, '" + QDir::currentPath() +
            "', '" + QDir::currentPath() + "', '" + QDir::currentPath() + "/filter.txt'" + ")";
    if(!query.exec(insert_table)){
        return false;
    }
    return true;
}

QString QSQLManager::getOpenDir()
{
    QString select_sql = "select openDir from dbtable where id = 1";
    QSqlQuery query(db);
    query.exec(select_sql);
    while(query.next()){
        return query.value("openDir").toString();
    }
    return "";
}

bool QSQLManager::setOpenDir(QString dir)
{
    QString update_sql = "update dbtable set openDir = :dir where id = 1";
    QSqlQuery query(db);
    query.prepare(update_sql);
    query.bindValue(":dir", dir);
    return query.exec();
}

QString QSQLManager::getGenerateDir()
{
    QString select_sql = "select generateDir from dbtable where id = 1";
    QSqlQuery query(db);
    query.exec(select_sql);
    while(query.next()){
        return query.value("generateDir").toString();
    }
    return "";
}

bool QSQLManager::setGenerateDir(QString dir)
{
    QString update_sql = "update dbtable set generateDir = :dir WHERE id = 1";
    QSqlQuery query(db);
    query.prepare(update_sql);
    query.bindValue(":dir", dir);
    return query.exec();
}

bool QSQLManager::setFilterFile(QString file)
{
    QString update_sql = "update dbtable set filterFile = :file WHERE id = 1";
    QSqlQuery query(db);
    query.prepare(update_sql);
    query.bindValue(":file", file);
    return query.exec();
}

QString QSQLManager::getFilterFile()
{
    QString select_sql = "select filterFile from dbtable where id = 1";
    QSqlQuery query(db);
    query.exec(select_sql);
    while(query.next()){
        return query.value("filterFile").toString();
    }
    return "";
}
