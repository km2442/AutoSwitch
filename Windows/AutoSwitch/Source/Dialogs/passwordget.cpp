#include "passwordget.h"
#include "ui_passwordget.h"

PasswordGet::PasswordGet(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PasswordGet)
{
    ui->setupUi(this);
}

PasswordGet::~PasswordGet()
{
    delete ui;
}

void PasswordGet::on_PasswordGet_Authorize_clicked()
{
    QSettings pwConf("HKEY_LOCAL_MACHINE\\SOFTWARE\\AutoSwitch", QSettings::NativeFormat);
    QString pass;
    pass = pwConf.value("Password").toString();
    if (qstringToSha1(ui->PasswordGet_Password->text()) == pass)
    {
        this->accept();
        close();
    }
    else
    {
        ui->PasswordGet_WarnIcon->setEnabled(1);
        ui->PasswordGet_WarnText->setText("Hasło nie jest poprawne!");
    }
}

void PasswordGet::on_PasswordGet_Cancel_clicked()
{
    this->reject();
    close();
}
