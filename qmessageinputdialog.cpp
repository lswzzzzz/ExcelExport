#include "qmessageinputdialog.h"
#include "ui_qmessageinputdialog.h"

QMessageInputDialog::QMessageInputDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QMessageInputDialog)
{
    ui->setupUi(this);
}

QMessageInputDialog::~QMessageInputDialog()
{
    delete ui;
}

void QMessageInputDialog::on_sureButton_clicked()
{
    text = ui->lineEdit->text();
    ui->lineEdit->setText("");
    emit messageDialogText(text);
    this->hide();
}

void QMessageInputDialog::on_cancelButton_clicked()
{
    text = "";
    ui->lineEdit->setText("");
    this->hide();
}

