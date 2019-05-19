#pragma once
#define PASSWORDCONFIG_H

#include <QDialog>
#include <QSettings>
#include <QMessageBox>
#include <QSystemTrayIcon>

extern QSystemTrayIcon *trayIcon;

namespace Ui {
class PasswordConfig;
}

class PasswordConfig : public QDialog
{
    Q_OBJECT

public:
    void loadFromReg();
    static PasswordConfig *getInstance();
    bool saveToRegistry(QString pw, bool pwr_run, bool pwr_settings, bool pwr_autorun, bool pwr_show);
    explicit PasswordConfig(QWidget *parent = 0);
    ~PasswordConfig();

    bool getPassword_enable() const;

    bool getPasswordrules_run() const;

    bool getPasswordrules_settingsedit() const;

    bool getPasswordrules_autorunedit() const;

    bool getPasswordrules_showmainwindow() const;

private slots:
    void on_PasswordConfig_SaveQuit_clicked();

    void on_PasswordConfig_PassNew_editingFinished();

    void on_PasswordConfig_Security_currentIndexChanged(int index);

    void on_PasswordConfig_Rules_Run_currentIndexChanged(int /*index*/);

    void on_PasswordConfig_Rules_Settings_currentIndexChanged(int /*index*/);

    void on_PasswordConfig_Rules_Autorun_currentIndexChanged(int /*index*/);

    void on_PasswordConfig_Rules_ShowApp_currentIndexChanged(int /*index*/);

    void on_PasswordConfig_WantNew_stateChanged(int arg1);

    void on_PasswordConfig_PassNew2_editingFinished();

    void on_PasswordConfig_CancelChanges_clicked();

private:
    bool cleanPassConf();
    void comparePass();
    Ui::PasswordConfig *ui;

    bool password_enable = 0;
    bool passwordrules_run = 0;
    bool passwordrules_settingsedit = 0;
    bool passwordrules_autorunedit = 0;
    bool passwordrules_showmainwindow = 0;

    bool pwSettings_changed = 0;
    bool pwSettings_newPass = 0;
    bool isNewPassGood = 0;
};

