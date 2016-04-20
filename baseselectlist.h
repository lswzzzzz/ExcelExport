#ifndef BASESELECTLIST_H
#define BASESELECTLIST_H
#include "QListWidget"
#include "qmessageinputdialog.h"

class BaseSelectList : public QListWidget
{
    Q_OBJECT
public:
    BaseSelectList(QWidget* parent = 0);
    void addContext(QString name);
    QStringList getItemList();
private:
    void contextMenuEvent(QContextMenuEvent *event);
    void connect__();
private slots:
    void itemDoubleClicked(QListWidgetItem* item);
    void itemClicked(QListWidgetItem *item);
    void add_Field();
    void delete_Field();
    void addItemSLot(QString name);
private:
    QMessageInputDialog* dialog;
};

#endif // BASESELECTLIST_H
