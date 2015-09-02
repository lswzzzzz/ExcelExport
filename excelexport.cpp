#include "excelexport.h"
#include "ui_excelexport.h"
#include "xlsxdocument.h"
#include "global.h"
#include "QFileDialog"
#include "FontChina.h"
#include "QListWidget"

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
    excelwidget_->setVisible(false);
}

ExcelExport::~ExcelExport()
{
    delete ui;
}

void ExcelExport::on_actionOpen_triggered()
{

}

void ExcelExport::on_OpenButton_clicked()
{
    QStringList openFiles = QFileDialog::getOpenFileNames(this, "打开Excel文件", g_saveDir, tr("Excel Files (*.xml *.xlsx)"));
    if (openFiles.size() > 0){
        g_saveDir = openFiles.at(0).section("/", 0, -2);
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

void ExcelExport::on_GenerationButton_clicked()
{
    if (xmlwidget_->isVisible()){
        xmlwidget_->writeToJson();
    }
    else if (excelwidget_->isVisible()){
       excelwidget_->writeToJson();
    }

}
