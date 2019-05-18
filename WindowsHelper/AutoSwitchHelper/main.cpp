#include <string>
#include <QString>
#include <QSettings>
#include <QDir>

int main(int argc, char *argv[])
{
    if(argc > 1)
    {
        if ("autorun_global" == QString::fromStdString(argv[1]))
        {
            QSettings bootUp("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);

            if ("enable" == QString::fromStdString(argv[2]))
            {
                bootUp.setValue("AutoSwitch", QDir::toNativeSeparators("\"" + QDir::currentPath() + "/AutoSwitch.exe\"" + " -a"));
            }
            else if ("disable" == QString::fromStdString(argv[2]))
            {
                bootUp.remove("AutoSwitch");
            }
            else return 1;
        }
        else if ("password" == QString::fromStdString(argv[1]))
        {
            QSettings bootUp("HKEY_LOCAL_MACHINE\\SOFTWARE\\AutoSwitch", QSettings::NativeFormat);

            if ("enable" == QString::fromStdString(argv[2]))
            {
                bootUp.setValue("PasswordSecurity", 1);
                bootUp.setValue("Password", argv[3]);
                bootUp.setValue("Rules_Run", argv[4]);
                bootUp.setValue("Rules_SettingsEdit", argv[5]);
                bootUp.setValue("Rules_AutorunEdit", argv[6]);
                bootUp.setValue("Rules_ShowMainWindow", argv[7]);
            }
            else if ("disable" == QString::fromStdString(argv[2]))
            {
                bootUp.remove("PasswordSecurity");
                bootUp.remove("Password");
                bootUp.remove("Rules_Run");
                bootUp.remove("Rules_SettingsEdit");
                bootUp.remove("Rules_AutorunEdit");
                bootUp.remove("Rules_ShowMainWindow");

                QSettings bootUp("HKEY_LOCAL_MACHINE\\SOFTWARE", QSettings::NativeFormat);
                bootUp.remove("AutoSwitch");
            }
        }
        exit(0);
    }
    return 1;
}
