#include "hidedialog.h"
#include "ui_hidedialog.h"

HideDialog::HideDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HideDialog)
{
    ui->setupUi(this);
}

HideDialog::~HideDialog()
{
    delete ui;
}

void HideDialog::on_HideDialog_Ok_clicked()
{
    if (ui->HideDialog_DoNotShow->isChecked())
    {
        Settings ust;
        ust.changeHideDialog(0);
        ust.close();
    }
    close();
}
