#ifndef QMESSAGEINPUTDIALOG_H
#define QMESSAGEINPUTDIALOG_H

#include <QDialog>

namespace Ui {
class QMessageInputDialog;
}

class QMessageInputDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QMessageInputDialog(QWidget *parent = 0);
    ~QMessageInputDialog();

private slots:
    void on_sureButton_clicked();
    void on_cancelButton_clicked();
signals:
    void messageDialogText(QString text);
private:
    Ui::QMessageInputDialog *ui;
    QString text;
};

#endif // QMESSAGEINPUTDIALOG_H
