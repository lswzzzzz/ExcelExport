#ifndef BATCHFILTERTHREAD_H
#define BATCHFILTERTHREAD_H

#include "qthread.h"
#include "QtXlsx"
#include "QFile"
#include "QTextStream"

QTXLSX_USE_NAMESPACE

class BatchFilterThread : public QThread
{
    Q_OBJECT
public:
    enum dataType{
        INT_TYPE,
        NUMBER_TYPE,
        STRING_TYPE,
        STRING_INT_TYPE,
        STRING_NUMBER_TYPE,
        STRING_STRING_TYPE,
        HELPER_TYPE,
        CUSTOM_TYPE
    };

    BatchFilterThread(QObject* parent = 0);
    void run();
    void setFile(QString file);
    void getData();
    bool checkValid(Worksheet* sheet);
    void readRow(Worksheet* work_sheet, int row);
    void readHead(Worksheet* work_sheet);
    void readType(Worksheet * work_sheet);
    void findStringErrorChar(QString str, int row, int col);
    void findStringIntErrorChar(QString str, int row, int col, bool fixedLength);
    void findStringNumberErrorChar(QString str, int row, int col);
    void findStringStringErrorChar(QString str, int row, int col);
    void setNoEmptyList();
    void deleteVec();
    void getFilterData();
    void changeToFilterType(QVector<QVector<QString>*> vec);
    void addNameVec(QVector<QStringList> vec);
signals:
    void hasErrorSignal(QString filename);
    void noErrorSignal(QString filename);

private:
    QVector<QVector<dataType>> p_typeVec;
    QVector<QStringList> p_nameVec;
    QString m_filename;
    QXlsx::Document* xlsx;
    Worksheet* sheet;
    QVector<dataType> typeVec;
    QStringList nameList;
    int intlength;
    int minInt;
    QString filterStr;
    QStringList errorList;
    QString sheetname;
    QVector<int> noEmptyColList;
    int dealIndex;
};

#endif // BATCHFILTERTHREAD_H
