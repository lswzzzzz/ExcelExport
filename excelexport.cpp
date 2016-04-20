#include "excelexport.h"
#include "ui_excelexport.h"
#include "xlsxdocument.h"
#include "global.h"
#include "QFileDialog"
#include "FontChina.h"
#include "QListWidget"
#include "qsqlmanager.h"
#include "fontchina.h"

ExcelExport::ExcelExport(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ExcelExport)
{
    ui->setupUi(this);
    logText = ui->OutputView;
    listWidget = new SourceListWidget;
    ui->ListView->setWidget(listWidget);
    g_listWidget = listWidget;
    xmlwidget_ = new DetailXmlWidget;
    g_DetailXmlWidget = xmlwidget_;
    excelwidget_ = new DetailExcelWidget;
    g_DetailExcelWidget = excelwidget_;
    gridLayout = new QGridLayout;
    gridLayout->addWidget(xmlwidget_, 0, 0, 1, 1);
    gridLayout->addWidget(excelwidget_, 0, 1, 1, 1);
    ui->scrollAreaWidgetContents->setLayout(gridLayout);
    excelwidget_->setVisible(true);
    xmlwidget_->setVisible(false);
    fieldDialog = new FieldDialog(this);
    fieldDialog->setWindowTitle("字段设置");
    g_FieldDialog = fieldDialog;
    this->setWindowTitle("Excel导出");
    batchGenerateDialog = new BatchGenerateDialog;
    filterDialog = new FilterDialog;
    g_FilterDialog = filterDialog;
    //QSQLManager::getInstance()->createTable();
}

ExcelExport::~ExcelExport()
{
    delete ui;
}

void ExcelExport::on_actionOpen_triggered()
{
    QString openDir = QSQLManager::getInstance()->getOpenDir();
    QStringList openFiles = QFileDialog::getOpenFileNames(this, "打开Excel文件", openDir, tr("Excel Files (*.xml *.xlsx *.xlsm)"));
    if (openFiles.size() > 0){
        openDir = openFiles.at(0).section("/", 0, -2);
        if(!QSQLManager::getInstance()->setOpenDir(openDir)){
            addConsoleInfo("插入数据失败");
        }
        QListWidget* listWidget = g_listWidget;
        for (int i = 0; i < openFiles.size(); i++){
            QString toFile = openFiles.at(i);
            auto item = new QListWidgetItem;
            item->setData(Qt::UserRole, toFile);
            item->setText(toFile.section("/", -1).section(".", 0, -2));
            listWidget->addItem(item);
        }
        addConsoleInfo("打开%d个文件", openFiles.size());
    }
}

void ExcelExport::on_actionGenerateDir_triggered()
{
    QString generateDir = QSQLManager::getInstance()->getGenerateDir();
    QString strdir = QFileDialog::getExistingDirectory(this, "设置保存目录", generateDir);
    QSQLManager::getInstance()->setGenerateDir(strdir);
}

void ExcelExport::on_OpenButton_clicked()
{
    QString openDir = QSQLManager::getInstance()->getOpenDir();
    QStringList openFiles = QFileDialog::getOpenFileNames(this, "打开Excel文件", openDir, tr("Excel Files (*.xml *.xlsx *.xlsm)"));
    if (openFiles.size() > 0){
        openDir = openFiles.at(0).section("/", 0, -2);
        QSQLManager::getInstance()->setOpenDir(openDir);
        QListWidget* listWidget = g_listWidget;
        for (int i = 0; i < openFiles.size(); i++){
            QString toFile = openFiles.at(i);
            auto item = new QListWidgetItem;
            item->setData(Qt::UserRole, toFile);
            item->setText(toFile.section("/", -1).section(".", 0, -2));
            listWidget->addItem(item);
        }
        addConsoleInfo("打开%d个文件", openFiles.size());
    }
}

void ExcelExport::on_actionField_triggered()
{
    fieldDialog->show();
}

void ExcelExport::on_actionBatchGenerator_triggered()
{
    batchGenerateDialog->setTableList(listWidget->getTableList());
    batchGenerateDialog->show();
}

void ExcelExport::on_actionFilterSetting_triggered()
{
    filterDialog->show();
}

void ExcelExport::on_GenerationButton_clicked()
{
    if (xmlwidget_->isVisible()){
        xmlwidget_->writeToJson();
    }
    else if (excelwidget_->isVisible()){
       //excelwidget_->writeToJson();
        QStringList list = listWidget->getTableList();
        QVector<int> vec;
        for(int i=0; i<list.size(); i++){
            vec.push_back(i);
        }
        excelwidget_->BatchGenerate(vec);
    }

}

void ExcelExport::on_checkButton_clicked()
{
   if (excelwidget_->isVisible()){
      //excelwidget_->checkError();
       QStringList list = listWidget->getTableList();
       QVector<int> vec;
       for(int i=0; i<list.size(); i++){
           vec.push_back(i);
       }
       excelwidget_->BatchCheck(vec);
   }
}
