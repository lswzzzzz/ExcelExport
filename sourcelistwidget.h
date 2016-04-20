#ifndef SOURCELISTWIDGET_H
#define SOURCELISTWIDGET_H

#include <QWidget>
#include "QListWidget"

class SourceListWidget : public QListWidget
{
    Q_OBJECT
public:
    SourceListWidget();
    ~SourceListWidget();
    QStringList getTableList();
private slots:
    void itemDoubleClicked(QListWidgetItem * item);
protected:

};


#endif // SOURCELISTWIDGET_H
