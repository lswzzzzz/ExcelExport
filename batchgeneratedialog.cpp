#include "batchgeneratedialog.h"
#include "ui_batchgeneratedialog.h"
#include "QGridLayout"
#include "QCheckBox"
#include "QLabel"
#include "detailexcelwidget.h"
#include "global.h"

BatchGenerateDialog::BatchGenerateDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BatchGenerateDialog)
{
    ui->setupUi(this);
}

BatchGenerateDialog::~BatchGenerateDialog()
{
    delete ui;
}

void BatchGenerateDialog::setTableList(QStringList list)
{
    for(int i=0; i<vecW.size(); i++){
        delete vecW.at(i);
    }
    vecW.clear();
    QGridLayout* glayout = new QGridLayout;
    QWidget* widget = new QWidget;
    for(int i=0; i<list.size(); i++){
        auto str = list.at(i);
        auto box = new QCheckBox(str);
        glayout->addWidget(box, i, 0, 1, 1);
        vecW.push_back(box);
    }
    glayout->addWidget(new QLabel, list.size(), 0, 100, 1);
    widget->setLayout(glayout);
    ui->scrollArea->setWidget(widget);
}

void BatchGenerateDialog::on_GenetateButton_clicked()
{
    QVector<int> vec;
    for(int i=0; i<vecW.size(); i++){
        QCheckBox* box = dynamic_cast<QCheckBox*>(vecW.at(i));
        if(box->isChecked()){
            vec.push_back(i);
        }
    }
    auto widget_ = dynamic_cast<DetailExcelWidget*>(g_DetailExcelWidget);
    widget_->BatchGenerate(vec);
}

void BatchGenerateDialog::on_checkButton_clicked()
{
    QVector<int> vec;
    for(int i=0; i<vecW.size(); i++){
        QCheckBox* box = dynamic_cast<QCheckBox*>(vecW.at(i));
        if(box->isChecked()){
            vec.push_back(i);
        }
    }
    auto widget_ = dynamic_cast<DetailExcelWidget*>(g_DetailExcelWidget);
    widget_->BatchCheck(vec);
}
