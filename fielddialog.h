#ifndef FIELDDIALOG_H
#define FIELDDIALOG_H

#include <QDialog>
#include <baseselectlist.h>

namespace Ui {
class FieldDialog;
}

class FieldDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FieldDialog(QWidget *parent = 0);
    ~FieldDialog();
    QStringList getSelectList();
    QStringList getIgnoreList();
protected:
    void addLayout__();
    void connect__();

private:
    Ui::FieldDialog *ui;
    BaseSelectList* selectList;
    BaseSelectList* ignoreList;
};

#endif // FIELDDIALOG_H
