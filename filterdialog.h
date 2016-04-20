#ifndef FILTERDIALOG_H
#define FILTERDIALOG_H

#include <QDialog>

namespace Ui {
class FilterDialog;
}

class FilterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FilterDialog(QWidget *parent = 0);
    ~FilterDialog();
    int getMaxInt();
    int getMinInt();
    int getMaxIntLength();
    int getMinIntLength();
    bool getOutputArray();
    QString getFilterString();
private slots:
    void on_pushButton_clicked();
private:
    Ui::FilterDialog *ui;
    QString filterString;
};

#endif // FILTERDIALOG_H
