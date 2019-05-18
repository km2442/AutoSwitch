#pragma once
#define INFOSWITCH_H

#include <QDialog>

namespace Ui {
class InfoSwitch;
}

class InfoSwitch : public QDialog
{
    Q_OBJECT

public:
    explicit InfoSwitch(QWidget *parent = 0);
    ~InfoSwitch();

private slots:
    void on_Changelog_close_clicked();

private:
    Ui::InfoSwitch *ui;
};
