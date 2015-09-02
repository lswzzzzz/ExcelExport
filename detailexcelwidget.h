#ifndef DETAILEXCELWIDGET_H
#define DETAILEXCELWIDGET_H

#include <QWidget>
#include <QLayout>
#include <QtXlsx>
#include <QPushButton>
#include <QCheckBox>
#include <QAxWidget>
#include <QAxObject>

QTXLSX_USE_NAMESPACE

class DetailExcelWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DetailExcelWidget(QWidget *parent = 0);
    void getXlsxData(QString filename);
    void getSheetData(Worksheet * sheet);
    void readRow(Worksheet* work_sheet, int row, int colCount, int index);
    void readHead(Worksheet * work_sheet);
    void readType(Worksheet * work_sheet);
    void writeToJson();
    void deleteVec();
    void addToExcelWidget();
    void translateTableToJson();
    void writeStart();
    bool checkValid(Worksheet* sheet);
signals:

public:
    struct ItemData{
        QCheckBox* Checkbox;
    };
    int checkInSheetVec(QString name);
public slots:

private:
    QHBoxLayout* hlayout;
    QGridLayout* glayout;
    QVBoxLayout* vlayout;
    QVector<QVector<ItemData*>*> m_vec;
    QVector<QVector<QStringList>*> dataVec;
    QVector<QVector<QString>*> typeVec;
    QVector<QString> sheetName;
    QVector<QPushButton*> sheetBuVec;
    QVector<QWidget*> widgetVec;
    QVector<QWidget*> buttonVec;
    QVector<QAxObject*> qAxVec;
    QString file_name;
    QXlsx::Document* xlsx;
    QAxObject* excel;
};

#endif // DETAILEXCELWIDGET_H
