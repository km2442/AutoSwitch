#pragma once
#define FIRSTRUN_H

#include <QDialog>
#include <QTranslator>
#include <QStyleFactory>
#include <QSettings>

#include "Source/Main/functions.h"
#include "Source/Main/functions.h"

#include "Source/Classes/database.h"

extern QString appData;

namespace Ui {
class FirstRun;
}

class FirstRun : public QDialog
{
    Q_OBJECT

public:
    explicit FirstRun(QWidget *parent = 0);
    ~FirstRun();

private slots:
    void on_First_LanguageSelect_currentIndexChanged(int index);

    void on_First_NextButton_clicked();

    void on_First_Theme_Choose_currentIndexChanged(int index);

private:
    Ui::FirstRun *ui;
};

