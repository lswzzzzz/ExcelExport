#include "detailexcelwidget.h"
#include "QtXlsx"
#include "global.h"

DetailExcelWidget::DetailExcelWidget(QWidget *parent) : QWidget(parent)
{
    hlayout = new QHBoxLayout;
    glayout = new QGridLayout;
    vlayout = new QVBoxLayout;
    xlsx = NULL;
    excel = new QAxObject("Excel.Application", 0);
    excel->dynamicCall("SetVisible (bool Visible)", "false");//不显示窗体
    excel->setProperty("DisplayAlerts", false);//不显示任何警告信息。如果为true那么在关闭是会出现类似“文件已修改，是否保存”的提示
}

void DetailExcelWidget::getXlsxData(QString filename)
{
    deleteVec();
    file_name = filename;
    delete xlsx;
    xlsx = new QXlsx::Document(filename);
    QAxObject *workbooks = excel->querySubObject("WorkBooks");//获取工作簿集合
    workbooks->dynamicCall("Open (const QString&)", QString(filename));
    QAxObject* workbook = excel->querySubObject("ActiveWorkBook");
    auto namelist = xlsx->sheetNames();
    for(int i=0; i<namelist.size(); i++){
        auto sheetname = namelist.at(i);
        Worksheet* sheet = dynamic_cast<Worksheet*>(xlsx->sheet(sheetname));
        if(checkValid(sheet)){
            getSheetData(sheet);
            auto workSheet = workbook->querySubObject("Sheets(int)", i+1);
            qAxVec.push_back(workSheet);
        }
    }
    addToExcelWidget();
}

void DetailExcelWidget::getSheetData(Worksheet* sheet)
{
    sheetName.push_back(sheet->sheetName());
    QVector<QStringList>* vec = new QVector<QStringList>;
    dataVec.push_back(vec);
    readHead(sheet);
    readType(sheet);
}

void DetailExcelWidget::readRow(Worksheet* work_sheet, int row, int colCount, int index)
{
    QStringList strList;
    QString value = work_sheet->read(row, 1).toString();  //获取单元格内容
    if (value == "")return;
    for (int j = 1; j <= colCount; j++)
    {
        QString value = work_sheet->read(row, j).toString();
        if(value.size() > 0 && value.at(0) == QChar('=')){
            auto workSheet = qAxVec.at(index);
            QAxObject *cell = workSheet->querySubObject("Cells(int,int)", row, j);
            value = cell->property("Value").toString();  //获取单元格内容
            addConsoleInfo(value.toLocal8Bit().data());
        }
        strList << value;
    }
    dataVec.at(index)->push_back(strList);
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
        else if (head.compare("string", Qt::CaseInsensitive) == 0 || head.compare("helper", Qt::CaseInsensitive) == 0){
            vec->push_back("string");
        }
        else if (head.compare("number", Qt::CaseInsensitive) == 0){
            vec->push_back("float");
        }
    }
    typeVec.push_back(vec);
}

void DetailExcelWidget::writeToJson()
{
    translateTableToJson();
    writeStart();
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
    for (int i = 0; i < dataVec.size(); i++){
        delete dataVec.at(i);
    }
    dataVec.clear();
    for (int i = 0; i < typeVec.size(); i++){
        delete typeVec.at(i);
    }
    typeVec.clear();
    sheetName.clear();
    sheetBuVec.clear();
    for (int i = 0; i < widgetVec.size(); i++){
        hlayout->removeWidget(widgetVec.at(i));
        delete widgetVec.at(i);
    }
    widgetVec.clear();
    qAxVec.clear();
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

void DetailExcelWidget::translateTableToJson()
{
    if (dataVec.size() > 0 && dataVec.at(0)->size()>0)return;
    auto namelist = xlsx->sheetNames();
    for(int i=0; i<namelist.size(); i++){
        auto sheetname = namelist.at(i);
        Worksheet* sheet = dynamic_cast<Worksheet*>(xlsx->sheet(sheetname));
        QString name = sheet->sheetName();
        int index = checkInSheetVec(name);
        if(index > -1){
            int row = sheet->dimension().lastRow();
            int col = sheet->dimension().lastColumn();
            for(int k=4; k<=row; k++){
                readRow(sheet, k, col, index);
            }
        }
    }
}

void DetailExcelWidget::writeStart()
{
    for (int i = 0; i < sheetName.size(); i++){
        QVariantMap map;
        int size = dataVec.at(i)->size();
        for (int k = 0; k < size; k++){
            auto strList = dataVec.at(i)->at(k);
            auto str = strList.at(0);
            QVariantMap data;
            for (int n = 0; n < m_vec.at(i)->size(); n++){
                if (n >= 1){
                    auto box = m_vec.at(i)->at(n)->Checkbox;
                    if (box->checkState() == Qt::CheckState::Checked){
                        if (strList.at(n) == ""){
                            continue;
                        }
                        if (typeVec.at(i)->at(n) == "int"){
                            auto str = strList.at(n);
                            data.insert(box->text(), str.toInt());
                        }
                        else if (typeVec.at(i)->at(n) == "string"){
                            auto str = strList.at(n);
                            data.insert(box->text(), str);
                        }
                        else if (typeVec.at(i)->at(n) == "float"){
                            auto str = strList.at(n);
                            data.insert(box->text(), str.toFloat());
                        }
                    }
                }
            }
            if (str != ""){
                map.insert(str, data);
            }
        }
        QString path = file_name.section("/", 0, -2);
        path += "/";
        auto name = file_name.section("/", -1).section(".", 0, -2);
        name += "_";
        name += sheetName.at(i);
        path += name;
        path += ".json";
        QFile dFile(path);
        if (!dFile.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            return;
        }
        QTextStream out(&dFile);
        QJsonDocument doc = QJsonDocument::fromVariant(map);
        out << doc.toJson();
        dFile.close();
    }
    addConsoleInfo("导出成功");
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

int DetailExcelWidget::checkInSheetVec(QString name)
{
    for (int i = 0; i < sheetName.size(); i++){
        if (name == sheetName.at(i)){
            return i;
        }
    }
    return -1;
}
