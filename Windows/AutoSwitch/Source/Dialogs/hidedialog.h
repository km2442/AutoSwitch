#pragma once
#define HIDEDIALOG_H

#include <QDialog>

#include "Source/Main/settings.h"

namespace Ui {
class HideDialog;
}

class HideDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HideDialog(QWidget *parent = 0);
    ~HideDialog();

private slots:

    void on_HideDialog_Ok_clicked();

private:
    Ui::HideDialog *ui;
};

