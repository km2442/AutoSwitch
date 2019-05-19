#include "passwordconfig.h"
#include "ui_passwordconfig.h"

#include "Source/Main/functions.h"

Q_GLOBAL_STATIC(PasswordConfig, passwordConfigInstance);

PasswordConfig::PasswordConfig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PasswordConfig)
{
    ui->setupUi(this);
    loadFromReg();
    ui->PasswordConfig_Security->setCurrentIndex(password_enable);
    ui->PasswordConfig_Rules_Run->setCurrentIndex(passwordrules_run);
    ui->PasswordConfig_Rules_Settings->setCurrentIndex(passwordrules_settingsedit);
    ui->PasswordConfig_Rules_Autorun->setCurrentIndex(passwordrules_autorunedit);
    ui->PasswordConfig_Rules_ShowApp->setCurrentIndex(passwordrules_showmainwindow);

    if (password_enable) ui->PasswordConfig_SettingsCover->hide();

    pwSettings_changed = 0;
}

PasswordConfig::~PasswordConfig()
{
    delete ui;
}

void PasswordConfig::on_PasswordConfig_SaveQuit_clicked()
{
    QSettings pwConf("HKEY_LOCAL_MACHINE\\SOFTWARE\\AutoSwitch", QSettings::NativeFormat);
    QString pass;

    if (ui->PasswordConfig_Security->currentIndex() == 1)
    {
        if (pwConf.contains("Password")) pass = pwConf.value("Password").toString();

        if (pwSettings_changed || ui->PasswordConfig_WantNew->isChecked())
        {
            if (ui->PasswordConfig_WantNew->isChecked() && isNewPassGood) pass = qstringToSha1(ui->PasswordConfig_PassNew->text());
            else if ((ui->PasswordConfig_WantNew->isChecked() && isNewPassGood == 0) ||
                     (pwConf.contains("Password") == 0 && isNewPassGood == 0))
            {
                QMessageBox::warning(this, tr("Błąd!"), tr("Nie wprowadzono nowego hasła, lub nie jest ono poprawne!"));
            }
            if ((ui->PasswordConfig_WantNew->isChecked() && isNewPassGood) ||
                    (pass != "" && pwSettings_changed))
            {
                if (saveToRegistry(pass,
                                   ui->PasswordConfig_Rules_Run->currentIndex(),
                                   ui->PasswordConfig_Rules_Settings->currentIndex(),
                                   ui->PasswordConfig_Rules_Autorun->currentIndex(),
                                   ui->PasswordConfig_Rules_ShowApp->currentIndex()))
                {
                    password_enable = 1;
                    passwordrules_run = ui->PasswordConfig_Rules_Run->currentIndex();
                    passwordrules_settingsedit = ui->PasswordConfig_Rules_Settings->currentIndex();
                    passwordrules_autorunedit = ui->PasswordConfig_Rules_Autorun->currentIndex();
                    passwordrules_showmainwindow = ui->PasswordConfig_Rules_ShowApp->currentIndex();
                    close();
                }
                else QMessageBox::critical(this, tr("Błąd!"), tr("Nie udało się uruchomic aplikacji pomocniczej!"));
            }
        }
        else close();
    }
    else
    {
        if (pwConf.contains("PasswordSecurity"))
        {
            if (cleanPassConf())
            {
                password_enable = 0;
                passwordrules_run = 0;
                passwordrules_settingsedit = 0;
                passwordrules_autorunedit = 0;
                passwordrules_showmainwindow = 0;
                PasswordConfig::close();
            }
            else QMessageBox::critical(this, tr("Błąd!"), tr("Nie udało się uruchomic aplikacji pomocniczej!"));
        }
        PasswordConfig::close();
    }
}

void PasswordConfig::on_PasswordConfig_CancelChanges_clicked()
{
    if (pwSettings_changed || pwSettings_newPass)
    {
        QSettings pwConf("HKEY_LOCAL_MACHINE\\SOFTWARE\\AutoSwitch", QSettings::NativeFormat);

        if (pwConf.contains("Password"))
        {
            loadFromReg();
            ui->PasswordConfig_Security->setCurrentIndex(password_enable);
            ui->PasswordConfig_Rules_Run->setCurrentIndex(passwordrules_run);
            ui->PasswordConfig_Rules_Settings->setCurrentIndex(passwordrules_settingsedit);
            ui->PasswordConfig_Rules_Autorun->setCurrentIndex(passwordrules_autorunedit);
            ui->PasswordConfig_Rules_ShowApp->setCurrentIndex(passwordrules_showmainwindow);
        }
        else
        {
            ui->PasswordConfig_Security->setCurrentIndex(0);
            ui->PasswordConfig_Rules_Run->setCurrentIndex(0);
            ui->PasswordConfig_Rules_Settings->setCurrentIndex(0);
            ui->PasswordConfig_Rules_Autorun->setCurrentIndex(0);
            ui->PasswordConfig_Rules_ShowApp->setCurrentIndex(0);
        }

        ui->PasswordConfig_NewPassCover->show();
        ui->PasswordConfig_WantNew->setChecked(0);
        ui->PasswordConfig_PassNew->setText("");
        ui->PasswordConfig_PassNew2->setText("");
        ui->PasswordConfig_WarnIcon->setEnabled(0);
        ui->PasswordConfig_WarnText->setText("");
        isNewPassGood = 0;
        pwSettings_changed = 0;
        trayIcon->showMessage(tr("Informacja"), tr("Ustawienia przywrócone!"));
    }
    else trayIcon->showMessage(tr("Informacja"), tr("Nie wprowadzono żadnych zmian!"));
}

void PasswordConfig::on_PasswordConfig_PassNew_editingFinished()
{
    if(ui->PasswordConfig_PassNew->text().length() > 0)
    {
        ui->label_7->setEnabled(1);
        ui->PasswordConfig_PassNew2->setReadOnly(0);
    }
    else if(ui->PasswordConfig_PassNew->text().length() == 0)
    {
        if(ui->label_7->isEnabled()) ui->label_7->setEnabled(0);
        if(ui->PasswordConfig_PassNew2->isReadOnly() == 0)
        {
            ui->PasswordConfig_PassNew2->setReadOnly(1);
            ui->PasswordConfig_PassNew2->setText("");
        }
    }

    if (ui->PasswordConfig_PassNew2->text().length() != 0)
    {
        comparePass();
        if (isNewPassGood == 0)
        {
            ui->PasswordConfig_WarnIcon->setEnabled(1);
            ui->PasswordConfig_WarnText->setText(tr("Podane hasła nie są identyczne!"));
        }
        else
        {
            ui->PasswordConfig_WarnIcon->setEnabled(0);
            ui->PasswordConfig_WarnText->setText("");
        }
    }
}

void PasswordConfig::on_PasswordConfig_PassNew2_editingFinished()
{
    comparePass();
    if (isNewPassGood == 0)
    {
        ui->PasswordConfig_WarnIcon->setEnabled(1);
        ui->PasswordConfig_WarnText->setText(tr("Podane hasła nie są identyczne!"));
    }
    else
    {
        ui->PasswordConfig_WarnIcon->setEnabled(0);
        ui->PasswordConfig_WarnText->setText("");
    }
}

void PasswordConfig::comparePass()
{
    if (ui->PasswordConfig_PassNew->text() == "" || ui->PasswordConfig_PassNew2->text() == "") isNewPassGood = 0;
    else
    {
        if (ui->PasswordConfig_PassNew->text() == ui->PasswordConfig_PassNew2->text()) isNewPassGood = 1;
        else isNewPassGood = 0;
    }
}

bool PasswordConfig::getPasswordrules_showmainwindow() const
{
    return passwordrules_showmainwindow;
}

bool PasswordConfig::getPasswordrules_autorunedit() const
{
    return passwordrules_autorunedit;
}

bool PasswordConfig::getPasswordrules_settingsedit() const
{
    return passwordrules_settingsedit;
}

bool PasswordConfig::getPasswordrules_run() const
{
    return passwordrules_run;
}

bool PasswordConfig::getPassword_enable() const
{
    return password_enable;
}

void PasswordConfig::loadFromReg()
{
    QSettings pwConf("HKEY_LOCAL_MACHINE\\SOFTWARE\\AutoSwitch", QSettings::NativeFormat);
    if (pwConf.contains("PasswordSecurity"))
    {
        password_enable = 1;
        passwordrules_run = pwConf.value("Rules_Run").toBool();
        passwordrules_settingsedit = pwConf.value("Rules_SettingsEdit").toBool();
        passwordrules_autorunedit = pwConf.value("Rules_AutorunEdit").toBool();
        passwordrules_showmainwindow = pwConf.value("Rules_ShowMainWindow").toBool();
    }
    else
    {
        password_enable = 0;
        passwordrules_run = 0;
        passwordrules_settingsedit = 0;
        passwordrules_autorunedit = 0;
        passwordrules_showmainwindow = 0;
    }

    pwSettings_changed = 0;
}

PasswordConfig *PasswordConfig::getInstance()
{
    //return passwordConfigInstance;
    return nullptr;
}

bool PasswordConfig::saveToRegistry(QString pw, bool pwr_run, bool pwr_settings, bool pwr_autorun, bool pwr_show)
{
    QString param = "password enable " + pw;
    if (pwr_run) param += " 1";
    else param += " 0";
    if (pwr_settings) param += " 1";
    else param += " 0";
    if (pwr_autorun) param += " 1";
    else param += " 0";
    if (pwr_show) param += " 1";
    else param += " 0";

    if (AutoSwitchHelper(param)) return 1;
    else return 0;
}

bool PasswordConfig::cleanPassConf()
{
    if (AutoSwitchHelper("password disable")) return 1;
    else return 0;
}

void PasswordConfig::on_PasswordConfig_Security_currentIndexChanged(int index)
{
    pwSettings_changed = 1;
    QSettings pwConf("HKEY_LOCAL_MACHINE\\SOFTWARE\\AutoSwitch", QSettings::NativeFormat);

    if (pwConf.contains("Password") == 0)
    {
        if (index == 1)
        {
            ui->PasswordConfig_WantNew->setChecked(1);
            ui->PasswordConfig_SettingsCover->hide();
        }
        else
        {
            ui->PasswordConfig_WantNew->setChecked(0);
            ui->PasswordConfig_SettingsCover->show();
        }
    }
}

void PasswordConfig::on_PasswordConfig_Rules_Run_currentIndexChanged(int /*index*/)
{
    pwSettings_changed = 1;
}

void PasswordConfig::on_PasswordConfig_Rules_Settings_currentIndexChanged(int /*index*/)
{
    pwSettings_changed = 1;
}

void PasswordConfig::on_PasswordConfig_Rules_Autorun_currentIndexChanged(int /*index*/)
{
    pwSettings_changed = 1;
}

void PasswordConfig::on_PasswordConfig_Rules_ShowApp_currentIndexChanged(int /*index*/)
{
    pwSettings_changed = 1;
}

void PasswordConfig::on_PasswordConfig_WantNew_stateChanged(int arg1)
{
    if (arg1 == 2) ui->PasswordConfig_NewPassCover->hide();
    else ui->PasswordConfig_NewPassCover->show();
    pwSettings_newPass = 1;
}
