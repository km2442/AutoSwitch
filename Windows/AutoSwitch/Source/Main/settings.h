#pragma once
#define SETTINGS_H

#include <ShlObj.h>
#include "Psapi.h"

#include <QDialog>
#include <QFileDevice>
#include <QSettings>
#include <QStyleFactory>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QCloseEvent>
#include <QProcess>
#include <QFileDialog>
#include <QInputDialog>

#include "Source/Main/functions.h"

#include "Source/Dialogs/passwordconfig.h"

#include "Source/Classes/database.h"

extern QString appData;
extern QSystemTrayIcon *trayIcon;

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = 0);
    static Settings *getInstance();
    void settingsSave(bool password, QString path);
    void settingsLoad();
    void changeHideDialog(bool i);
    ~Settings();

    int getCurrent_theme() const;

    int getLanguage() const;

    bool getDev_opt() const;

    bool getPlzLog() const;

    bool getSettings_changed() const;

    bool getTray_changed() const;

    bool getUser_changed() const;

    bool getFexec() const;

    bool getShowHideDialog() const;

    bool getSendStatistics() const;

    bool getMulti() const;

    bool getTraySettings_Shutdown() const;

    bool getTraySettings_Reboot() const;

    bool getTraySettings_Sleep() const;

    bool getTraySettings_Hibernate() const;

    bool getTraySettings_Logoff() const;

    bool getTraySettings_Lock() const;

    bool getTraySettings_Monitor() const;

    bool getTraySettingsTime_Shutdown() const;

    bool getTraySettingsTime_Reboot() const;

    bool getTraySettingsTime_Sleep() const;

    bool getTraySettingsTime_Hibernate() const;

    bool getTraySettingsTime_Logoff() const;

    bool getTraySettingsTime_Lock() const;

    bool getTraySettingsTime_Monitor() const;

private slots:

    void on_Settings_Theme_Choose_currentIndexChanged(int index);

    void on_Settings_DeveloperMode_currentIndexChanged(int index);

    void on_Settings_Log_currentIndexChanged(int index);

    void on_Settings_LogShow_clicked();

    void on_Settings_Tray_Switch_currentIndexChanged(int index);

    void on_Settings_Cancel_clicked();

    void on_Settings_SaveQuit_clicked();

    void updMem();

    void on_Settings_LanguageSelect_currentIndexChanged(int index);

    void on_Settings_DefaultSettings_clicked();

    void on_Settings_ExportSettings_clicked();

    void on_Settings_ImportSettings_clicked();

    void on_Settings_HideDialogWarning_currentIndexChanged(int index);

    void on_Settings_PasswordButton_clicked();

    void on_Settings_Tray_Shutdown_stateChanged(int arg1);

    void on_Settings_Tray_Reboot_stateChanged(int arg1);

    void on_Settings_Tray_Sleep_stateChanged(int arg1);

    void on_Settings_Tray_Hibernate_stateChanged(int arg1);

    void on_Settings_Tray_Logoff_stateChanged(int arg1);

    void on_Settings_Tray_Lock_stateChanged(int arg1);

    void on_Settings_Tray_Monitor_stateChanged(int arg1);

    void on_Settings_Tray_ShutdownTime_currentIndexChanged(int index);

    void on_Settings_Tray_RebootTime_currentIndexChanged(int index);

    void on_Settings_Tray_SleepTime_currentIndexChanged(int index);

    void on_Settings_Tray_HibernateTime_currentIndexChanged(int index);

    void on_Settings_Tray_MonitorTime_currentIndexChanged(int index);

    void on_Settings_Tray_LogoffTime_currentIndexChanged(int index);

    void on_Settings_Tray_LockTime_currentIndexChanged(int index);

    void on_Settings_Tray_ApplyChanges_clicked();

    void on_Settings_MultiInstance_currentIndexChanged(int index);

    void on_Settings_Statistics_currentIndexChanged(int index);

private:
    void Settings::closeEvent(QCloseEvent * event);
    Ui::Settings *ui;

    int current_theme = 0;
    int language = 0;

    bool dev_opt = 0;
    bool plzLog = 1;
    bool settings_changed = 0;
    bool tray_changed = 0;
    bool user_changed = 0;
    bool fexec;
    bool showHideDialog = 1;
    bool sendStatistics = 1;
    bool multi = 1;

    bool TraySettings_Shutdown = 1;
    bool TraySettings_Reboot = 1;
    bool TraySettings_Sleep = 1;
    bool TraySettings_Hibernate = 1;
    bool TraySettings_Logoff = 1;
    bool TraySettings_Lock = 1;
    bool TraySettings_Monitor = 1;

    bool TraySettingsTime_Shutdown = 1;
    bool TraySettingsTime_Reboot = 1;
    bool TraySettingsTime_Sleep = 1;
    bool TraySettingsTime_Hibernate = 1;
    bool TraySettingsTime_Logoff = 0;
    bool TraySettingsTime_Lock = 0;
    bool TraySettingsTime_Monitor = 1;
};
