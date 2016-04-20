#include "fielddialog.h"
#include "ui_fielddialog.h"
#include "QLabel"
#include "QLayout"

FieldDialog::FieldDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FieldDialog)
{
    ui->setupUi(this);
    addLayout__();
    connect__();
}

FieldDialog::~FieldDialog()
{
    delete ui;
}

void FieldDialog::addLayout__()
{
    QGridLayout* glayout = new QGridLayout();
    QLabel* label1 = new QLabel("必填字段");
    QLabel* label2 = new QLabel("忽略字段");
    selectList = new BaseSelectList();
    ignoreList = new BaseSelectList();
    ignoreList->addContext("helper");
    glayout->addWidget(label1, 0, 0, 1, 1);
    glayout->addWidget(label2, 0, 1, 1, 1);
    glayout->addWidget(selectList, 1, 0, 10, 1);
    glayout->addWidget(ignoreList, 1, 1, 10, 1);
    setLayout(glayout);
}

void FieldDialog::connect__()
{

}

QStringList FieldDialog::getSelectList()
{
    return selectList->getItemList();
}

QStringList FieldDialog::getIgnoreList()
{
    return ignoreList->getItemList();
}
