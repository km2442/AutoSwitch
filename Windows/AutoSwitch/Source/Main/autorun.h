#pragma once
#define AUTORUN_H

#include <QDialog>
#include <QCloseEvent>
#include <QMessageBox>
#include <QDir>
#include <QSettings>
#include <QFileDialog>
#include <QSystemTrayIcon>

#include "Source/Main/structures.h"
#include "Source/Main/functions.h"

#include "Source/Dialogs/infoswitch.h"

extern QString appData;
extern QString PrgrData;
extern bool autoRunned;
extern QSystemTrayIcon *trayIcon;
extern bool preAutomat_Error;

namespace Ui {
class Autorun;
}

class Autorun : public QDialog
{
    Q_OBJECT

public:
    explicit Autorun(QWidget *parent = 0);
    void autorunSave();
    void autorunLoad(bool first);
    ~Autorun();

private slots:
    void on_Autorun_table_cellClicked(int row, int column);

    void on_AutoRun_CancelAll_clicked();

    void on_AutoRun_boxAdd_when_ChooseHour_pressed();

    void on_AutoRun_boxAdd_when_ChooseDT_pressed();

    void on_AutoRun_boxAdd_when_ChooseTR_pressed();

    void on_AutoRun_boxAdd_dodaj_clicked();

    void on_AutoRun_visibility_choose_currentIndexChanged(int index);

    void on_Autorun_SaveQuit_clicked();

    void on_Autorun_CancelChanges_clicked();

    void on_Autorun_TimeUpd_clicked();

    void on_Autorun_ImportSettings_clicked();

    void on_Autorun_ExportSettings_clicked();

    void on_AutoRun_Info_clicked();

    void on_AutoRun_Enable_clicked();

    void on_AutoRun_Disable_clicked();

    void on_AutoRun_Enable_global_clicked();

    void on_AutoRun_Disable_global_clicked();

private:
    Ui::Autorun *ui;

    bool changes = 0;
    bool autorun_hide;
    int autoMode[25];
    bool autoState[25];
    bool autoForce[25];
    bool autoDays[25][7];
    bool autoRepeat[25];
    bool autorunGlobal;

    void addAutoTask(bool first, Task task);
    void timeSync();
    short determineNumber();
    void autorunStateUpdater();
    void Autorun::closeEvent(QCloseEvent * event);
};
