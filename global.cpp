#include "global.h"
#include "FontChina.h"

QPlainTextEdit* logText = NULL;
QListWidget* g_listWidget = NULL;
QWidget* g_DetailXmlWidget = NULL;
QWidget* g_DetailExcelWidget = NULL;
QWidget* g_FieldDialog = NULL;
QWidget* g_FilterDialog = NULL;

void addConsoleInfo(std::string fmt, ...)
{
    char buf[fmt.length()+1] = {0};
    char* p = buf;
    std::string str;
    va_list args;
    int count = 0;
    size_t pos = 0;
    while (pos != std::string::npos){
        pos = fmt.find("%", pos);
        if (pos != std::string::npos){
            count++;
            pos += (fmt.size() + 1);
        }
    }
    if (count != 0){
        va_start(args, fmt);
        vsprintf(p, fmt.c_str(), args);
        va_end(args);
        str = buf;
        str += "\n";
    }
    else{
        str = fmt;
        str += "\n";
    }

    QTextCursor cursor = logText->textCursor();
    cursor.movePosition(QTextCursor::End);
    logText->setTextCursor(cursor);
    logText->insertPlainText(str.c_str());
    cursor.movePosition(QTextCursor::End);
    logText->setTextCursor(cursor);
    logText->ensureCursorVisible();
}
