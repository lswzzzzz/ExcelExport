#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>
#include <qplaintextedit.h>
#include <QtCore\qobject.h>
#include <QListWidget>
extern QPlainTextEdit* logText;
extern QListWidget* g_listWidget;
extern QString g_saveDir;
extern QWidget* g_DetailXmlWidget;
extern QWidget* g_DetailExcelWidget;
void addConsoleInfo(char* fmt, ...);

#endif // GLOBAL_H
