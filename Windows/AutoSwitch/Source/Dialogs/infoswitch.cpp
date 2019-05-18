#include "infoswitch.h"
#include "ui_infoswitch.h"

InfoSwitch::InfoSwitch(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InfoSwitch)
{
    ui->setupUi(this);
}

InfoSwitch::~InfoSwitch()
{
    delete ui;
}

void InfoSwitch::on_Changelog_close_clicked()
{
    close();
}
