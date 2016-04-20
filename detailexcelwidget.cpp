#include "detailexcelwidget.h"
#include "QtXlsx"
#include "global.h"
#include "fielddialog.h"
#include "QMessageBox"
#include "sourcelistwidget.h"
#include "qsqlmanager.h"

DetailExcelWidget::DetailExcelWidget(QWidget *parent) : QWidget(parent)
{
    hlayout = new QHBoxLayout;
    glayout = new QGridLayout;
    vlayout = new QVBoxLayout;
    xlsx = nullptr;
}

DetailExcelWidget::~DetailExcelWidget()
{
}

void DetailExcelWidget::getXlsxData(QString filename)
{
    deleteVec();
    file_name = filename;
    delete xlsx;
    xlsx = new QXlsx::Document(filename);
    auto namelist = xlsx->sheetNames();
    for(int i=0; i<namelist.size(); i++){
        auto sheetname = namelist.at(i);
        Worksheet* sheet = dynamic_cast<Worksheet*>(xlsx->sheet(sheetname));
        if(checkValid(sheet)){
            getSheetData(sheet);
        }
    }
    addToExcelWidget();
}

void DetailExcelWidget::getSheetData(Worksheet* sheet)
{
    sheetName.push_back(sheet->sheetName());
    readHead(sheet);
    readType(sheet);
}

void DetailExcelWidget::readHead(Worksheet * sheet)
{
    QVector<ItemData*>* vec = new QVector<ItemData*>;
    int colCount = sheet->dimension().lastColumn();
    for(int i=1; i<= colCount; i++){
        QString head = sheet->read(3, i).toString();
        if(head == "")continue;
        auto item = new ItemData;
        item->Checkbox = new QCheckBox;
        item->Checkbox->setText(head);
        item->Checkbox->setCheckState(Qt::CheckState::Checked);
        auto  dialog = dynamic_cast<FieldDialog*>(g_FieldDialog);
        auto list = dialog->getIgnoreList();
        for(int k=0; k<list.size(); k++){
            auto name = list.at(k);
            if(head == name){
                item->Checkbox->setCheckState(Qt::CheckState::Unchecked);
                break;
            }
        }
        vec->push_back(item);
    }
    m_vec.push_back(vec);
}

void DetailExcelWidget::readType(Worksheet * sheet)
{
    QVector<QString>* vec = new QVector<QString>;
    int colCount = sheet->dimension().lastColumn();
    for(int i=1; i<= colCount; i++){
        QString head = sheet->read(2, i).toString();
        if (head.compare("int", Qt::CaseInsensitive) == 0){
            vec->push_back("int");
        }
        else if (head.compare("string", Qt::CaseInsensitive) == 0){
            vec->push_back("string");
        }
        else if (head.compare("number", Qt::CaseInsensitive) == 0){
            vec->push_back("double");
        }else if(head.compare("helper", Qt::CaseInsensitive) == 0){
            vec->push_back("helper");
        }else{
            vec->push_back("other");
        }
    }
    typeVec.push_back(vec);
}

void DetailExcelWidget::writeToJson()
{
    isBatch = false;
    BatchGenerateThread* t = new BatchGenerateThread();
    connect(t, SIGNAL(writeFinishSignal(QString)), this, SLOT(writeJsonFinished(QString)));
    generateVec.push_back(t);
    t->setXlsxFile(file_name);
    t->addTypeVec(typeVec);
    QVector<QStringList> vec__;
    addConsoleInfo("%s %d", file_name.toLocal8Bit().data(), m_vec.size());
    for(int i=0; i<m_vec.size(); i++){
        auto vecChild = m_vec.at(i);
        QStringList listChild;
        for(int n=0; n<vecChild->size(); n++){
            auto item = vecChild->at(n);
            listChild.push_back(item->Checkbox->text());
        }
        vec__.push_back(listChild);
    }
    t->addNameVec(vec__);
    t->start();
}

void DetailExcelWidget::checkError()
{
    isBatch = false;
    BatchFilterThread* t = new BatchFilterThread();
    QObject::connect(t, SIGNAL(hasErrorSignal(QString)), this, SLOT(findError(QString)));
    QObject::connect(t, SIGNAL(noErrorSignal(QString)), this, SLOT(noError(QString)));
    filterVec.push_back(t);
    t->setFile(file_name);
    t->changeToFilterType(typeVec);
    QVector<QStringList> vec__;
    for(int i=0; i<m_vec.size(); i++){
        auto vecChild = m_vec.at(i);
        QStringList listChild;
        for(int n=0; n<vecChild->size(); n++){
            auto item = vecChild->at(n);
            listChild.push_back(item->Checkbox->text());
        }
        vec__.push_back(listChild);
    }
    t->addNameVec(vec__);
    t->start();
}

void DetailExcelWidget::deleteVec()
{
    for (int i = 0; i < m_vec.size(); i++){
        auto vec = m_vec.at(i);
        for (int k = 0; k < vec->size(); k++){
            auto data = vec->at(k);
            delete data->Checkbox;
            delete data;
        }
        delete vec;
    }
    m_vec.clear();
    for (int i = 0; i < buttonVec.size(); i++){
        delete buttonVec.at(i);
    }
    buttonVec.clear();
    for (int i = 0; i < typeVec.size(); i++){
        delete typeVec.at(i);
    }
    typeVec.clear();
    sheetName.clear();
    for (int i = 0; i < widgetVec.size(); i++){
        hlayout->removeWidget(widgetVec.at(i));
        delete widgetVec.at(i);
    }
    widgetVec.clear();
}

void DetailExcelWidget::deleteBatchVec()
{
    for(int i=0; i<filterVec.size(); i++){
        delete filterVec.at(i);
    }
    filterVec.clear();
    for(int i=0; i<generateVec.size(); i++){
        delete generateVec.at(i);
    }
    generateVec.clear();
    errorFileList.clear();
}

void DetailExcelWidget::addToExcelWidget()
{
    for (int i = 0; i < sheetName.size(); i++){
        QPushButton* button = new QPushButton(sheetName.at(i).toLocal8Bit().data());
        QWidget* widget_ = new QWidget;
        QGridLayout* g_layout = new QGridLayout;
        auto vec = m_vec.at(i);
        for (int k = 0; k < vec->size(); k++){
            auto data = vec->at(k);
            g_layout->addWidget(data->Checkbox, k / 3, k % 3, 1, 1);
        }
        widget_->setLayout(g_layout);
        hlayout->addWidget(button);
        buttonVec.push_back(button);
        widgetVec.push_back(widget_);
        if (i != 0){
            widget_->setVisible(false);
        }
        glayout->addWidget(widget_, 0, i, 1, 1);
        connect(button, &QPushButton::clicked, [=]{
            for (int k = 0; k < widgetVec.size(); k++){
                auto widget__ = widgetVec.at(k);
                widget__->setVisible(false);
            }
            widget_->setVisible(true);
        });
    }
    vlayout->addLayout(hlayout);
    vlayout->addLayout(glayout);
    setLayout(vlayout);
}

bool DetailExcelWidget::checkValid(Worksheet* sheet)
{
    int row = sheet->dimension().lastRow();
    if(row > 3){
         QString head1 = sheet->read(1, 1).toString();
         QString head2 = sheet->read(2, 1).toString();
         QString head3 = sheet->read(3, 1).toString();
         if (head1.compare("ID", Qt::CaseInsensitive) == 0 && head2.compare("int", Qt::CaseInsensitive) == 0
             && head3.compare("id", Qt::CaseInsensitive) == 0){
             return true;
         }
    }
    return false;
}

void DetailExcelWidget::BatchGenerate(QVector<int> vec)
{
    BatchTotal = vec.size();
    curCount = 0;
    isBatch = true;
    deleteBatchVec();
    auto sourceListWidget = dynamic_cast<SourceListWidget*>(g_listWidget);
    for(int i=0; i<vec.size(); i++){
        int index = vec.at(i);
        auto item = sourceListWidget->item(index);
        QString name = item->data(Qt::UserRole).toString();
        BatchGenerateThread* t = new BatchGenerateThread();
        connect(t, SIGNAL(writeFinishSignal(QString)), this, SLOT(writeJsonFinished(QString)));
        t->setXlsxFile(name);
        generateVec.push_back(t);
    }
    generateVec.at(0)->start();
}

void DetailExcelWidget::BatchCheck(QVector<int> vec)
{
    BatchTotal = vec.size();
    curCount = 0;
    isBatch = true;
    deleteBatchVec();
    auto sourceListWidget = dynamic_cast<SourceListWidget*>(g_listWidget);
    for(int i=0; i<vec.size(); i++){
        int index = vec.at(i);
        auto item = sourceListWidget->item(index);
        QString name = item->data(Qt::UserRole).toString();
        BatchFilterThread* t = new BatchFilterThread();
        connect(t, SIGNAL(hasErrorSignal(QString)), this, SLOT(findError(QString)));
        connect(t, SIGNAL(noErrorSignal(QString)), this, SLOT(noError(QString)));
        t->setFile(name);
        filterVec.push_back(t);
    }
    filterVec.at(0)->start();
}

void DetailExcelWidget::findError(QString filename)
{
    if(!isBatch){
        QMessageBox::warning(NULL, "发现错误", filename, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }else{
        curCount++;
        errorFileList.push_back(filename);
        if(curCount < BatchTotal){
            addConsoleInfo("当前检查进度%d/%d", curCount, BatchTotal);
            filterVec.at(curCount)->start();
        }else{
            QString str;
            for(int i=0; i<errorFileList.size(); i++){
                str += errorFileList.at(i);
                str += "\n";
            }
            addConsoleInfo("检查完成", curCount, BatchTotal);
            QMessageBox::warning(NULL, "发现错误", str, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        }
    }
}

void DetailExcelWidget::noError(QString filename)
{
    if(!isBatch){
        QMessageBox::warning(NULL, "没有发现错误", "空", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }else{
        curCount++;
        if(curCount < BatchTotal){
            addConsoleInfo("当前检查进度%d/%d", curCount, BatchTotal);
            filterVec.at(curCount)->start();
        }else{
            QString str;
            for(int i=0; i<errorFileList.size(); i++){
                str += errorFileList.at(i);
                str += "\n";
            }
            if(errorFileList.size() > 0){
                QMessageBox::warning(NULL, "发现错误", str, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
            }else{
                addConsoleInfo("检查完成", curCount, BatchTotal);
                QMessageBox::warning(NULL, "没有发现错误", "空", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
            }
        }
    }
}

void DetailExcelWidget::writeJsonFinished(QString filename)
{
    if(!isBatch){
        addConsoleInfo("导出%s成功", filename.section("/", -1).toLocal8Bit().data());
    }else{
         curCount++;
         if(curCount < BatchTotal){
             addConsoleInfo("当前生成进度%d/%d", curCount, BatchTotal);
             generateVec.at(curCount)->start();
         }else{
             addConsoleInfo("导出所有文件完成");
         }
    }

}
