#pragma once
#define OPINION_H

namespace Ui {
class Opinion;
}

#include <QDialog>
#include <QPalette>
#include <QMessageBox>

#include "Source/Classes/database.h"

class Opinion : public QDialog
{
    Q_OBJECT

public:
    explicit Opinion(QWidget *parent = 0);
    ~Opinion();

private slots:
    void on_Opinion_CancelButton_clicked();

    void on_Opinion_Text_textChanged();

    void on_Opinion_ClearButton_clicked();

    void on_Opinion_Mark_valueChanged(int value);

    void on_Opinion_Email_textChanged(const QString &arg1);

    void on_Opinion_Type_currentIndexChanged(int index);

    void on_Opinion_SendButton_clicked();

private:
    QPalette em;
    int changes = 0;
    Ui::Opinion *ui;
    void closeEvent(QCloseEvent * event);
    bool emailValidator(QString email);
    bool checkChanges();
};
