#include <ShlObj.h>

#include <QString>
#include <QSettings>

#include "Source/Main/mainwindow.h"
#include "Source/Main/functions.h"
#include "Source/Classes/database.h"
#include "Source/Dialogs/firstrun.h"

#include "QtSingleApplication"

QString appData, PrgrData, appVersion = "1.0.2.0";
bool autoRunned = 0;

int main(int argc, char *argv[])
{

    if (argc >= 2)
	{
		switch (argv[1][1])
		{
		case 's':
			{
                if (argc > 1 && argv[2][0] == '1') shutdownComputer(1);
                else shutdownComputer(0);
                exit(0);
			}
			break;
		case 'r':
			{
                if (argc > 1 && argv[2][0] == '1') rebootComputer(1);
                else rebootComputer(0);
                exit(0);
			}
			break;
        case 'u':
			{
                sleepComputer();
                exit(0);
			}
			break;
        case 'h':
            {
                hibernateComputer();
                exit(0);
            }
            break;
		case 'l':
			{
                if (argc > 1 && argv[2][0] == '1') logoffUser(1);
                else logoffUser(0);
                exit(0);
			}
			break;
        case 'b':
            {
                LockWorkStation();
                exit(0);
            }
            break;
        case 'm':
            {
                monitorOff();
                exit(0);
            }
            break;
        case 'a': autoRunned = 1; break;
        default:
            break;
		}
    }

    Database* db = Database::getInstance();
    db->getGUID();

    TCHAR szPath[MAX_PATH];
    SHGetFolderPath(nullptr, CSIDL_COMMON_APPDATA, nullptr, 0, szPath);
    PrgrData = QString::fromWCharArray(szPath) + "\\";

    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QtSingleApplication a(argc, argv);

    PasswordConfig pass;
    pass.loadFromReg();
    pass.close();

    QSettings myapp(QSettings::IniFormat, QSettings::UserScope, "AutoSwitch", "application");
    appData = QFileInfo(myapp.fileName()).absolutePath();
    QSettings setting(appData + "/config/settings.ini", QSettings::IniFormat);

    if (QDir(appData).exists() == false) QDir().mkdir(appData);
    if (QDir(appData + "/config").exists() == false) QDir().mkdir(appData + "/config");

    if (autoRunned && QFile(appData + "/config/settings.ini").exists() == false)
    {
        QFile::copy(PrgrData + "AutoSwitch/settings.ini", appData + "/config/settings.ini");
        QFileDevice::Permissions p = QFile(appData + "/config/settings.ini").permissions();
        if (!(p & QFileDevice::WriteOwner)) QFile(appData + "/config/settings.ini").setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner);
    }
    else if (QFile(appData + "/config/settings.ini").exists() == false)
    {
        QFile::copy(":/ConfigurationFiles/resources/ConfigurationFiles/settings.ini", appData + "/config/settings.ini");
        QFileDevice::Permissions p = QFile(appData + "/config/settings.ini").permissions();
        if (!(p & QFileDevice::WriteOwner)) QFile(appData + "/config/settings.ini").setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner);
    }

    if (QFile(appData + "/config/autorun.ini").exists() == false)
    {
        QFile::copy(":/ConfigurationFiles/resources/ConfigurationFiles/autorun.ini", appData + "/config/autorun.ini");
        QFileDevice::Permissions p = QFile(appData + "/config/autorun.ini").permissions();
        if (!(p & QFileDevice::WriteOwner)) QFile(appData + "/config/autorun.ini").setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner);
    }

    setting.beginGroup("Settings");
    if (setting.value("Language").toInt() == 0)
    {
        FirstRun first;
        first.setWindowFlags(Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint);
        first.setModal(true);
        first.exec();
    }

    QTranslator tra;
    if (setting.value("Language").toInt() == 2)
    {
        if (tra.load(":/translations/Resources/Languages/English.qm"))
        {
            a.installTranslator(&tra);
        }
    }

    bool multi = setting.value("AllowMultiInstance").toInt();
    setting.endGroup();

    if (a.isRunning() && multi == 0)
    {
        a.sendMessage("1");
        return 1;
    }

    bool hide;
    if (autoRunned == 0)
    {
        QSettings autorun(appData + "/AutoSwitch/config/autorun.ini", QSettings::IniFormat);
        autorun.beginGroup("AutourunSettings");
        hide = autorun.value("Autorun_hide").toBool();
        autorun.endGroup();
    }
    else
    {
        QSettings autorun(PrgrData + "AutoSwitch/autorun.ini", QSettings::IniFormat);
        autorun.beginGroup("AutourunSettings");
        hide = autorun.value("Autorun_hide").toBool();
        autorun.endGroup();
    }

    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    MainWindow *w = new MainWindow;

    a.setActivationWindow(w);
    w->setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);
	QObject::connect(&a, SIGNAL(messageReceived(QString))
    , w, SLOT(handleMessage(QString)));

    if (hide && argc >= 2) w->hide();
    else w->show();

    int ret = a.exec();
    QObject::disconnect(&a, 0, w, 0);
	return ret;
}
