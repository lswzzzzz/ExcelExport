#ifndef BATCHGENERATEDIALOG_H
#define BATCHGENERATEDIALOG_H

#include <QDialog>

namespace Ui {
class BatchGenerateDialog;
}

class BatchGenerateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BatchGenerateDialog(QWidget *parent = 0);
    ~BatchGenerateDialog();
    void setTableList(QStringList list);

private slots:
    void on_GenetateButton_clicked();
    void on_checkButton_clicked();
private:
    Ui::BatchGenerateDialog *ui;
    QVector<QWidget*> vecW;
};

#endif // BATCHGENERATEDIALOG_H
