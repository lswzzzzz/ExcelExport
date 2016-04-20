#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>
#include <qplaintextedit.h>
#include <QtCore\qobject.h>
#include <QListWidget>
#include <string.h>

extern QPlainTextEdit* logText;
extern QListWidget* g_listWidget;
extern QWidget* g_DetailXmlWidget;
extern QWidget* g_DetailExcelWidget;
extern QWidget* g_FieldDialog;
extern QWidget* g_FilterDialog;
void addConsoleInfo(std::string fmt, ...);

#endif // GLOBAL_H
