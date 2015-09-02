#include "detailxmlwidget.h"
#include "QFile"
#include "Global.h"

DetailXmlWidget::DetailXmlWidget()
{
    glayout = new QGridLayout;
    setLayout(glayout);
}

DetailXmlWidget::~DetailXmlWidget()
{
}

void DetailXmlWidget::getXMLData(QString filename)
{
    QFile sFile(filename);
    QStringList list;
    if (filename == "" || !sFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        return;
    }
    openFile = filename;
    QDomDocument document;
    if (!document.setContent(&sFile)){
        return;
    }
    QDomElement workbook = document.documentElement();
    auto lists = workbook.childNodes();
    for (int i = 0; i < lists.size(); i++){
        QDomElement child = lists.at(i).toElement();
        if (child.tagName() == "Worksheet"){
            auto childList = child.childNodes();
            for (int j = 0; j < childList.size(); j++){
                auto table_ = childList.at(j).toElement();
                if (table_.tagName() == "Table"){
                    auto tableList = table_.childNodes();
                    int nameindex = 3;
                    int typeindex = 2;
                    int rowindex = 0;
                    for (int k = 0; k < tableList.size(); k++){
                        auto tableChild = tableList.at(k).toElement();
                        if (tableChild.tagName() == "Row"){
                            rowindex++;
                            if (rowindex == typeindex){
                                readType(tableChild);
                            }
                            if (rowindex == nameindex){
                                readHead(tableChild);
                            }
                        }
                    }
                }
            }
        }
    }
    sFile.close();
}

void DetailXmlWidget::readHead(QDomElement& node)
{
    clearVector();
    auto lists = node.childNodes();
    for (int i = 0; i < lists.size(); i++){
        auto child = lists.at(i);
        auto childList = child.childNodes();
        for (int k = 0; k < childList.size(); k++){
            auto namechild = childList.at(k).toElement();
            if (namechild.tagName() == "Data"){
                auto item = new ItemData;
                item->Checkbox = new QCheckBox;
                item->Checkbox->setText(namechild.text());
                item->Checkbox->setCheckState(Qt::CheckState::Checked);
                m_vec.push_back(item);
            }
        }
    }
    addToXmlWidget();
}

void DetailXmlWidget::readType(QDomElement& node)
{
    typeVec.clear();
    auto lists = node.childNodes();
    for (int i = 0; i < lists.size(); i++){
        auto child = lists.at(i);
        auto childList = child.childNodes();
        for (int k = 0; k < childList.size(); k++){
            auto namechild = childList.at(k).toElement();
            if (namechild.tagName() == "Data"){
                if (namechild.text().compare("int", Qt::CaseInsensitive) == 0){
                    typeVec.push_back("int");
                }
                else if (namechild.text().compare("string", Qt::CaseInsensitive) == 0){
                    typeVec.push_back("string");
                }
                else if (namechild.text().compare("helper", Qt::CaseInsensitive) == 0){
                    typeVec.push_back("string");
                }
            }
        }
    }
}

void DetailXmlWidget::clearVector()
{
    for (int i = 0; i < m_vec.size(); i++){
        auto item = m_vec.at(i);
        delete item->Checkbox;
        delete item;
    }
    m_vec.clear();
}

void DetailXmlWidget::addToXmlWidget()
{
    for (int i = 0; i < m_vec.size(); i++)
    {
        auto item = m_vec.at(i);
        glayout->addWidget(item->Checkbox, i / 3, i % 3, 1, 1);
    }
}

void DetailXmlWidget::writeToJson()
{
    QVariantMap map;
    translateTableToJson();
    for (int i = 0; i < dataVec.size(); i++){
        auto strList = dataVec.at(i);
        auto str = strList.at(0);
        QVariantMap data;
        for (int k = 0; k < m_vec.size(); k++){
            if (k >= 1){
                auto box = m_vec.at(k)->Checkbox;
                if (box->checkState() == Qt::CheckState::Checked){
                    if (strList.at(k) == ""){
                        continue;
                    }
                    if (typeVec.at(k) == "int"){
                        data.insert(box->text(), strList.at(k).toInt());
                    }
                    else if (typeVec.at(k) == "string"){
                        auto str = strList.at(k);
                        data.insert(box->text(), str);
                    }
                }
            }
        }
        if (str != ""){
            map.insert(str, data);
        }
    }
    QString path = openFile.section("/", 0, -2);
    path += "/";
    auto name = openFile.section("/", -1).section(".", 0, -2);
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
    addConsoleInfo("导出成功");
    dFile.close();
}

void DetailXmlWidget::translateTableToJson()
{
    dataVec.clear();
    QFile sFile(openFile);
    QDomElement* table = NULL;
    QStringList list;
    if (openFile == "" || !sFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        return;
    }
    QDomDocument document;
    if (!document.setContent(&sFile)){
        return;
    }
    QDomElement workbook = document.documentElement();
    auto lists = workbook.childNodes();
    for (int i = 0; i < lists.size(); i++){
        QDomElement child = lists.at(i).toElement();
        if (child.tagName() == "Worksheet"){
            auto childList = child.childNodes();
            for (int j = 0; j < childList.size(); j++){
                auto table_ = childList.at(j).toElement();
                if (table_.tagName() == "Table"){
                    table = &table_;
                    break;
                }
            }
        }
        if (table != NULL){
            break;
        }
    }
    sFile.close();
    auto tableList = table->childNodes();
    int rowindex = 0;
    for (int k = 0; k < tableList.size(); k++){
        auto tableChild = tableList.at(k).toElement();
        if (tableChild.tagName() == "Row"){
            if (rowindex >= 3){
                readRow(tableChild);
            }
            rowindex++;
        }
    }
}

void DetailXmlWidget::readRow(QDomElement& node)
{
    auto lists = node.childNodes();
    QStringList strList;
    QString str = node.firstChildElement().firstChildElement().text();
    if (str == ""){ return; }
    for (int i = 0; i < lists.size(); i++){
        auto child = lists.at(i);
        auto childList = child.childNodes();
        for (int k = 0; k < childList.size(); k++){
            auto namechild = childList.at(k).toElement();
            if (namechild.tagName() == "Data"){
                if (child.toElement().attribute("ss:Index") == ""){
                    strList.push_back(namechild.text());
                }
                else{
                    auto index = child.toElement().attribute("ss:Index").toInt();
                    auto count = strList.size();
                    for (int s = count + 1; s < index; s++){
                        strList.push_back("");
                    }
                    strList.push_back(namechild.text());
                }

            }
        }
    }
    if (strList.size() < typeVec.size()){
        for (int i = strList.size(); i < typeVec.size(); i++){
            strList.push_back("");
        }
    }
    dataVec.push_back(strList);
}
