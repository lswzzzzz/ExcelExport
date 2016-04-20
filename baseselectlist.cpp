#include "baseselectlist.h"
#include "QMenu"
#include "QMessageBox"
#include <qmimedata.h>

BaseSelectList::BaseSelectList(QWidget* parent)
    :QListWidget(parent)
{
    dialog = new QMessageInputDialog(this);
    connect__();
}

void BaseSelectList::addContext(QString name)
{
    auto item = new QListWidgetItem();
    item->setText(name);
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    this->addItem(item);
}

void BaseSelectList::addItemSLot(QString name)
{
    auto item = new QListWidgetItem();
    item->setText(name);
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    this->addItem(item);
}

void BaseSelectList::connect__()
{
    connect(this, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(itemClicked(QListWidgetItem*)));
    connect(this, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(itemDoubleClicked(QListWidgetItem *)));
    connect(dialog, SIGNAL(messageDialogText(QString)), this, SLOT(addItemSLot(QString)));
}

void BaseSelectList::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu* menu = new QMenu(this);
    QAction* deleteAc = new QAction(tr("删除字段"), this);
    QAction* addAc = new QAction(tr("添加字段"), this);
    connect(deleteAc, SIGNAL(triggered()), this, SLOT(delete_Field()));
    connect(addAc, SIGNAL(triggered()), this, SLOT(add_Field()));
    auto item = this->currentItem();
    if(item){
        menu->addAction(deleteAc);
    }
    menu->addAction(addAc);
    menu->exec(QCursor::pos());
}

QStringList BaseSelectList::getItemList()
{
    int size = this->count();
    QStringList list;
    for(int i=0; i<size; i++){
        list<<this->item(i)->text();
    }
    return list;
}

void BaseSelectList::itemDoubleClicked(QListWidgetItem* item)
{

}

void BaseSelectList::itemClicked(QListWidgetItem *item)
{

}

void BaseSelectList::add_Field()
{
    dialog->show();
}

void BaseSelectList::delete_Field()
{
    auto item = currentItem();
    QModelIndex index = currentIndex();
    if(item){
        this->takeItem(index.row());
    }
}
