#ifndef BATCHGENERATETHREAD_H
#define BATCHGENERATETHREAD_H
#include "qthread.h"
#include <QtXlsx>
#include <QAxWidget>
#include <QAxObject>

QTXLSX_USE_NAMESPACE

class BatchGenerateThread : public QThread
{
    Q_OBJECT
public:
    enum StringType{
        INT,
        STRING,
        NUMBER,
    };

    BatchGenerateThread(QObject* parent = 0);
    void run();
    void init();
    void unload();
    void setXlsxFile(QString file);
    void getSheetData(Worksheet * sheet);
    void transtoJson(QString name);
    bool checkValid(Worksheet* sheet);
    QStringList readRow(Worksheet* work_sheet, QAxObject* axSheet, int row);
    void readType(Worksheet * work_sheet);
    void readHead(Worksheet* work_sheet);
    QVariantMap createARowMap(QStringList strList);
    void setIgnoreList();
    void deleteVec();
    void addNameVec(QVector<QStringList> vec);
    void addTypeVec(QVector<QVector<QString>*> vec);

    QVariantList getStringArray(QString str, StringType type_);
public:

signals:
    void writeFinishSignal(QString filename);
private:
    QVector<QStringList> p_nameVec;
    QVector<QStringList> p_typeVec;
    QString xlsxFile;
    QXlsx::Document* xlsx;
    Worksheet* sheet;
    QAxObject* excel;
    QAxObject *workbook;
    QStringList nameList;
    QStringList typeList;
    QVariantMap map;
    QVector<int> ignoreColList;
    int dealIndex;
};

#endif // BATCHGENERATETHREAD_H
