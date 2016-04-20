#ifndef DETAILEXCELWIDGET_H
#define DETAILEXCELWIDGET_H

#include <QWidget>
#include <QLayout>
#include <QtXlsx>
#include <QPushButton>
#include <QCheckBox>
#include "batchfilterthread.h"
#include "batchgeneratethread.h"

QTXLSX_USE_NAMESPACE

class DetailExcelWidget : public QWidget
{
    Q_OBJECT
public:
    ~DetailExcelWidget();
    explicit DetailExcelWidget(QWidget *parent = 0);
    void getXlsxData(QString filename);
    void getSheetData(Worksheet * sheet);
    void readHead(Worksheet * work_sheet);
    void readType(Worksheet * work_sheet);
    void writeToJson();
    void deleteVec();
    void deleteBatchVec();
    void addToExcelWidget();
    void checkError();
    bool checkValid(Worksheet* sheet);
    void BatchGenerate(QVector<int> vec);
    void BatchCheck(QVector<int> vec);
protected slots:
    void findError(QString filename);
    void noError(QString filename);
    void writeJsonFinished(QString filename);
public:
    struct ItemData{
        QCheckBox* Checkbox;
    };
public slots:

private:
    QHBoxLayout* hlayout;
    QGridLayout* glayout;
    QVBoxLayout* vlayout;
    QVector<QVector<ItemData*>*> m_vec;
    QVector<QVector<QString>*> typeVec;
    QVector<QString> sheetName;
    QVector<QWidget*> widgetVec;
    QVector<QWidget*> buttonVec;
    QString file_name;
    QXlsx::Document* xlsx;
    QVector<BatchFilterThread*> filterVec;
    QVector<BatchGenerateThread*> generateVec;
    QStringList errorFileList;
    int BatchTotal;
    int curCount;
    bool isBatch;
};

#endif // DETAILEXCELWIDGET_H
