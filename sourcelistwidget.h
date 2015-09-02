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

private slots:
    void itemClicked(QListWidgetItem * item);
protected:

};


#endif // SOURCELISTWIDGET_H
