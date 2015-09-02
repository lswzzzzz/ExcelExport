#include "excelexport.h"
#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("gbk"));
    QApplication::addLibraryPath("./plugins");
    QApplication a(argc, argv);
    ExcelExport w;
    w.show();

    return a.exec();
}
