#pragma once
#define PASSWORDGET_H

#include <QDialog>
#include <QSettings>

#include <Source/Main/functions.h>

namespace Ui {
class PasswordGet;
}

class PasswordGet : public QDialog
{
    Q_OBJECT

public:
    explicit PasswordGet(QWidget *parent = 0);
    ~PasswordGet();

private slots:
    void on_PasswordGet_Cancel_clicked();

    void on_PasswordGet_Authorize_clicked();

private:
    Ui::PasswordGet *ui;
};
