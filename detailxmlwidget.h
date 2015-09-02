#ifndef DETAILXMLWIDGET_H
#define DETAILXMLWIDGET_H
#include "QWidget"
#include "QDomDocument"
#include "QLayout"
#include "QVector"
#include "QCheckBox"
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonarray.h>
#include <qtextstream.h>

class DetailXmlWidget : public QWidget
{
    Q_OBJECT
public:
    DetailXmlWidget();
    ~DetailXmlWidget();
    void getXMLData(QString filename);
    void readRow(QDomElement& node);
    void readHead(QDomElement& node);
    void readType(QDomElement& node);
    void clearVector();
    void addToXmlWidget();
    void writeToJson();
    void translateTableToJson();


public:
    struct ItemData{
        QCheckBox* Checkbox;
    };
private:
    QGridLayout* glayout;
    QVector<ItemData*> m_vec;
    QString openFile;
    QVector<QStringList> dataVec;
    QVector<QString> typeVec;
};

#endif // DETAILXMLWIDGET_H
