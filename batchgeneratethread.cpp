#include "batchgeneratethread.h"
#include "fielddialog.h"
#include "qsqlmanager.h"
#include "global.h"
#include "qt_windows.h"

BatchGenerateThread::BatchGenerateThread(QObject* parent)
    :QThread(parent)
{

}

void BatchGenerateThread::init()
{
    HRESULT r = OleInitialize(0);
    if (r != S_OK && r != S_FALSE)
    {qWarning("Qt:初始化Ole失败（error %x）",(unsigned int)r);}
    xlsx = new QXlsx::Document(xlsxFile);
    excel = new QAxObject("Excel.Application", 0);
    excel->dynamicCall("SetVisible (bool Visible)", "false");//不显示窗体
    excel->setProperty("DisplayAlerts", false);//不显示任何警告信息。如果为true那么在关闭是会出现类似“文件已修改，是否保存”的提示
    QAxObject *workbooks = excel->querySubObject("WorkBooks");//获取工作簿集合
    workbooks->dynamicCall("Open (const QString&)", QString(xlsxFile));
    workbook = excel->querySubObject("ActiveWorkBook");
}

void BatchGenerateThread::unload()
{
    OleUninitialize();
    delete xlsx;
    delete excel;
}

QString ToStr(int number, int size)
{
    QString str;
    str.fill('0', size);
    str.push_back(QString::number(number));
    str = str.right(size);
    return str;
}

void BatchGenerateThread::run()
{
    init();
    dealIndex = 0;
    auto namelist = xlsx->sheetNames();
    for(int i=0; i<namelist.size(); i++){
       auto sheetname = namelist.at(i);
       sheet = dynamic_cast<Worksheet*>(xlsx->sheet(sheetname));
       if(checkValid(sheet)){
           getSheetData(sheet);
           auto workSheet = workbook->querySubObject("Sheets(int)", i+1);
           auto rowCount = sheet->dimension().lastRow();
           for(int k=4;k<=rowCount;k++){
                auto strList = readRow(sheet, workSheet, k);
                if(strList.size() != 0 && strList.at(0) != ""){
                    map.insert(ToStr(k-4, 9), createARowMap(strList));
                }
           }
           auto name = xlsxFile.section("/", -1).section(".", 0, -2);
           name += "_";
           auto subname = sheetname.section("-", 0, -1);
           name += subname;
           transtoJson(name);
       }
       dealIndex++;
   } 
   unload();
   emit writeFinishSignal(xlsxFile);
}

QVariantMap BatchGenerateThread::createARowMap(QStringList strList)
{
    QVariantMap data;
    for(int i=0;i<strList.size();i++){
        auto str = strList.at(i);
        if(str == ""){
            continue;
        }
        if(typeList.at(i) == "int"){
            data.insert(nameList.at(i), str.toInt());
        }else if(typeList.at(i) == "string"){
            data.insert(nameList.at(i), str);
        }else if(typeList.at(i) == "number"){
            double rs = str.toDouble();
            data.insert(nameList.at(i), rs);
        }else if(typeList.at(i) == "int[]"){
            data.insert(nameList.at(i), getStringArray(str, StringType::INT));
        }else if(typeList.at(i) == "number[]"){
            data.insert(nameList.at(i), getStringArray(str, StringType::NUMBER));
        }else if(typeList.at(i) == "string[]"){
            data.insert(nameList.at(i), getStringArray(str, StringType::STRING));
        }else if(!ignoreColList.contains(i+1)){
            data.insert(nameList.at(i), str);
        }
    }
    return data;
}

QVariantList BatchGenerateThread::getStringArray(QString str, StringType type_)
{
    QString result = str;
    QStringList array = result.split(",");
    QVariantList list;
    for(int i=0; i<array.size(); i++){
        switch(type_){
        case INT:
            list.push_back(array.at(i).toInt());
            break;
        case NUMBER:
            list.push_back(array.at(i).toDouble());
            break;
        case STRING:
            list.push_back(array.at(i));
            break;
        }
    }
    return list;
}

 void BatchGenerateThread::setIgnoreList()
 {
     auto dialog = dynamic_cast<FieldDialog*>(g_FieldDialog);
     QStringList strlist = dialog->getIgnoreList();
     for(int i=0; i<nameList.size(); i++){
         auto name = nameList.at(i);
         for(int n=0; n<strlist.size(); n++){
             auto item = strlist.at(n);
             if(name == item){
                 ignoreColList.push_back(i+1);
                 break;
             }
         }
     }
 }

 void BatchGenerateThread::deleteVec()
 {
     ignoreColList.clear();
     nameList.clear();
     typeList.clear();
     map.clear();
 }

void BatchGenerateThread::setXlsxFile(QString file)
{
    xlsxFile = file;
}

void BatchGenerateThread::getSheetData(Worksheet * sheet)
{
    deleteVec();
    if(p_typeVec.size() != 0){
        typeList = p_typeVec.at(dealIndex);
        nameList = p_nameVec.at(dealIndex);
    }else{
        readHead(sheet);
        readType(sheet);
    }
    setIgnoreList();
}

bool BatchGenerateThread::checkValid(Worksheet* sheet)
{
    int row = sheet->dimension().lastRow();
    if(row > 3){
        return true;
    }
    return false;
}

QStringList BatchGenerateThread::readRow(Worksheet* work_sheet, QAxObject* axSheet, int row)
{
    QStringList strList;
    int colCount = work_sheet->dimension().columnCount();
    QString value = work_sheet->read(row, 1).toString();  //获取单元格内容
    if (value == "")return strList;
    for(int i=1;i<= colCount;i++){
        QString value = work_sheet->read(row, i).toString();
        if(value.size() > 1 && value.at(0) == '='){
            QAxObject* cell = axSheet->querySubObject("Cells(int,int)", row, i);
            value = cell->property("Value").toString();  //获取单元格内容
        }
        strList << value;
    }
    return strList;
}

void BatchGenerateThread::readType(Worksheet * sheet)
{
    int colCount = sheet->dimension().lastColumn();
    for(int i=1; i<= colCount; i++){
        QString head = sheet->read(2, i).toString();
        if(head.compare("int", Qt::CaseInsensitive) == 0){
            typeList.push_back("int");
        }else if(head.compare("number", Qt::CaseInsensitive) == 0){
            typeList.push_back("number");
        }else if(head.compare("helper", Qt::CaseInsensitive) == 0){
            typeList.push_back("helper");
        }else if(head.compare("string", Qt::CaseInsensitive) == 0){
            typeList.push_back("string");
        }else if(head.compare("int[]", Qt::CaseInsensitive) == 0){
            typeList.push_back("int[]");
        }else if(head.compare("number[]", Qt::CaseInsensitive) == 0){
            typeList.push_back("number[]");
        }else if(head.compare("string[]", Qt::CaseInsensitive) == 0){
            typeList.push_back("string[]");
        }else{
            typeList.push_back(head);
        }
    }
}

void BatchGenerateThread::readHead(Worksheet* sheet)
{
    int colCount = sheet->dimension().lastColumn();
    for(int i=1; i<= colCount; i++){
        QString head = sheet->read(3, i).toString();
        nameList.push_back(head);
    }
}

void BatchGenerateThread::transtoJson(QString name)
{
    QString path = QSQLManager::getInstance()->getGenerateDir();
    path += "/" + name;
    path += ".json";
    QFile dFile(path);
    if (dFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&dFile);
        QJsonDocument doc = QJsonDocument::fromVariant(map);
        out << doc.toJson();
    }
    dFile.close();
}

void BatchGenerateThread::addNameVec(QVector<QStringList> vec)
{
    p_nameVec = vec;
}

void BatchGenerateThread::addTypeVec(QVector<QVector<QString>*> vec)
{
    for(int i=0; i<vec.size(); i++){
        auto vecChild = vec.at(i);
        QStringList listChild;
        for(int n=0; n<vecChild->size(); n++){
            listChild.push_back(vecChild->at(n));
        }
        p_typeVec.push_back(listChild);
    }
}
