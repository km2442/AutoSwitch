#pragma once
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QMessageBox>
#include <QDesktopServices>

#include "Source/Main/autorun.h"
#include "Source/Main/settings.h"

#include "Source/Dialogs/passwordconfig.h"
#include "Source/Dialogs/help.h"
#include "Source/Dialogs/hidedialog.h"
#include "Source/Dialogs/restxtview.h"
#include "Source/Dialogs/opinion.h"

#include "Source/Classes/socketcommunicator.h"

extern QString appVersion;
extern bool TAB_stanu[25];
extern QString opis_watku[25];
extern QString opis_watku_dni[25];
extern QString opis_watku_data[25];
extern QString opis_watku_godzina[25];

struct TraySubmenu
{
    QMenu *mainMenu, *subMenu;
    char mode;
    QAction *action[17], *actionSingle;
    QString actionText, iconPath;
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    //static MainWindow* getInstance() { return &mainwindowInstance; }
    void showMainWindow();
    void appendSocket(QString s);
    ~MainWindow();

private slots:

    void handleMessage(const QString &message);

    void iconActivated(QSystemTrayIcon::ActivationReason reason);

    void show_time();

    void on_Main_Task_Shutdown_clicked();

    void on_Main_Task_Reboot_clicked();

    void on_Main_Task_Sleep_clicked();

    void on_Main_Task_Hibernate_clicked();

    void on_Main_Task_LogOff_clicked();

    void on_Main_Task_Lock_clicked();

    void on_Main_Task_MonitorOff_clicked();

    void on_Main_Task_Help_clicked();

    void on_Main_Actions_ActiveTasks_clicked();

    void on_Main_Actions_Autorun_clicked();

    void on_Main_Actions_Settings_clicked();

    void on_Main_Actions_MonitorOff_clicked();

    void on_Main_Actions_Hide_clicked();

    void on_Main_Actions_About_clicked();

    void on_Main_Actions_Help_clicked();

    void on_When_AddTask_clicked();

    void on_When_Cancel_clicked();

    void on_When_TimeSyncButton_clicked();

    void on_Confirm_MenuButton_clicked();

    void on_Confirm_TaskButton_clicked();

    void on_Error_ActiveTasks_clicked();

    void on_Tasks_CancelAll_clicked();

    void on_Tasks_Table_cellClicked(int row, int column);

    void on_Tasks_Actions_MainMenu_clicked();

    void on_Tasks_Actions_MonitorOff_clicked();

    void on_Tasks_Actions_Hide_clicked();

    void on_Tasks_Actions_Autorun_clicked();

    void on_Tasks_Actions_Settings_clicked();

    void on_Tasks_Actions_About_clicked();

    void on_Tasks_Actions_Help_clicked();

    void on_About_Changelog_clicked();

    void on_About_License_clicked();

    void on_About_Opinion_clicked();

    void on_About_Donate_clicked();

    void on_About_Action_MainMenu_clicked();

    void on_About_Action_MonitorOff_clicked();

    void on_About_Action_Hide_clicked();

    void on_About_Action_ActiveTasks_clicked();

    void on_About_Action_Settings_clicked();

    void on_About_Action_Autorun_clicked();

    void on_About_Action_Help_clicked();

    void on_When_Mode_Now_clicked();

    void on_When_Mode_SecondsRB_pressed();

    void on_When_Mode_MinutesRB_pressed();

    void on_When_Mode_HoursRB_pressed();

    void on_When_Mode_AtHourRB_pressed();

    void on_When_Mode_CustomRB_pressed();

    void on_When_Mode_AtDateRB_pressed();

    void on_When_ClearForm_clicked();

    void on_Error_MenuButton_clicked();

private:
    Ui::MainWindow *ui;
    //static MainWindow mainwindowInstance;
    QSystemTrayIcon *trayIconn;
    void tray();
    void trayBuilder(TraySubmenu tsm, bool enableSubmenu);
    void MainWindow::closeEvent(QCloseEvent * event);
    void MainWindow::ukrywanie(bool u);
    void timeUpd();
    void isUserRegistered();
    void setAlignedIcons();
    bool taskState();

    void openHelpDialog();
    void openInfoSwitchDialog();
    void openSettingsDialog();
    void openAutorunDialog();
    void hideApp();
    void setVisibility(bool v);
    void setConfirmationIcon(char mode);
    void resetWhenPage();
    void whenSyncTime();

    char choosenMode = '\0';

    //Menu główne
    QMenu *trayIconMenu;

    QAction *ShowAction;
    QAction *WyjdzAction;
    //

    // Inicjacja wszystkich submenu
    QMenu *subMenu_Shutdown;
    QMenu *subMenu_Reboot;
    QMenu *subMenu_Sleep;
    QMenu *subMenu_Hibernate;
    QMenu *subMenu_Logoff;
    QMenu *subMenu_Lock;
    QMenu *subMenu_MonitorOff;
    //

    //Submenu QActions declaration
    QAction *ShutdownAction;
    QAction *MShutdown[16];

    QAction *RebootAction;
    QAction *MReboot[16];

    QAction *SleepAction;
    QAction *MSleep[16];

    QAction *HibernateAction;
    QAction *MHibernate[16];

    QAction *LogoffAction;
    QAction *MLogoff[16];

    QAction *LockAction;
    QAction *MLock[16];

    QAction *MonitoroffAction;
    QAction *MMonitoroff[16];
};
