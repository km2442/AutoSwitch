#pragma comment(lib, "user32.lib")
#include <Windows.h>
#include <atlbase.h>
#include <thread>
#include <ShlObj.h>
#include "Psapi.h"

#include <QDateTime>
#include <QMessageBox>
#include <thread>
#include <QSystemTrayIcon>
#include <QTableWidget>
#include <QProcess>
#include <QCryptographicHash>
#include <QApplication>

#include "Source/Main/functions.h"
#include "Source/Main/settings.h"
#include <Source/Main/structures.h>

#include "Source/Classes/database.h"
#include "Source/Dialogs/passwordget.h"

extern QSystemTrayIcon *trayIcon;
extern QTableWidget *tasklist;
extern QString PrgrData;

using namespace std;

bool TAB_stanu[25];
QString opis_watku[25];
QString opis_watku_dni[25];
QString opis_watku_data[25];
QString opis_watku_godzina[25];

bool preAutomat_Error;

int leapYear(int year)
{
    return ((year % 4 == 0  &&  year % 100 != 0) || year % 400 == 0);
}

//in date, out number of week day
//0 - monday, 1 - tuesday, ... 6 - sunday
int dayOfWeek(QDate d)
{
    int dzienRoku;
    int yy, c, g;
    int result;
    //tablica z liczba dni od początku roku (nieprzestepnego) dla kolejnych miesiecy
    int nextMonth[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

    dzienRoku = d.day() + nextMonth[d.month()-1];
    if ((d.month() > 2) && (leapYear(d.year()) == 1))
        dzienRoku++;

    yy = (d.year() - 1) % 100;
    c = (d.year() - 1) - yy;
    g = yy + (yy / 4);
    result = (((((c / 100) % 4) * 5) + g) % 7);
    result += dzienRoku - 1;
    result %= 7;

    return result;
}

struct TaskTime countDate(char unit, int quantity, struct TaskTime time)
{
    QDate d = QDate::currentDate();
    time.dt.setDate(d);

    switch (unit)
    {
        case 'd': time.dt = time.dt.addDays(quantity); break;
        case 'm': time.dt = time.dt.addMonths(quantity); break;
        case 'r': time.dt = time.dt.addYears(quantity); break;
    }
    return time;
}

struct TaskTime countTime(unsigned long long quantity, char unit, struct TaskTime time)
{
    unsigned long long temp;

    time.dt = time.dt.currentDateTime();

    if (unit == 'h') temp = quantity * 3600;
    else if (unit == 'm') temp = quantity * 60;
    else temp = quantity;

    time.dt = time.dt.addSecs(temp);

    return time;
}
void trayAction(char mode, char unit, unsigned long long quantity)
{
    struct Task task;
    task.mode = mode;
    task.time = countTime(quantity, unit, task.time);
    task.force = 1;
    preAutomat(task);
}

void preAutomat(Task task)
{
    for(short i = 1; i <= 26; i++)
    {
        if(TAB_stanu[i - 1] == 0 && i != 26)
        {
            task.taskNumber = i;
            break;
        }

        if(i == 26)
        {
            preAutomat_Error = 1;
        }
    }

    if(preAutomat_Error == 0)
    {
        QString temp;
        QString n2s_temp;

        switch (task.mode)
        {
        case 'w': temp += QObject::tr("Wyłączenie Komputera"); break;
        case 'r': temp += QObject::tr("Ponowne uruchomienie Komputera"); break;
        case 'u': temp += QObject::tr("Uśpienie Komputera"); break;
        case 'h': temp += QObject::tr("Hibernacja Komputera"); break;
        case 'l': temp += QObject::tr("Wylogowanie Użytkownika"); break;
        case 'b': temp += QObject::tr("Zablokowanie Komputera"); break;
        case 'm': temp += QObject::tr("Wyłączenie Monitora"); break;
        }
        opis_watku[task.taskNumber - 1] = temp;
        temp = "";

        n2s_temp = QString::number(task.time.dt.time().hour());
        if(n2s_temp.length() == 1) n2s_temp = "0" + n2s_temp;
        temp += n2s_temp;
        temp += ":";
        n2s_temp = QString::number(task.time.dt.time().minute());
        if(n2s_temp.length() == 1) n2s_temp = "0" + n2s_temp;
        temp += n2s_temp;
        temp += ":";
        n2s_temp = QString::number(task.time.dt.time().second());
        if(n2s_temp.length() == 1) n2s_temp = "0" + n2s_temp;
        temp += n2s_temp;
        opis_watku_godzina[task.taskNumber - 1] = temp;
        temp = "";

        if(task.repeatability)
        {
            if(task.time.monday) opis_watku_dni[task.taskNumber - 1] = "1";
            if(task.time.tuesday && opis_watku_dni[task.taskNumber - 1].length() != 0) opis_watku_dni[task.taskNumber - 1] += ", 2";
            else if(task.time.tuesday && opis_watku_dni[task.taskNumber - 1].length() == 0) opis_watku_dni[task.taskNumber - 1] = "2";
            if(task.time.wednesday && opis_watku_dni[task.taskNumber - 1].length() != 0) opis_watku_dni[task.taskNumber - 1] += ", 3";
            else if(task.time.wednesday && opis_watku_dni[task.taskNumber - 1].length() == 0) opis_watku_dni[task.taskNumber - 1] = "3";
            if(task.time.thursday && opis_watku_dni[task.taskNumber - 1].length() != 0) opis_watku_dni[task.taskNumber - 1] += ", 4";
            else if(task.time.thursday && opis_watku_dni[task.taskNumber - 1].length() == 0) opis_watku_dni[task.taskNumber - 1] = "4";
            if(task.time.friday && opis_watku_dni[task.taskNumber - 1].length() != 0) opis_watku_dni[task.taskNumber - 1] += ", 5";
            else if(task.time.friday && opis_watku_dni[task.taskNumber - 1].length() == 0) opis_watku_dni[task.taskNumber - 1] = "5";
            if(task.time.saturday && opis_watku_dni[task.taskNumber - 1].length() != 0) opis_watku_dni[task.taskNumber - 1] += ", 6";
            else if(task.time.saturday && opis_watku_dni[task.taskNumber- 1].length() == 0) opis_watku_dni[task.taskNumber - 1] = "6";
            if(task.time.sunday && opis_watku_dni[task.taskNumber - 1].length() != 0) opis_watku_dni[task.taskNumber - 1] += ", 7";
            else if(task.time.sunday && opis_watku_dni[task.taskNumber - 1].length() == 0) opis_watku_dni[task.taskNumber - 1] = "7";

            addTaskDescription(task.repeatability, task.taskNumber - 1, opis_watku[task.taskNumber - 1], "", opis_watku_godzina[task.taskNumber - 1], opis_watku_dni[task.taskNumber - 1]);
        }
        else
        {
            n2s_temp = QString::number(task.time.dt.date().day());
            if(n2s_temp.length() == 1) n2s_temp = "0" + n2s_temp;
            temp += n2s_temp;
            temp += ".";
            n2s_temp = QString::number(task.time.dt.date().month());
            if(n2s_temp.length() == 1) n2s_temp = "0" + n2s_temp;
            temp += n2s_temp;
            temp += ".";
            n2s_temp = QString::number(task.time.dt.date().year());
            temp += n2s_temp;
            opis_watku_data[task.taskNumber - 1] = temp;

            addTaskDescription(task.repeatability, task.taskNumber - 1, opis_watku[task.taskNumber - 1], opis_watku_data[task.taskNumber - 1], opis_watku_godzina[task.taskNumber - 1], "");
        }
        thread wylacznik1(automat, task);
        wylacznik1.detach();
    }
}

void automat(Task task)
{
    bool zrobione = 0, error = 0, execute = 0;
    TAB_stanu[task.taskNumber - 1] = 1;

    while (zrobione == 0 && TAB_stanu[task.taskNumber - 1])
    {
        while (execute == 0 && TAB_stanu[task.taskNumber - 1])
        {
            QDateTime dt = QDateTime::currentDateTime();
            Sleep(850);
            if(task.repeatability)
            {
                if (((task.time.monday = 1 && dayOfWeek(dt.date()) == 0) ||
                     (task.time.tuesday = 1 && dayOfWeek(dt.date()) == 1) ||
                     (task.time.wednesday = 1 && dayOfWeek(dt.date()) == 2) ||
                     (task.time.thursday = 1 && dayOfWeek(dt.date()) == 3) ||
                     (task.time.friday = 1 && dayOfWeek(dt.date()) == 4) ||
                     (task.time.saturday = 1 && dayOfWeek(dt.date()) == 5) ||
                     (task.time.sunday = 1 && dayOfWeek(dt.date()) == 6)) &&
                        task.time.dt.time().secsTo(QTime::currentTime()) == 0) execute = 1;
            }
            else
            {
                if (dt.date() == QDate::currentDate() && task.time.dt.time().secsTo(QTime::currentTime()) == 0) execute = 1;
            }
        }

        if (execute)
        {
            Settings *s = Settings::getInstance();
            if(s->getDev_opt() == 0)
            {
                if (s->getPlzLog() && s->getDev_opt() == 0 && error == 0) log(task.mode);

                switch (task.mode)
                {
                case 'w': shutdownComputer(task.force); break;
                case 'r': rebootComputer(task.force); break;
                case 'u': sleepComputer(); break;
                case 'h':
                {
                    if (isHibernateAvailable()) hibernateComputer();
                    else
                    {
                        QMessageBox::warning(nullptr, QObject::tr("Błąd!"), QObject::tr("Przełączenie Komputera w stan hibernacji nie może zostać wykonane! Upewnij się, czy hibernacja jest aktywna."));
                        error = 1;
                    }
                }
                    break;
                case 'l': logoffUser(task.force); break;
                case 'b': lockComputer(); break;
                case 'm': monitorOff(); break;
                }
            }
            else if (error == 0)
            {
                QString develop;
                switch (task.mode)
                {
                case 'w': develop = QObject::tr("Wyłączenie Komputera"); break;
                case 'r': develop = QObject::tr("Restart Komputera"); break;
                case 'u': develop = QObject::tr("Uśpienie Komputera"); break;
                case 'h': develop = QObject::tr("Hibernacja Komputera"); break;
                case 'l': develop = QObject::tr("Wylogowanie Użytkownika"); break;
                case 'b': develop = QObject::tr("Zablokowanie Komputera"); break;
                case 'm': develop = QObject::tr("Wyłączenie Monitora"); break;
                }
                trayIcon->showMessage(QObject::tr("AutoSwitch TRYB TESTOWY"), develop);
            }

            if (task.repeatability == 0)
            {
                TAB_stanu[task.taskNumber - 1] = 0;
                opis_watku[task.taskNumber - 1] = "";
                opis_watku_dni[task.taskNumber - 1] = "";
                opis_watku_godzina[task.taskNumber - 1] = "";
                opis_watku_data[task.taskNumber - 1] = "";

                tasklist->item(task.taskNumber - 1, 0)->setText("");
                tasklist->item(task.taskNumber - 1, 1)->setText("");
                tasklist->item(task.taskNumber - 1, 2)->setText("");
                tasklist->item(task.taskNumber - 1, 3)->setText("");

                tasklist->viewport()->update();
                zrobione = 1;
            }
            execute = 0;
        }
    }
}

void getShutdownPriviliges()
{
    HANDLE hToken;
    TOKEN_PRIVILEGES tkp;

    OpenProcessToken(::GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY, &hToken);
    LookupPrivilegeValue(nullptr, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);

    tkp.PrivilegeCount = 1; // set 1 privilege
    tkp.Privileges[0].Attributes= SE_PRIVILEGE_ENABLED;

    //get the shutdown privilege for this process
    ::AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)nullptr, nullptr);
}

void shutdownComputer(bool force)
{
    Settings *s = Settings::getInstance();
    if (s->getDev_opt()) trayIcon->showMessage(QObject::tr("AutoSwitch TRYB TESTOWY"), QObject::tr("Wyłączenie Komputera"));
    else
    {
        if(s->getSendStatistics() == 1)
        {
            Database* db = Database::getInstance();
            db->sendStatistics("Shutdown");
        }
        getShutdownPriviliges();
        if (force) ExitWindowsEx(EWX_SHUTDOWN|EWX_FORCE, 1);
        else ExitWindowsEx(EWX_SHUTDOWN|EWX_FORCE, 0);
    }
}

void rebootComputer(bool force)
{
    Settings *s = Settings::getInstance();
    if (s->getDev_opt()) trayIcon->showMessage(QObject::tr("AutoSwitch TRYB TESTOWY"), QObject::tr("Restart Komputera"));
    else
    {
        if(s->getSendStatistics() == 1)
        {
            Database* db = Database::getInstance();
            db->sendStatistics("Reboot");
        }
        getShutdownPriviliges();
        if (force) ::ExitWindowsEx(EWX_REBOOT|EWX_FORCE, 1);
        else ::ExitWindowsEx(EWX_REBOOT|EWX_FORCE, 0);
    }
}

void sleepComputer()
{
    Settings *s = Settings::getInstance();
    if (s->getDev_opt()) trayIcon->showMessage(QObject::tr("AutoSwitch TRYB TESTOWY"), QObject::tr("Uśpienie Komputera"));
    else
    {
        if(s->getSendStatistics() == 1)
        {
            Database* db = Database::getInstance();
            db->sendStatistics("Sleep");
        }
        system("rundll32.exe powrprof.dll,SetSuspendState 0,1,0");
    }
}

void hibernateComputer()
{
    Settings *s = Settings::getInstance();
    if (s->getDev_opt()) trayIcon->showMessage(QObject::tr("AutoSwitch TRYB TESTOWY"), QObject::tr("Hibernacja Komputera"));
    else
    {
        if(s->getSendStatistics() == 1)
        {
            Database* db = Database::getInstance();
            db->sendStatistics("Hibernate");
        }
        system("shutdown -h -f");
    }
}

void logoffUser(bool force)
{
    Settings *s = Settings::getInstance();
    if (s->getDev_opt()) trayIcon->showMessage(QObject::tr("AutoSwitch TRYB TESTOWY"), QObject::tr("Wylogowanie Użytkownika"));
    else
    {
        if(s->getSendStatistics() == 1)
        {
            Database* db = Database::getInstance();
            db->sendStatistics("Logout");
        }
        if (force) system("shutdown -l -f -t 0");
        else system("shutdown -l -t 0");
    }
}

void lockComputer()
{
    Settings *s = Settings::getInstance();
    if (s->getDev_opt()) trayIcon->showMessage(QObject::tr("AutoSwitch TRYB TESTOWY"), QObject::tr("Zablokowanie Komputera"));
    else
    {
        if(s->getSendStatistics() == 1)
        {
            Database* db = Database::getInstance();
            db->sendStatistics("Lock");
        }
        LockWorkStation();
    }
}

void monitorOff()
{
    Sleep(500);
    Settings *s = Settings::getInstance();
    if (s->getDev_opt()) trayIcon->showMessage(QObject::tr("AutoSwitch TRYB TESTOWY"), QObject::tr("Wyłączenie Monitora"));
    else
    {
        if(s->getSendStatistics() == 1)
        {
            Database* db = Database::getInstance();
            db->sendStatistics("MonitorOff");
        }
        SendMessageTimeout(HWND_BROADCAST, WM_SYSCOMMAND, SC_MONITORPOWER, static_cast<LPARAM>(2), SMTO_ABORTIFHUNG, 100, nullptr);
    }

}

void restartApp()
{
    qApp->quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}

bool isHibernateAvailable()
{
    return QFile("C://hiberfil.sys").exists();
}

QString currentSystemLanguage()
{
    QLocale *loc = new QLocale;
    return loc->languageToString(loc->language());
}

QString qstringToSha1(QString s)
{
    return QString(QCryptographicHash::hash(s.toUtf8(),QCryptographicHash::Sha1).toHex());
}

bool AutoSwitchHelper(QString param)
{
    QString plik = QDir::toNativeSeparators("\"" + QDir::currentPath() + "/AutoSwitchHelper.exe\"");

    SHELLEXECUTEINFO ShExecInfo = {0};
    ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
    ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
    ShExecInfo.hwnd = nullptr;
    ShExecInfo.lpVerb =  L"runas";
    ShExecInfo.lpFile = (const wchar_t*) plik.utf16();
    ShExecInfo.lpParameters = (const wchar_t*) param.utf16();
    ShExecInfo.lpDirectory = nullptr;
    ShExecInfo.nShow = SW_SHOW;
    ShExecInfo.hInstApp = nullptr;
    int helper = ShellExecuteEx(&ShExecInfo);
    WaitForSingleObject(ShExecInfo.hProcess, INFINITE);

    return helper;
}

bool authorize()
{
    PasswordGet auth;
    auth.setWindowFlags(Qt::WindowTitleHint | Qt::MSWindowsFixedSizeDialogHint);
    auth.setModal(true);
    int authorization = auth.exec();

    if (authorization == QDialog::Accepted) return true;
    else return false;
}

void log(char t)
{
    QString name = qgetenv("USER");
    if (name.isEmpty()) name = qgetenv("USERNAME");

    if (QDir(QDir::toNativeSeparators(PrgrData + "AutoSwitch Debug")).exists() == false) QDir().mkdir(QDir::toNativeSeparators(PrgrData + "AutoSwitch Debug"));

    QString filename="AutoSwitch log.txt";
    QFile log(QDir::toNativeSeparators(PrgrData + "AutoSwitch Debug\\" + filename));
    if ( log.open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text) )
    {
        QTextStream file( &log );
        file.setCodec("UTF-8");
        file << QObject::tr("[AUTOMAT] ").toUtf8() << QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss").toUtf8() << QObject::tr(" [UŻYTKOWNIK: ").toUtf8() << name << "]" << QObject::tr("\t\tUruchomiono akcję: ").toUtf8();
        switch (t)
        {
        case 'w': file << QObject::tr("Wyłączenie Komputera\n").toUtf8(); break;
        case 'r': file << QObject::tr("Restart Komputera\n").toUtf8(); break;
        case 'u': file << QObject::tr("Uśpienie Komputera\n").toUtf8(); break;
        case 'h': file << QObject::tr("Hibernacja Komputera\n").toUtf8(); break;
        case 'l': file << QObject::tr("Wylogowanie Użytkownika\n").toUtf8(); break;
        case 'b': file << QObject::tr("Zablokowanie Komputera\n").toUtf8(); break;
        case 'm': file << QObject::tr("Wyłączenie Monitora\n").toUtf8(); break;
        }
        log.close();
    }
}
