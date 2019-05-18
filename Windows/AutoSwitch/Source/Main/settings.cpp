#include "settings.h"
#include "ui_settings.h"

Settings Settings::settingsInstance;

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    if (QFile(appData + "/config/settings.ini").exists() == false)
    {
        QMessageBox::information(this, tr("Brak pliku konfiguracyjnego!"), tr("Plik konfiguracyjny \"settings.ini\" nie istnieje! Program utworzy nowy plik z domyślnymi ustawieniami"));

        if (QDir(appData).exists() == false) QDir().mkdir(appData);
        if (QDir(appData + "/config").exists() == false) QDir().mkdir(appData + "/config");

        QFile::copy(":/ConfigurationFiles/resources/ConfigurationFiles/settings.ini", appData + "/config/settings.ini");
        QFileDevice::Permissions p = QFile(appData + "/config/settings.ini").permissions();
        if (!(p & QFileDevice::WriteOwner)) QFile(appData + "/config/settings.ini").setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner);
    }

    ui->setupUi(this);

    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()), this, SLOT(updMem()));
    timer->start(1000);

    ui->retranslateUi(this);

    fexec = 1;
    settingsLoad();
    fexec = 0;
}

Settings::~Settings()
{
    delete ui;
}

void Settings::closeEvent(QCloseEvent * event)
{
    if(settings_changed || tray_changed)
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("Uwaga!"), tr("Czy chcesz zapisać te ustawienia?"));
        if (reply == QMessageBox::Yes)
        {
            settingsSave(0, "");
        }
        else settingsLoad();
    }
    event->accept();
}

void Settings::settingsLoad()
{
    QSettings setting(appData + "/config/settings.ini", QSettings::IniFormat);
    setting.beginGroup("Settings");
    ui->Settings_Theme_Choose->setCurrentIndex(setting.value("WindowStyle").toInt());
    ui->Settings_LanguageSelect->setCurrentIndex(setting.value("Language").toInt() - 1);
    ui->Settings_Log->setCurrentIndex(setting.value("LogActions").toInt());
    ui->Settings_Statistics->setCurrentIndex(setting.value("SendStatistics").toInt());
    ui->Settings_MultiInstance->setCurrentIndex(setting.value("AllowMultiInstance").toInt());
    ui->Settings_DeveloperMode->setCurrentIndex(setting.value("DeveloperOptions").toInt());
    ui->Settings_HideDialogWarning->setCurrentIndex(setting.value("HideDialogWarning").toInt());
    setting.endGroup();

    setting.beginGroup("TraySettings");
    ui->Settings_Tray_Switch->setCurrentIndex(setting.value("TrayActive").toInt());
    if (setting.value("ShutdownActive").toBool()) ui->Settings_Tray_Shutdown->setChecked(2);
    else ui->Settings_Tray_Shutdown->setChecked(0);
    if (setting.value("RebootActive").toBool()) ui->Settings_Tray_Reboot->setChecked(2);
    else ui->Settings_Tray_Reboot->setChecked(0);
    if (setting.value("SleepActive").toBool()) ui->Settings_Tray_Sleep->setChecked(2);
    else ui->Settings_Tray_Sleep->setChecked(0);
    if (setting.value("HibernateActive").toBool()) ui->Settings_Tray_Hibernate->setChecked(2);
    else ui->Settings_Tray_Hibernate->setChecked(0);
    if (setting.value("LogoffActive").toBool()) ui->Settings_Tray_Logoff->setChecked(2);
    else ui->Settings_Tray_Logoff->setChecked(0);
    if (setting.value("LockActive").toBool()) ui->Settings_Tray_Lock->setChecked(2);
    else ui->Settings_Tray_Lock->setChecked(0);
    if (setting.value("MonitorActive").toBool()) ui->Settings_Tray_Monitor->setChecked(2);
    else ui->Settings_Tray_Monitor->setChecked(0);
    setting.endGroup();

    setting.beginGroup("TraySettingsTime");
    ui->Settings_Tray_ShutdownTime->setCurrentIndex(setting.value("ShutdownTime").toInt());
    ui->Settings_Tray_RebootTime->setCurrentIndex(setting.value("RebootTime").toInt());
    ui->Settings_Tray_SleepTime->setCurrentIndex(setting.value("SleepTime").toInt());
    ui->Settings_Tray_HibernateTime->setCurrentIndex(setting.value("HibernateTime").toInt());
    ui->Settings_Tray_LogoffTime->setCurrentIndex(setting.value("LogoffTime").toInt());
    ui->Settings_Tray_LockTime->setCurrentIndex(setting.value("LockTime").toInt());
    ui->Settings_Tray_MonitorTime->setCurrentIndex(setting.value("MonitorTime").toInt());
    setting.endGroup();

    settings_changed = 0;
    tray_changed = 0;
    user_changed = 0;
}

void Settings::settingsSave(bool p, QString path)
{
    QSettings *setting = nullptr;

    if (p == 0) setting = new QSettings(appData + "/config/settings.ini", QSettings::IniFormat);
    else setting = new QSettings(path, QSettings::IniFormat);

    setting->beginGroup("Settings");
    setting->setValue("Language", ui->Settings_LanguageSelect->currentIndex() + 1);
    setting->setValue("WindowStyle", ui->Settings_Theme_Choose->currentIndex());
    setting->setValue("LogActions", ui->Settings_Log->currentIndex());
    setting->setValue("SendStatistics", ui->Settings_Statistics->currentIndex());
    setting->setValue("AllowMultiInstance", ui->Settings_MultiInstance->currentIndex());
    setting->setValue("DeveloperOptions", ui->Settings_DeveloperMode->currentIndex());
    setting->setValue("HideDialogWarning", ui->Settings_HideDialogWarning->currentIndex());
    setting->endGroup();

    setting->beginGroup("TraySettings");
    setting->setValue("TrayActive", ui->Settings_Tray_Switch->currentIndex());
    setting->setValue("ShutdownActive", ui->Settings_Tray_Shutdown->isChecked());
    setting->setValue("RebootActive", ui->Settings_Tray_Reboot->isChecked());
    setting->setValue("SleepActive", ui->Settings_Tray_Sleep->isChecked());
    setting->setValue("HibernateActive", ui->Settings_Tray_Hibernate->isChecked());
    setting->setValue("LogoffActive", ui->Settings_Tray_Logoff->isChecked());
    setting->setValue("LockActive", ui->Settings_Tray_Lock->isChecked());
    setting->setValue("MonitorActive", ui->Settings_Tray_Monitor->isChecked());
    setting->endGroup();

    setting->beginGroup("TraySettingsTime");
    setting->setValue("ShutdownTime", ui->Settings_Tray_ShutdownTime->currentIndex());
    setting->setValue("RebootTime", ui->Settings_Tray_RebootTime->currentIndex());
    setting->setValue("SleepTime", ui->Settings_Tray_SleepTime->currentIndex());
    setting->setValue("HibernateTime", ui->Settings_Tray_HibernateTime->currentIndex());
    setting->setValue("LogoffTime", ui->Settings_Tray_LogoffTime->currentIndex());
    setting->setValue("LockTime", ui->Settings_Tray_LockTime->currentIndex());
    setting->setValue("MonitorTime", ui->Settings_Tray_MonitorTime->currentIndex());
    setting->endGroup();

    if (p)
    {
        PasswordConfig *pc = PasswordConfig::getInstance();
        QSettings pwConf("HKEY_LOCAL_MACHINE\\SOFTWARE\\AutoSwitch", QSettings::NativeFormat);
        QString pass = pwConf.value("Password").toString();

        setting->beginGroup("PasswordExport");
        setting->setValue("Password", pass);
        setting->setValue("PassRules_Run", pc->getPasswordrules_run());
        setting->setValue("PassRules_SettingEdit", pc->getPasswordrules_autorunedit());
        setting->setValue("PassRules_AutorunEdit", pc->getPasswordrules_autorunedit());
        setting->setValue("PassRules_ShowApp", pc->getPasswordrules_showmainwindow());
        setting->endGroup();
    }
    setting->sync();
    delete setting;

    if(user_changed = 1)
    {
        Database* db = Database::getInstance();
        int user = db->lookForUser();
        if (user > 0) db->updateUser(user, sendStatistics);
        else if (user == -1) db->registerUser(sendStatistics);
    }
}

void Settings::on_Settings_Theme_Choose_currentIndexChanged(int index)
{
    settings_changed = 1;
    int themechoosen = index;

    if(themechoosen == 0)
    {
        QPalette light = QStyleFactory::create("Fusion")->standardPalette();
        qApp->setPalette(light);
        qApp->setStyle("fusion");

        current_theme = 0;
    }
    if(themechoosen == 1)
    {
        qApp->setStyle("fusion");

        QPalette dark = QStyleFactory::create("Fusion")->standardPalette();
        dark.setColor(QPalette::Window, QColor("#282D31"));
        dark.setColor(QPalette::Base, QColor("#404040"));
        dark.setColor(QPalette::Disabled, QPalette::Base, QColor("#333333"));
        dark.setColor(QPalette::AlternateBase, QColor(53,53,53));
        dark.setColor(QPalette::Highlight, QColor(90,90,90));
        dark.setColor(QPalette::WindowText, QColor("#EBEBEB"));
        dark.setColor(QPalette::Disabled, QPalette::WindowText, QColor("#a1a1a1"));
        dark.setColor(QPalette::Text, QColor("#EBEBEB"));
        dark.setColor(QPalette::Disabled, QPalette::Text, QColor("#a1a1a1"));
        dark.setColor(QPalette::Button, QColor(53,53,53));
        dark.setColor(QPalette::Disabled, QPalette::Button, QColor("#1b1e21"));
        dark.setColor(QPalette::ButtonText, QColor(255,255,255));
        dark.setColor(QPalette::Disabled, QPalette::ButtonText, QColor("#a1a1a1"));
        dark.setColor(QPalette::Link, QColor(42, 130, 218));
        dark.setColor(QPalette::ToolTipText, QColor("#EBEBEB"));

        qApp->setPalette(dark);

        qApp->setStyleSheet("QToolTip { border: 1px solid palette(highlight); padding: 2px; }");

        current_theme = 1;
    }
    if(themechoosen == 2)
    {
        QPalette light = QStyleFactory::create("Fusion")->standardPalette();
        qApp->setPalette(light);
        qApp->setStyle("WindowsVista");

        current_theme = 2;
    }
}

void Settings::on_Settings_DeveloperMode_currentIndexChanged(int index)
{
    settings_changed = 1;
    if(index == 0) dev_opt = 0;
    else if(index == 1)
    {
        dev_opt = 1;
        if (ui->Settings_Tray_Switch->currentIndex() == 0) ui->Settings_Tray_Switch->setCurrentIndex(1);
    }
}

void Settings::on_Settings_MultiInstance_currentIndexChanged(int index)
{
    settings_changed = 1;
    if(index == 0) multi = 0;
    else multi = 1;
}

void Settings::on_Settings_Log_currentIndexChanged(int index)
{
    settings_changed = 1;
    if(index == 0) plzLog = 0;
    if(index == 1) plzLog = 1;
}

void Settings::on_Settings_LogShow_clicked()
{
    if (QFile(QDir::toNativeSeparators(PrgrData + "AutoSwitch Debug/AutoSwitch log.txt")).exists() == false) QMessageBox::warning(this, tr("Błąd"), tr("Plik z logami nie istnieje!"));
    else
    {
        QProcess *proc = new QProcess(nullptr);
        proc->start(QDir::toNativeSeparators("notepad.exe " + PrgrData + "AutoSwitch Debug/AutoSwitch log.txt"));
    }
}

void Settings::on_Settings_Tray_Switch_currentIndexChanged(int index)
{
    settings_changed = 1;
    if(index == 0)
    {
        trayIcon->hide();
        ui->Settings_DeveloperMode->setCurrentIndex(0);
        if (ui->Settings_TrayConfiguration->isEnabled()) ui->Settings_TrayConfiguration->setEnabled(0);
    }
    if(index == 1)
    {
        trayIcon->show();
        if (ui->Settings_TrayConfiguration->isEnabled() == 0) ui->Settings_TrayConfiguration->setEnabled(1);
    }
}

void Settings::on_Settings_Cancel_clicked()
{
    if (settings_changed  || tray_changed || user_changed)
    {
        settingsLoad();
        trayIcon->showMessage(tr("Informacja"), tr("Ustawienia przywrócone!"));
    }
    else trayIcon->showMessage(tr("Informacja"), tr("Nie wprowadzono żadnych zmian!"));
}

void Settings::on_Settings_SaveQuit_clicked()
{
    settingsSave(0, "");
    settings_changed = 0;
    Settings::close();
}
#include<Windows.h>
void Settings::updMem()
{
    PROCESS_MEMORY_COUNTERS pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
    SIZE_T physMemUsedByMe = pmc.WorkingSetSize;

    double usedmembyApp = static_cast<double>(physMemUsedByMe);

    QString resultMemoryTotal = QString::number(usedmembyApp / 1048576) + " MB";
    ui->Settings_DeveloperMode_Memory->setText(resultMemoryTotal);
}

void zmien()
{
    QTranslator tra;
    if (tra.load(":/translations/Resources/Languages/English.qm"))
    {
        qApp->installTranslator(&tra);
    }
}

void Settings::on_Settings_LanguageSelect_currentIndexChanged(int index)
{
    if (index == 0) language = 1;
    else if (index == 1) language = 2;

    if (fexec == 0)
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this,  tr("Uwaga!"), tr("Czy chcesz zrestartować teraz aplikację aby zmienić język? Bieżace ustawienia zostaną zapisane. Jeśli zaplanowałeś jakieś zadania, nie zostaną one wykonane!"));
        if (reply == QMessageBox::Yes)
        {
            settingsSave(0, "");
            restartApp();
        }
    }

    settings_changed = 1;
}

void Settings::on_Settings_DefaultSettings_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,  tr("Uwaga!"), tr("Czy jesteś pewny, że chcesz przywrócić ustawienia domyślne?"));
    if (reply == QMessageBox::Yes)
    {
        short currentLang = static_cast<short>(ui->Settings_LanguageSelect->currentIndex());
        QFile::remove(appData + "/config/settings.ini");
        QFile::copy(":/ConfigurationFiles/resources/ConfigurationFiles/settings.ini", appData + "/config/settings.ini");
        QFileDevice::Permissions p = QFile(appData + "/config/settings.ini").permissions();
        if (!(p & QFileDevice::WriteOwner)) QFile(appData + "/config/settings.ini").setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner);

        QSettings setting(appData + "/config/settings.ini", QSettings::IniFormat);
        setting.beginGroup("Settings");
        setting.setValue("Language", currentLang + 1);
        setting.endGroup();
        setting.sync();

        settingsLoad();
    }
}

void Settings::on_Settings_ImportSettings_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,  tr("Uwaga!"), tr("Czy jesteś pewny, że chcesz nadpisać bieżące ustawienia nowymi z pliku?"));
    if (reply == QMessageBox::Yes)
    {
        QString importINI = QFileDialog::getOpenFileName(
                    this,
                    tr("Wybierz folder i plik do importu"),
                    "/",
                    tr("Ustawienia konfiguracyjne (*.ini)")
                    );

        if (importINI.length() == 0) QMessageBox::warning(this, tr("Błąd!"), tr("Nie wybrano pliku. Spróbuj ponownie!"));
        else
        {
            QSettings setting(importINI, QSettings::IniFormat);
            if (setting.childGroups().contains("PasswordExport", Qt::CaseSensitive))
            {
                QMessageBox::StandardButton reply;
                reply = QMessageBox::question(this, tr("Uwaga!"), tr("W pliku znaleziono ustawienia hasła. Czy chcesz je zaimportować?"));
                if (reply == QMessageBox::Yes)
                {
                    setting.beginGroup("PasswordExport");

                    bool passGood = 0;
                    while (passGood == 0)
                    {
                        bool ok;
                        QString pass = setting.value("Password").toString();
                        QString passGet = QInputDialog::getText(this, tr("Podaj importowane hasło"), tr("Hasło:"), QLineEdit::Password, QString(), &ok);
                        passGet = qstringToSha1(passGet);

                        if (ok && passGet == pass)
                        {
                            bool helperOK = 0;
                            while (helperOK == 0)
                            {
                                PasswordConfig save;
                                if(save.saveToRegistry(pass,
                                                       setting.value("PassRules_Run").toBool(),
                                                       setting.value("PassRules_SettingEdit").toBool(),
                                                       setting.value("PassRules_AutorunEdit").toBool(),
                                                       setting.value("PassRules_ShowApp").toBool()))
                                {
                                    QFile::remove(appData + "/config/settings.ini");
                                    QFile::copy(importINI, appData + "/config/settings.ini");
                                    QFileDevice::Permissions p = QFile(appData + "/config/settings.ini").permissions();
                                    if (!(p & QFileDevice::WriteOwner)) QFile(appData + "/config/settings.ini").setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner);

                                    QSettings setting2(appData + "/config/settings.ini", QSettings::IniFormat);
                                    setting2.remove("PasswordExport");

                                    settingsLoad();
                                    settings_changed = 0;
                                    QMessageBox::information(this, tr("Sukces!"), tr("Hasło poprawne.\nUstawienia zostały zaimportowane."));
                                    helperOK = 1;
                                }
                                else
                                {
                                    QMessageBox::StandardButton reply;
                                    reply = QMessageBox::warning(this, tr("Błąd!"), tr("Nie udało się uruchomic aplikacji pomocniczej! Czy chcesz spróbować jeszcze raz?"));
                                    if (reply == QMessageBox::No)
                                    {
                                        QFile::remove(appData + "/config/settings.ini");
                                        QFile::copy(importINI, appData + "/config/settings.ini");
                                        QFileDevice::Permissions p = QFile(appData + "/config/settings.ini").permissions();
                                        if (!(p & QFileDevice::WriteOwner)) QFile(appData + "/config/settings.ini").setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner);
                                        settingsLoad();
                                        settings_changed = 0;
                                        QMessageBox::information(this, tr("Sukces!"), tr("Ustawienia zostały zaimportowane!\n\nZrezygnowano z importu ustawień hasła. Nie zostało ono zaimportowane."));
                                        helperOK = 1;
                                    }
                                }
                                save.close();
                            }
                            passGood = 1;
                        }
                        else if (ok && passGet != pass)
                        {
                            QMessageBox::StandardButton reply;
                            reply = QMessageBox::warning(this, tr("Uwaga!"), tr("Podane hasło nie jest prawidłowe! Czy chcesz spróbować jeszcze raz?"));
                            if (reply == QMessageBox::No)
                            {
                                QFile::remove(appData + "/config/settings.ini");
                                QFile::copy(importINI, appData + "/config/settings.ini");
                                QFileDevice::Permissions p = QFile(appData + "/config/settings.ini").permissions();
                                if (!(p & QFileDevice::WriteOwner)) QFile(appData + "/config/settings.ini").setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner);
                                settingsLoad();
                                settings_changed = 0;
                                QMessageBox::information(this, tr("Sukces!"), tr("Ustawienia zostały zaimportowane!\n\nZrezygnowano z importu ustawień hasła. Nie zostało ono zaimportowane."));
                                passGood = 1;
                            }
                        }
                        else
                        {
                            QFile::remove(appData + "/config/settings.ini");
                            QFile::copy(importINI, appData + "/config/settings.ini");
                            QFileDevice::Permissions p = QFile(appData + "/config/settings.ini").permissions();
                            if (!(p & QFileDevice::WriteOwner)) QFile(appData + "/config/settings.ini").setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner);
                            settingsLoad();
                            settings_changed = 0;
                            QMessageBox::information(this, tr("Sukces!"), tr("Ustawienia zostały zaimportowane!\n\nZrezygnowano z importu ustawień hasła. Nie zostało ono zaimportowane."));
                            passGood = 1;
                        }
                    }
                    setting.endGroup();
                }
                else
                {
                    QFile::remove(appData + "/config/settings.ini");
                    QFile::copy(importINI, appData + "/config/settings.ini");
                    QFileDevice::Permissions p = QFile(appData + "/config/settings.ini").permissions();
                    if (!(p & QFileDevice::WriteOwner)) QFile(appData + "/config/settings.ini").setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner);
                    settingsLoad();
                    settings_changed = 0;
                    QMessageBox::information(this, tr("Sukces!"), tr("Ustawienia zostały zaimportowane!"));
                }
            }
            else
            {
                QFile::remove(appData + "/config/settings.ini");
                QFile::copy(importINI, appData + "/config/settings.ini");
                QFileDevice::Permissions p = QFile(appData + "/config/settings.ini").permissions();
                if (!(p & QFileDevice::WriteOwner)) QFile(appData + "/config/settings.ini").setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner);
                settingsLoad();
                settings_changed = 0;
                QMessageBox::information(this, tr("Sukces!"), tr("Ustawienia zostały zaimportowane!"));
            }
        }
    }
}

void Settings::on_Settings_ExportSettings_clicked()
{
    QSettings pwConf("HKEY_LOCAL_MACHINE\\SOFTWARE\\AutoSwitch", QSettings::NativeFormat);
    if (pwConf.contains("Password"))
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("Uwaga!"), tr("Czy chcesz wyeksportować również ustawienia hasła?"));
        if (reply == QMessageBox::Yes)
        {
            QString exportINI = QFileDialog::getSaveFileName(
                        this,
                        tr("Wybierz folder i plik do eksportu"),
                        "/",
                        tr("Ustawienia konfiguracyjne (*.ini)")
                        );

            if (exportINI.length() == 0) QMessageBox::warning(this, tr("Błąd!"), tr("Nie wybrano pliku. Spróbuj ponownie!"));
            else
            {
                if (QFile(exportINI).exists()) QFile::remove(exportINI);
                settingsSave(1, exportINI);
                QMessageBox::information(this, tr("Sukces!"), tr("Ustawienia zostały wyeksportowane!"));
            }
        }
        else
        {
            QString exportINI = QFileDialog::getSaveFileName(
                        this,
                        tr("Wybierz folder i plik do eksportu"),
                        "::{20D04FE0-3AEA-1069-A2D8-08002B30309D}",
                        tr("Ustawienia konfiguracyjne (*.ini)")
                        );

            if (exportINI.length() == 0) QMessageBox::warning(this, tr("Błąd!"), tr("Nie wybrano pliku. Spróbuj ponownie!"));
            else
            {
                settingsSave(0, "");
                if (QFile(exportINI).exists()) QFile::remove(exportINI);
                QFile::copy(appData + "/config/settings.ini", exportINI);
                QMessageBox::information(this, tr("Sukces!"), tr("Ustawienia zostały wyeksportowane!"));
            }
        }
    }
    else
    {
        QString exportINI = QFileDialog::getSaveFileName(
                    this,
                    tr("Wybierz folder i plik do eksportu"),
                    "::{20D04FE0-3AEA-1069-A2D8-08002B30309D}",
                    tr("Ustawienia konfiguracyjne (*.ini)")
                    );

        if (exportINI.length() == 0) QMessageBox::warning(this, tr("Błąd!"), tr("Nie wybrano pliku. Spróbuj ponownie!"));
        else
        {
            settingsSave(0, "");
            if (QFile(exportINI).exists()) QFile::remove(exportINI);
            QFile::copy(appData + "/config/settings.ini", exportINI);
            QMessageBox::information(this, tr("Sukces!"), tr("Ustawienia zostały wyeksportowane!"));
        }
    }
}

void Settings::on_Settings_Statistics_currentIndexChanged(int index)
{
    user_changed = 1;
    if (index == 1) sendStatistics = 1;
    else sendStatistics = 0;
}

bool Settings::getTraySettingsTime_Monitor() const
{
    return TraySettingsTime_Monitor;
}

bool Settings::getTraySettingsTime_Lock() const
{
    return TraySettingsTime_Lock;
}

bool Settings::getTraySettingsTime_Logoff() const
{
    return TraySettingsTime_Logoff;
}

bool Settings::getTraySettingsTime_Hibernate() const
{
    return TraySettingsTime_Hibernate;
}

bool Settings::getTraySettingsTime_Sleep() const
{
    return TraySettingsTime_Sleep;
}

bool Settings::getTraySettingsTime_Reboot() const
{
    return TraySettingsTime_Reboot;
}

bool Settings::getTraySettingsTime_Shutdown() const
{
    return TraySettingsTime_Shutdown;
}

bool Settings::getTraySettings_Monitor() const
{
    return TraySettings_Monitor;
}

bool Settings::getTraySettings_Lock() const
{
    return TraySettings_Lock;
}

bool Settings::getTraySettings_Logoff() const
{
    return TraySettings_Logoff;
}

bool Settings::getTraySettings_Hibernate() const
{
    return TraySettings_Hibernate;
}

bool Settings::getTraySettings_Sleep() const
{
    return TraySettings_Sleep;
}

bool Settings::getTraySettings_Reboot() const
{
    return TraySettings_Reboot;
}

bool Settings::getTraySettings_Shutdown() const
{
    return TraySettings_Shutdown;
}

bool Settings::getMulti() const
{
    return multi;
}

bool Settings::getSendStatistics() const
{
    return sendStatistics;
}

bool Settings::getShowHideDialog() const
{
    return showHideDialog;
}

bool Settings::getFexec() const
{
    return fexec;
}

bool Settings::getUser_changed() const
{
    return user_changed;
}

bool Settings::getTray_changed() const
{
    return tray_changed;
}

bool Settings::getSettings_changed() const
{
    return settings_changed;
}

bool Settings::getPlzLog() const
{
    return plzLog;
}

bool Settings::getDev_opt() const
{
    return dev_opt;
}

int Settings::getLanguage() const
{
    return language;
}

int Settings::getCurrent_theme() const
{
    return current_theme;
}

void Settings::on_Settings_HideDialogWarning_currentIndexChanged(int index)
{
    settings_changed = 1;
    if (index == 0) showHideDialog = 0;
    else if (index == 1) showHideDialog = 1;
}

void Settings::changeHideDialog(bool i)
{
    ui->Settings_HideDialogWarning->setCurrentIndex(i);
    settingsSave(0, "");
    settings_changed = 0;
}

void Settings::on_Settings_PasswordButton_clicked()
{
    PasswordConfig *pc = PasswordConfig::getInstance();
    if (pc->getPassword_enable())
    {
        if (authorize())
        {
            PasswordConfig pass;
            pass.setWindowFlags(Qt::WindowTitleHint | Qt::MSWindowsFixedSizeDialogHint);
            pass.setModal(true);
            pass.exec();
        }
    }
    else
    {
        PasswordConfig pass;
        pass.setWindowFlags(Qt::WindowTitleHint | Qt::MSWindowsFixedSizeDialogHint);
        pass.setModal(true);
        pass.exec();
    }
}

void Settings::on_Settings_Tray_Shutdown_stateChanged(int arg1)
{
    if (arg1 == 0)
    {
        ui->Settings_Tray_ShutdownIcon->setEnabled(0);
        ui->Settings_Tray_ShutdownText->setEnabled(0);
        ui->Settings_Tray_ShutdownTime->setEnabled(0);
        TraySettings_Shutdown = 0;
    }
    else
    {
        ui->Settings_Tray_ShutdownIcon->setEnabled(1);
        ui->Settings_Tray_ShutdownText->setEnabled(1);
        ui->Settings_Tray_ShutdownTime->setEnabled(1);
        TraySettings_Shutdown = 1;
    }
    tray_changed = 1;
}

void Settings::on_Settings_Tray_Reboot_stateChanged(int arg1)
{
    if (arg1 == 0)
    {
        ui->Settings_Tray_RebootIcon->setEnabled(0);
        ui->Settings_Tray_RebootText->setEnabled(0);
        ui->Settings_Tray_RebootTime->setEnabled(0);
        TraySettings_Reboot = 0;
    }
    else
    {
        ui->Settings_Tray_RebootIcon->setEnabled(1);
        ui->Settings_Tray_RebootText->setEnabled(1);
        ui->Settings_Tray_RebootTime->setEnabled(1);
        TraySettings_Reboot = 1;
    }
    tray_changed = 1;
}

void Settings::on_Settings_Tray_Sleep_stateChanged(int arg1)
{
    if (arg1 == 0)
    {
        ui->Settings_Tray_SleepIcon->setEnabled(0);
        ui->Settings_Tray_SleepText->setEnabled(0);
        ui->Settings_Tray_SleepTime->setEnabled(0);
        TraySettings_Sleep = 0;
    }
    else
    {
        ui->Settings_Tray_SleepIcon->setEnabled(1);
        ui->Settings_Tray_SleepText->setEnabled(1);
        ui->Settings_Tray_SleepTime->setEnabled(1);
        TraySettings_Sleep = 1;
    }
    tray_changed = 1;
}

void Settings::on_Settings_Tray_Hibernate_stateChanged(int arg1)
{
    if (arg1 == 0)
    {
        ui->Settings_Tray_HibernateIcon->setEnabled(0);
        ui->Settings_Tray_HibernateText->setEnabled(0);
        ui->Settings_Tray_HibernateTime->setEnabled(0);
        TraySettings_Hibernate = 0;
    }
    else
    {
        ui->Settings_Tray_HibernateIcon->setEnabled(1);
        ui->Settings_Tray_HibernateText->setEnabled(1);
        ui->Settings_Tray_HibernateTime->setEnabled(1);
        TraySettings_Hibernate = 1;
    }
    tray_changed = 1;
}

void Settings::on_Settings_Tray_Logoff_stateChanged(int arg1)
{
    if (arg1 == 0)
    {
        ui->Settings_Tray_LogoffIcon->setEnabled(0);
        ui->Settings_Tray_LogoffText->setEnabled(0);
        ui->Settings_Tray_LogoffTime->setEnabled(0);
        TraySettings_Logoff = 0;
    }
    else
    {
        ui->Settings_Tray_LogoffIcon->setEnabled(1);
        ui->Settings_Tray_LogoffText->setEnabled(1);
        ui->Settings_Tray_LogoffTime->setEnabled(1);
        TraySettings_Logoff = 1;
    }
    tray_changed = 1;
}

void Settings::on_Settings_Tray_Lock_stateChanged(int arg1)
{
    if (arg1 == 0)
    {
        ui->Settings_Tray_LockIcon->setEnabled(0);
        ui->Settings_Tray_LockText->setEnabled(0);
        ui->Settings_Tray_LockTime->setEnabled(0);
        TraySettings_Lock = 0;
    }
    else
    {
        ui->Settings_Tray_LockIcon->setEnabled(1);
        ui->Settings_Tray_LockText->setEnabled(1);
        ui->Settings_Tray_LockTime->setEnabled(1);
        TraySettings_Lock = 1;
    }
    tray_changed = 1;
}

void Settings::on_Settings_Tray_Monitor_stateChanged(int arg1)
{
    if (arg1 == 0)
    {
        ui->Settings_Tray_MonitorIcon->setEnabled(0);
        ui->Settings_Tray_MonitorText->setEnabled(0);
        ui->Settings_Tray_MonitorTime->setEnabled(0);
        TraySettings_Monitor = 0;
    }
    else
    {
        ui->Settings_Tray_MonitorIcon->setEnabled(1);
        ui->Settings_Tray_MonitorText->setEnabled(1);
        ui->Settings_Tray_MonitorTime->setEnabled(1);
        TraySettings_Monitor = 1;
    }
    tray_changed = 1;
}

void Settings::on_Settings_Tray_ShutdownTime_currentIndexChanged(int index)
{
    if (index == 0) TraySettingsTime_Shutdown = 0;
    else TraySettingsTime_Shutdown = 1;
    tray_changed = 1;
}

void Settings::on_Settings_Tray_RebootTime_currentIndexChanged(int index)
{
    if (index == 0) TraySettingsTime_Reboot = 0;
    else TraySettingsTime_Reboot = 1;
    tray_changed = 1;
}

void Settings::on_Settings_Tray_SleepTime_currentIndexChanged(int index)
{
    if (index == 0) TraySettingsTime_Sleep = 0;
    else TraySettingsTime_Sleep = 1;
    tray_changed = 1;
}

void Settings::on_Settings_Tray_HibernateTime_currentIndexChanged(int index)
{
    if (index == 0) TraySettingsTime_Hibernate = 0;
    else TraySettingsTime_Hibernate = 1;
    tray_changed = 1;
}

void Settings::on_Settings_Tray_LogoffTime_currentIndexChanged(int index)
{
    if (index == 0) TraySettingsTime_Logoff = 0;
    else TraySettingsTime_Logoff = 1;
    tray_changed = 1;
}

void Settings::on_Settings_Tray_LockTime_currentIndexChanged(int index)
{
    if (index == 0) TraySettingsTime_Lock = 0;
    else TraySettingsTime_Lock = 1;
    tray_changed = 1;
}

void Settings::on_Settings_Tray_MonitorTime_currentIndexChanged(int index)
{
    if (index == 0) TraySettingsTime_Monitor = 0;
    else TraySettingsTime_Monitor = 1;
    tray_changed = 1;
}

void Settings::on_Settings_Tray_ApplyChanges_clicked()
{  
    if (tray_changed)
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("Uwaga!"), tr("Czy chcesz zapisać aktualne ustawienia i uruchomić ponownie aplikację aby zastosować zmiany? Uruchomione akcje nie zostaną wykonane!"));
        if (reply == QMessageBox::Yes)
        {
            settingsSave(0, "");
            restartApp();
        }
    }
    else trayIcon->showMessage(tr("Informacja"), tr("Nie wprowadzono żadnych zmian!"));
}
