#include "filterdialog.h"
#include "ui_filterdialog.h"
#include "qsqlmanager.h"
#include "qfiledialog.h"
#include "QFile"
#include "QTextStream"
#include "qdir.h"
#include "qsqlmanager.h"

FilterDialog::FilterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FilterDialog)
{
    ui->setupUi(this);
    QString path = QSQLManager::getInstance()->getFilterFile();
    QFile f(path);
    if(f.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream text(&f);
        filterString = text.readAll();
        ui->pathshow->setText(path.section("/", -1));
    }
    f.close();
}

FilterDialog::~FilterDialog()
{
    delete ui;
}

int FilterDialog::getMaxInt()
{
    return ui->intMax->text().toInt();
}

int FilterDialog::getMinInt()
{
    return ui->intMin->text().toInt();
}

int FilterDialog::getMaxIntLength()
{
    return ui->intMax->text().length();
}

int FilterDialog::getMinIntLength()
{
    return ui->intMin->text().length();
}

bool FilterDialog::getOutputArray()
{
    return ui->outputArray->isChecked();
}

 void FilterDialog::on_pushButton_clicked()
 {
     QString filterFile = QSQLManager::getInstance()->getFilterFile();
     QString file = QFileDialog::getOpenFileName(this, "打开白名单", filterFile);
     if(file != nullptr){
        ui->pathshow->setText(file.section("/", -1));
        QSQLManager::getInstance()->setFilterFile(filterFile);
        QFile f(file);
        if(f.open(QIODevice::ReadOnly | QIODevice::Text)){
            QTextStream text(&f);
            filterString = text.readAll();
        }
        f.close();
     }
 }

 QString FilterDialog::getFilterString()
 {
    return filterString;
 }
