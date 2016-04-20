#ifndef QSQLMANAGER_H
#define QSQLMANAGER_H

#include "qsql.h"
#include "qsqldatabase.h"
#include "qsqlquery.h"

class QSQLManager
{
public:

    static QSQLManager* getInstance();

    bool openDatabase();
    bool createTable();
    QString getOpenDir();
    bool setOpenDir(QString dir);
    QString getGenerateDir();
    bool setGenerateDir(QString dir);
    bool setFilterFile(QString file);
    QString getFilterFile();
private:
    QSQLManager();
    static QSQLManager* instance;
    static QString dbname;
    static QSqlDatabase db;
    static QString tablename;
};

#endif // QSQLMANAGER_H
