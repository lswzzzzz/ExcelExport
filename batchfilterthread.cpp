#include "batchfilterthread.h"
#include "global.h"
#include "fontchina.h"
#include "fielddialog.h"
#include "qdir.h"
#include "filterdialog.h"

BatchFilterThread::BatchFilterThread(QObject* parent)
    :QThread(parent)
{
}

void BatchFilterThread::run()
{
    dealIndex = 0;
    xlsx = new QXlsx::Document(m_filename);
    QFile f(QDir::currentPath() + "/" +m_filename.section("/", -1).section(".", 0, -2) + ".txt");
    if(f.open(QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream text(&f);
        getData();
        for(int n =0; n<errorList.size(); n++){
            text<<errorList.at(n);
        }
        if(errorList.size() > 0){
            emit hasErrorSignal(m_filename);
        }else{
            emit noErrorSignal(m_filename);
        }
    }
    f.close();
    delete xlsx;
}

void BatchFilterThread::setFile(QString file)
{
    m_filename = file;
}

void BatchFilterThread::getData()
{
    auto namelist = xlsx->sheetNames();
    setNoEmptyList();
    getFilterData();
    for(int i=0; i<namelist.size(); i++){
        sheetname = namelist.at(i);
        sheet = dynamic_cast<Worksheet*>(xlsx->sheet(sheetname));
        if(checkValid(sheet)){
            deleteVec();
            if(p_typeVec.size() != 0){
                typeVec = p_typeVec.at(dealIndex);
                nameList = p_nameVec.at(dealIndex);
            }else{
                readType(sheet);
                readHead(sheet);
            }

            int row = sheet->dimension().lastRow();
            int col = sheet->dimension().lastColumn();
            for(int k=4;k<=row;k++){
                readRow(sheet, k);
            }
        }else{
            QString str = sheetname+"出错,文档格式不正确\n";
            errorList << str;
        }
        dealIndex++;
    }
}

bool BatchFilterThread::checkValid(Worksheet* sheet)
{
    int row = sheet->dimension().lastRow();
    if(row > 3){
        return true;
    }
    return false;
}

void BatchFilterThread::readRow(Worksheet* work_sheet, int row)
{
    int colCount = work_sheet->dimension().lastColumn();
    QString value = work_sheet->read(row, 1).toString();  //获取单元格内容
    if (value == "")return;
    for(int i=1;i<=colCount;i++){
        QString str = work_sheet->read(row, i).toString();
        if(noEmptyColList.contains(i) && str == ""){
            QString outStr;
            outStr.sprintf("%s出错,位置在[%d,%d],错误原因是必填字段为空\n", sheetname.toLocal8Bit().data(), row, i);
            errorList << outStr;
        }else{
            switch(typeVec.at(i-1)){
            case INT_TYPE:
                if(str.length() > 1 && str.toInt() == 0 && str.at(0) != '='){
                    QString outStr;
                    outStr.sprintf("%s出错,位置在[%d,%d],错误原因是int文本包含错误字符\n", sheetname.toLocal8Bit().data(), row, i);
                    errorList<<outStr;
                }else if(str.length() > intlength || str.toInt() < minInt){
                    QString outStr;
                    outStr.sprintf("%s出错,位置在[%d,%d],错误原因是int大小错误\n", sheetname.toLocal8Bit().data(), row, i);
                    errorList<<outStr;
                }
                break;
            case STRING_TYPE:
                if(FontChina::isAscII(str.toLocal8Bit().data())){
                    findStringErrorChar(str, row, i);
                }
                break;
            case NUMBER_TYPE:
                if(str.length() > 1 && str.toDouble() == 0.f && str.at(0) != '='){
                    QString outStr;
                    outStr.sprintf("%s出错,位置在[%d,%d],错误原因是number文本包含错误字符\n", sheetname.toLocal8Bit().data(), row, i);
                    errorList<<outStr;
                }
                break;
            case STRING_INT_TYPE:
                findStringIntErrorChar(str, row, i, false);
                break;
            case STRING_NUMBER_TYPE:
                findStringNumberErrorChar(str, row, i);
                break;
            case STRING_STRING_TYPE:
                findStringStringErrorChar(str, row, i);
                break;
            case HELPER_TYPE:
                break;
            case CUSTOM_TYPE:
                break;
            }
        }
    }
}

void BatchFilterThread::readHead(Worksheet* work_sheet)
{
    int colCount = work_sheet->dimension().lastColumn();
    for(int i=1; i<=colCount; i++){
        QString head = work_sheet->read(3, i).toString();
        nameList.push_back(head);
    }
}

void BatchFilterThread::readType(Worksheet * work_sheet)
{
    int colCount = work_sheet->dimension().lastColumn();
    for(int i=1; i<=colCount; i++){
        QString head = work_sheet->read(2, i).toString();
        if(head.compare("int", Qt::CaseInsensitive) == 0){
            typeVec.push_back(INT_TYPE);
        }else if (head.compare("string", Qt::CaseInsensitive) == 0){
            typeVec.push_back(STRING_TYPE);
        }else if (head.compare("number", Qt::CaseInsensitive) == 0){
            typeVec.push_back(NUMBER_TYPE);
        }else if(head.compare("int[]", Qt::CaseInsensitive) == 0){
            typeVec.push_back(STRING_INT_TYPE);
        }else if(head.compare("number[]", Qt::CaseInsensitive) == 0){
            typeVec.push_back(STRING_NUMBER_TYPE);
        }else if(head.compare("string[]", Qt::CaseInsensitive) == 0){
            typeVec.push_back(STRING_STRING_TYPE);
        }else if(head.compare("helper", Qt::CaseInsensitive) == 0){
            typeVec.push_back(HELPER_TYPE);
        }else{
            typeVec.push_back(CUSTOM_TYPE);
        }
    }
}

void BatchFilterThread::findStringErrorChar(QString str, int row, int col)
{
    if(!str.startsWith("=")){
        if(str.startsWith("[") && str.endsWith("]")){
            findStringIntErrorChar(str, row, col, true);
        }else{
            for(int i=0; i<str.size(); i++){
                if(!filterStr.contains(str.at(i))){
                    QString sss = filterStr;
                    QString outStr;
                    outStr.sprintf("%s出错,位置在[%d,%d],错误原因是String包含错误字符\n", sheetname.toLocal8Bit().data(), row, col);
                    errorList<<outStr;
                    break;
                }
            }
        }
    }
}

void BatchFilterThread::findStringIntErrorChar(QString str, int row, int col, bool fixedLength)
{
    if(fixedLength){
        for(int i=1;i<str.length();){
           QString mid = str.mid(i, intlength);
           if(mid.toInt() == 0){
               QString outStr;
               outStr.sprintf("%s出错,位置在[%d,%d],错误原因是String数组int长度错误\n", sheetname.toLocal8Bit().data(), row, col);
               errorList<<outStr;
               break;
           }
           int pos = i+intlength;
           if(pos < str.length() - 1 && str.at(pos) != ','){
               QString outStr;
               outStr.sprintf("%s出错,位置在[%d,%d],错误原因是String数组分隔符错误\n", sheetname.toLocal8Bit().data(), row, col);
               errorList<<outStr;
               break;
           }
           i += intlength + 1;
       }
    }else{
        QString result = str;
        QStringList array = result.split(",");
        for(int i=0; i<array.size(); i++){
            QString s_str = array.at(i);
            if(s_str.toInt() == 0 && s_str.length() > 1){
                QString outStr;
                outStr.sprintf("%s出错,位置在[%d,%d],错误原因是String[Int]数组包含错误字符或超出范围\n", sheetname.toLocal8Bit().data(), row, col);
                errorList<<outStr;
                break;
            }
        }
    }
}

void BatchFilterThread::findStringNumberErrorChar(QString str, int row, int col)
{
    QString result = str;
    QStringList array = result.split(",");
    for(int i=0; i<array.size(); i++){
        QString s_str = array.at(i);
        if(s_str.toDouble() == 0.00000f && s_str.length() > 1){
            QString outStr;
            outStr.sprintf("%s出错,位置在[%d,%d],错误原因是String[Number]数组包含错误字符\n", sheetname.toLocal8Bit().data(), row, col);
            errorList<<outStr;
            break;
        }
    }
}

void BatchFilterThread::findStringStringErrorChar(QString str, int row, int col)
{
    QString result = str;
    QStringList array = result.split(",");
    for(int i=0; i<array.size(); i++){
        QString s_str = array.at(i);
        for(int k=0; k<s_str.size(); k++){
            if(!filterStr.contains(s_str.at(k))){
                QString outStr;
                outStr.sprintf("%s出错,位置在[%d,%d],错误原因是String包含错误字符\n", sheetname.toLocal8Bit().data(), row, col);
                errorList<<outStr;
                break;
            }
        }
    }
}

void BatchFilterThread::setNoEmptyList()
{
    auto dialog = dynamic_cast<FieldDialog*>(g_FieldDialog);
    QStringList strlist = dialog->getSelectList();
    for(int i=0; i<nameList.size(); i++){
        auto name = nameList.at(i);
        for(int n=0; n<strlist.size(); n++){
            auto item = strlist.at(n);
            if(name == item){
                noEmptyColList.push_back(i+1);
                break;
            }
        }
    }
}

 void BatchFilterThread::getFilterData()
 {
    auto dialog = static_cast<FilterDialog*>(g_FilterDialog);
    intlength = dialog->getMaxIntLength();
    minInt =dialog->getMinInt();
    filterStr = dialog->getFilterString();
 }

void BatchFilterThread::deleteVec()
{
    nameList.clear();
    typeVec.clear();
}

void BatchFilterThread::changeToFilterType(QVector<QVector<QString>*> vec)
{
    for(int i=0; i<vec.size(); i++){
        auto vecChild = vec.at(i);
        QVector<dataType> filter_vec;
        for(int n = 0; n<vecChild->size(); n++){
            auto str = vecChild->at(n);
            if(str.compare("int", Qt::CaseInsensitive) == 0){
                filter_vec.push_back(INT_TYPE);
            }else if (str.compare("string", Qt::CaseInsensitive) == 0){
                filter_vec.push_back(STRING_TYPE);
            }else if (str.compare("number", Qt::CaseInsensitive) == 0){
                filter_vec.push_back(NUMBER_TYPE);
            }else if(str.compare("int[]", Qt::CaseInsensitive) == 0){
                typeVec.push_back(STRING_INT_TYPE);
            }else if(str.compare("number[]", Qt::CaseInsensitive) == 0){
                typeVec.push_back(STRING_NUMBER_TYPE);
            }else if(str.compare("string[]", Qt::CaseInsensitive) == 0){
                typeVec.push_back(STRING_STRING_TYPE);
            }else if(str.compare("helper", Qt::CaseInsensitive) == 0){
                filter_vec.push_back(HELPER_TYPE);
            }else{
                filter_vec.push_back(CUSTOM_TYPE);
            }
        }
        p_typeVec.push_back(filter_vec);
    }
}

void BatchFilterThread::addNameVec(QVector<QStringList> vec)
{
    p_nameVec = vec;
}
