#include "mainwindow.h"
#include "ui_mainwindow.h"

QSystemTrayIcon *trayIcon;
QTableWidget *tasklist;

//MainWindow MainWindow::mainwindowInstance;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->Tasks_Table->setColumnWidth(0, 200);
    ui->Tasks_Table->setColumnWidth(1, 100);
    ui->Tasks_Table->setColumnWidth(2, 100);
    ui->Tasks_Table->setColumnWidth(3, 100);
    ui->Tasks_Table->setColumnWidth(4, 100);

    qApp->setStyle("fusion");
    MainWindow::tray();

    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()), this, SLOT(show_time()));
    timer->start(950);

    ui->Info_BuildDate_Date->setText(__DATE__);
    ui->Info_BuildDate_Time->setText(__TIME__);
    ui->About_Version->setText(ui->About_Version->text() + appVersion);

    if (isHibernateAvailable() == 0) ui->Main_Task_Hibernate->setEnabled(0);

    PasswordConfig *pc = PasswordConfig::getInstance();
    if (pc->getPasswordrules_run())
    {
        if (authorize() == false) exit(10);
    }

    Autorun AR;
    AR.autorunLoad(1);
    AR.close();

    isUserRegistered();
    ui->retranslateUi(this);

    setAlignedIcons();

    SocketCommunicator server(nullptr, this);
    server.StartServer();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent * event)
{
    if (taskState())
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("Uwaga!"), tr("Czy jesteś pewny, że chcesz wyłączyć aplikację? Zaplanowane zadania nie zostaną wykonane!"));
        if (reply == QMessageBox::Yes)
        {
            QApplication::quit();
            trayIcon->deleteLater();
        }
        else event->ignore();
    }
    else
    {
        QApplication::quit();
        trayIcon->deleteLater();
    }
}

void MainWindow::tray()
{
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/pictures/Resources/Pictures/wylacznik.png"));

    connect(trayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this,SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

    trayIcon = trayIcon;

    Settings *s = Settings::getInstance();
    s->settingsLoad();
    s->close();
    tasklist = ui->Tasks_Table;
    TraySubmenu tsm;

    trayIconMenu = new QMenu(this);
    ShowAction = new QAction(tr("Pokaż program"), this);
    connect(ShowAction, &QAction::triggered, this, &MainWindow::showMainWindow);
    trayIconMenu->addAction(ShowAction);
    ShowAction->setIcon(QIcon(":/icons/Resources/Icons/show.png"));
    trayIconMenu->addSeparator();

    tsm.mainMenu = trayIconMenu;

//    if (s->getTraySettings_Shutdown())
//    {
//        tsm.actionSingle = ShutdownAction;
//        memcpy(MShutdown, tsm.action, sizeof(MShutdown));
//        tsm.actionText = tr("Wyłącz Komputer");
//        tsm.iconPath = ":/icons/Resources/Icons/shutdown.png";
//        tsm.mode = 'w';
//        tsm.subMenu = subMenu_Shutdown;
//        if (s->getTraySettings_Shutdown()) trayBuilder(tsm, 1);
//        else trayBuilder(tsm, 0);
//    }

//    if (s->getTraySettings_Reboot())
//    {
//        tsm.actionSingle = RebootAction;
//        memcpy(MReboot, tsm.action, sizeof(MReboot));
//        tsm.actionText = tr("Uruchom ponownie Komputer");
//        tsm.iconPath = ":/icons/Resources/Icons/reboot.png";
//        tsm.mode = 'r';
//        tsm.subMenu = subMenu_Reboot;
//        if (s->getTraySettings_Reboot()) trayBuilder(tsm, 1);
//        else trayBuilder(tsm, 0);
//    }

//    if (s->getTraySettings_Sleep())
//    {
//        tsm.actionSingle = SleepAction;
//        memcpy(MSleep, tsm.action, sizeof(MSleep));
//        tsm.actionText = tr("Uśpij Komputer");
//        tsm.iconPath = ":/icons/Resources/Icons/hibernate.png";
//        tsm.mode = 'u';
//        tsm.subMenu = subMenu_Sleep;
//        if (s->getTraySettings_Sleep()) trayBuilder(tsm, 1);
//        else trayBuilder(tsm, 0);
//    }

//    if (s->getTraySettings_Hibernate())
//    {
//        tsm.actionSingle = HibernateAction;
//        memcpy(MHibernate, tsm.action, sizeof(MHibernate));
//        tsm.actionText = tr("Hibernuj Komputer");
//        tsm.iconPath = ":/icons/Resources/Icons/hibernate.png";
//        tsm.mode = 'h';
//        tsm.subMenu = subMenu_Hibernate;
//        if (s->getTraySettings_Hibernate()) trayBuilder(tsm, 1);
//        else trayBuilder(tsm, 0);
//    }

//    if (s->getTraySettings_Logoff())
//    {
//        tsm.actionSingle = LogoffAction;
//        memcpy(MLogoff, tsm.action, sizeof(MLogoff));
//        tsm.actionText = tr("Wyloguj się");
//        tsm.iconPath = ":/icons/Resources/Icons/logout.png";
//        tsm.mode = 'l';
//        tsm.subMenu = subMenu_Logoff;
//        if (s->getTraySettings_Logoff()) trayBuilder(tsm, 1);
//        else trayBuilder(tsm, 0);
//    }

//    if (s->getTraySettings_Lock())
//    {
//        tsm.actionSingle = LockAction;
//        memcpy(MLock, tsm.action, sizeof(MLock));
//        tsm.actionText = tr("Zablokuj Komputer");
//        tsm.iconPath = ":/icons/Resources/Icons/lock.png";
//        tsm.mode = 'b';
//        tsm.subMenu = subMenu_Lock;
//        if (s->getTraySettings_Lock()) trayBuilder(tsm, 1);
//        else trayBuilder(tsm, 0);
//    }

//    if (s->getTraySettings_Monitor())
//    {
//        tsm.actionSingle = MonitoroffAction;
//        memcpy(MMonitoroff, tsm.action, sizeof(MMonitoroff));
//        tsm.actionText = tr("Wyłącz Monitor");
//        tsm.iconPath = ":/icons/Resources/Icons/monitor_off.png";
//        tsm.mode = 'm';
//        tsm.subMenu = subMenu_MonitorOff;
//        if (s->getTraySettings_Monitor()) trayBuilder(tsm, 1);
//        else trayBuilder(tsm, 0);
//    }

    trayIconMenu->addSeparator();
    WyjdzAction = new QAction(tr("Wyjdź z programu"), this);
    connect(WyjdzAction,SIGNAL(triggered()),this, SLOT(close()));
    trayIconMenu->addAction(WyjdzAction);
    WyjdzAction->setIcon(QIcon(":/icons/Resources/Icons/cancel.png"));
}

void MainWindow::trayBuilder(TraySubmenu tsm, bool enableSubmenu)
{
    if (enableSubmenu)
    {
        tsm.action[0] = new QAction(tr("Teraz"), this);
        connect(tsm.action[0], &QAction::triggered, bind(&trayAction, tsm.mode, 's', 1));
        tsm.action[1] = new QAction(tr("Za ") + "15" + tr(" sekund"), this);
        connect(tsm.action[1], &QAction::triggered, bind(&trayAction, tsm.mode, 's', 15));
        tsm.action[2] = new QAction(tr("Za ") + "30" + tr(" sekund"), this);
        connect(tsm.action[2], &QAction::triggered, bind(&trayAction, tsm.mode, 's', 30));
        tsm.action[3] = new QAction(tr("Za ") + "1" + tr(" minut"), this);
        connect(tsm.action[3], &QAction::triggered, bind(&trayAction, tsm.mode, 'm', 1));
        tsm.action[4] = new QAction(tr("Za ") + "2" + tr(" minut"), this);
        connect(tsm.action[4], &QAction::triggered, bind(&trayAction, tsm.mode, 'm', 2));
        tsm.action[5] = new QAction(tr("Za ") + "5" + tr(" minut"), this);
        connect(tsm.action[5], &QAction::triggered, bind(&trayAction, tsm.mode, 'm', 5));
        tsm.action[6] = new QAction(tr("Za ") + "10" + tr(" minut"), this);
        connect(tsm.action[6], &QAction::triggered, bind(&trayAction, tsm.mode, 'm', 10));
        tsm.action[7] = new QAction(tr("Za ") + "15" + tr(" minut"), this);
        connect(tsm.action[7], &QAction::triggered, bind(&trayAction, tsm.mode, 'm', 15));
        tsm.action[8] = new QAction(tr("Za ") + "30" + tr(" minut"), this);
        connect(tsm.action[8], &QAction::triggered, bind(&trayAction, tsm.mode, 'm', 30));
        tsm.action[9] = new QAction(tr("Za ") + "45" + tr(" minut"), this);
        connect(tsm.action[9], &QAction::triggered, bind(&trayAction, tsm.mode, 'm', 45));
        tsm.action[10] = new QAction(tr("Za ") + "1" + tr(" godzin"), this);
        connect(tsm.action[10], &QAction::triggered, bind(&trayAction, tsm.mode, 'h', 1));
        tsm.action[11] = new QAction(tr("Za ") + "2" + tr(" godzin"), this);
        connect(tsm.action[11], &QAction::triggered, bind(&trayAction, tsm.mode, 'h', 2));
        tsm.action[12] = new QAction(tr("Za ") + "3" + tr(" godzin"), this);
        connect(tsm.action[12], &QAction::triggered, bind(&trayAction, tsm.mode, 'h', 3));
        tsm.action[13] = new QAction(tr("Za ") + "6" + tr(" godzin"), this);
        connect(tsm.action[13], &QAction::triggered, bind(&trayAction, tsm.mode, 'h', 6));
        tsm.action[14] = new QAction(tr("Za ") + "12" + tr(" godzin"), this);
        connect(tsm.action[14], &QAction::triggered, bind(&trayAction, tsm.mode, 'h', 12));
        tsm.action[15] = new QAction(tr("Za ") + "1" + tr(" dzień"), this);
        connect(tsm.action[15], &QAction::triggered, bind(&trayAction, tsm.mode, 'h', 24));

        tsm.subMenu = tsm.mainMenu->addMenu(tsm.actionText);
        tsm.subMenu->setIcon(QIcon(tsm.iconPath));
        tsm.subMenu->addSection(tr("Kiedy?"));
        tsm.subMenu->addSeparator();
        tsm.subMenu->addAction(tsm.action[0]);
        tsm.subMenu->addAction(tsm.action[1]);
        tsm.subMenu->addAction(tsm.action[2]);
        tsm.subMenu->addAction(tsm.action[3]);
        tsm.subMenu->addAction(tsm.action[4]);
        tsm.subMenu->addAction(tsm.action[5]);
        tsm.subMenu->addAction(tsm.action[6]);
        tsm.subMenu->addAction(tsm.action[7]);
        tsm.subMenu->addAction(tsm.action[8]);
        tsm.subMenu->addAction(tsm.action[9]);
        tsm.subMenu->addAction(tsm.action[10]);
        tsm.subMenu->addAction(tsm.action[11]);
        tsm.subMenu->addAction(tsm.action[12]);
        tsm.subMenu->addAction(tsm.action[13]);
        tsm.subMenu->addAction(tsm.action[14]);
        tsm.subMenu->addAction(tsm.action[15]);
    }
    else
    {
        tsm.actionSingle = new QAction(tsm.actionText, this);
        connect(tsm.actionSingle, &QAction::triggered, bind(&trayAction, tsm.mode, 's', 1));
        tsm.mainMenu->addAction(tsm.actionSingle);
        tsm.actionSingle->setIcon(QIcon(tsm.iconPath));
    }
}

void MainWindow::setAlignedIcons()
{
    int w = 25;
    int h = 25;
    //Main
    QPixmap ico;
    ico.load(":/icons/Resources/Icons/shutdown.png");
    ui->Main_Task_Shutdown->setPixmap(ico.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ico.load(":/icons/Resources/Icons/reboot.png");
    ui->Main_Task_Reboot->setPixmap(ico.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ico.load(":/icons/Resources/Icons/hibernate.png");
    ui->Main_Task_Sleep->setPixmap(ico.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ico.load(":/icons/Resources/Icons/hibernate.png");
    ui->Main_Task_Hibernate->setPixmap(ico.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ico.load(":/icons/Resources/Icons/logout.png");
    ui->Main_Task_LogOff->setPixmap(ico.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ico.load(":/icons/Resources/Icons/lock.png");
    ui->Main_Task_Lock->setPixmap(ico.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ico.load(":/icons/Resources/Icons/monitor_off.png");
    ui->Main_Task_MonitorOff->setPixmap(ico.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ico.load(":/icons/Resources/Icons/question.png");
    ui->Main_Task_Help->setPixmap(ico.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    ico.load(":/icons/Resources/Icons/monitor_off.png");
    ui->Main_Actions_MonitorOff->setPixmap(ico.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ico.load(":/icons/Resources/Icons/hidden.png");
    ui->Main_Actions_Hide->setPixmap(ico.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ico.load(":/icons/Resources/Icons/info.png");
    ui->Main_Actions_About->setPixmap(ico.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ico.load(":/icons/Resources/Icons/question.png");
    ui->Main_Actions_Help->setPixmap(ico.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    //When
    ico.load(":/icons/Resources/Icons/cancel.png");
    ui->When_ClearForm->setPixmap(ico.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ico.load(":/icons/Resources/Icons/synchronize_time.png");
    ui->When_TimeSyncButton->setPixmap(ico.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    //Active Tasks
    ico.load(":/icons/Resources/Icons/cancel.png");
    ui->Tasks_CancelAll->setPixmap(ico.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ico.load(":/icons/Resources/Icons/automat.png");
    ui->Tasks_Actions_MainMenu->setPixmap(ico.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    //About
    ico.load(":/icons/Resources/Icons/changelog.png");
    ui->About_Changelog->setPixmap(ico.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ico.load(":/icons/Resources/Icons/automat.png");
    ui->About_Action_MainMenu->setPixmap(ico.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    w = 32;
    h = 32;
    //Main
    ico.load(":/icons/Resources/Icons/list.png");
    ui->Main_Actions_ActiveTasks->setPixmap(ico.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ico.load(":/icons/Resources/Icons/autorun.png");
    ui->Main_Actions_Autorun->setPixmap(ico.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ico.load(":/icons/Resources/Icons/settings.png");
    ui->Main_Actions_Settings->setPixmap(ico.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    //Confirmation
    ico.load(":/icons/Resources/Icons/automat.png");
    ui->Confirm_MenuButton->setPixmap(ico.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ico.load(":/icons/Resources/Icons/list.png");
    ui->Confirm_TaskButton->setPixmap(ico.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    //Error
    ico.load(":/icons/Resources/Icons/automat.png");
    ui->Error_MenuButton->setPixmap(ico.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ico.load(":/icons/Resources/Icons/list.png");
    ui->Error_ActiveTasks->setPixmap(ico.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    //About
    ico.load(":/icons/Resources/Icons/info.png");
    ui->About_Opinion->setPixmap(ico.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    w = 36;
    h = 36;
    //When
    ico.load(":/icons/Resources/Icons/accept.png");
    ui->When_AddTask->setPixmap(ico.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ico.load(":/icons/Resources/Icons/cancel.png");
    ui->When_Cancel->setPixmap(ico.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
    case QSystemTrayIcon::Context:
        trayIconMenu->popup(QCursor::pos());
        break;
    case QSystemTrayIcon::DoubleClick:
    {
        if (MainWindow::isHidden()) MainWindow::show();
        else if (MainWindow::isMinimized()) MainWindow::setWindowState(Qt::WindowActive);
    }
        break;
    }
}

void MainWindow::handleMessage(const QString &message)
{
    PasswordConfig *pc = PasswordConfig::getInstance();
    if (pc->getPasswordrules_showmainwindow())
    {
        if (authorize())
        {
            if(message == "1")
            {
                setVisibility(1);
                showMainWindow();
            }
        }
    }
    else
    {
        if(message == "1")
        {
            setVisibility(1);
            showMainWindow();
        }
    }
}

bool MainWindow::taskState()
{
    for (int i = 0; i < 25; i++)
    {
        if (TAB_stanu[i]) return 1;
    }
    return 0;
}

void MainWindow::show_time()
{
    QTime time = QTime::currentTime();
    QString currentTime = time.toString("hh:mm:ss");

    if((time.second() % 2) == 0)
    {
        currentTime[2] = ' ';
        currentTime[5] = ' ';
    }
    ui->CurrentTime->setText(currentTime);
}

void addTaskDescription(bool repeatability, short number, QString mode, QString date, QString time, QString weekDays)
{
    if(repeatability)
    {
        tasklist->item(number, 0)->setText(mode);
        tasklist->item(number, 1)->setText("-");
        tasklist->item(number, 2)->setText(weekDays);
        tasklist->item(number, 3)->setText(time);
    }
    else
    {
        tasklist->item(number, 0)->setText(mode);
        tasklist->item(number, 1)->setText(date);
        tasklist->item(number, 2)->setText("-");
        tasklist->item(number, 3)->setText(time);
    }
    tasklist->viewport()->update();
}

void MainWindow::isUserRegistered()
{
    Settings *s = Settings::getInstance();
    Database* db = Database::getInstance();
    if(db->lookForUser() == -1) db->registerUser(s->getSendStatistics());
}

void MainWindow::openHelpDialog()
{
    Help help;
    help.setWindowFlags(Qt::WindowTitleHint | Qt::MSWindowsFixedSizeDialogHint);
    help.setModal(true);
    help.exec();
}

void MainWindow::openInfoSwitchDialog()
{
    InfoSwitch autoinfo;
    autoinfo.setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);
    autoinfo.setModal(true);
    autoinfo.exec();
}

void MainWindow::openSettingsDialog()
{
    PasswordConfig *pc = PasswordConfig::getInstance();
    if (pc->getPasswordrules_settingsedit() == 0 || authorize())
    {
        Settings ustawienia;
        ustawienia.setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);
        ustawienia.setModal(true);
        ustawienia.exec();
    }
}

void MainWindow::openAutorunDialog()
{
    PasswordConfig *pc = PasswordConfig::getInstance();
    if (pc->getPasswordrules_autorunedit() == 0 || authorize())
    {
        Autorun autostart;
        autostart.setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);
        autostart.setModal(true);
        autostart.exec();
    }
}

void MainWindow::showMainWindow()
{
    PasswordConfig *pc = PasswordConfig::getInstance();
    if (pc->getPasswordrules_showmainwindow())
    {
        if (authorize()) this->showNormal();
    }
    else this->showNormal();
}

void MainWindow::appendSocket(QString s)
{
    ui->SocketOutput->append(s);
}

void MainWindow::hideApp()
{
    Settings *s = Settings::getInstance();
    if (s->getShowHideDialog())
    {
        HideDialog hidemsg;
        hidemsg.setWindowFlags(Qt::WindowTitleHint | Qt::MSWindowsFixedSizeDialogHint);
        hidemsg.setModal(true);
        hidemsg.exec();
    }
    setVisibility(0);
}

void MainWindow::setVisibility(bool v)
{
    if (v == 0 && MainWindow::isHidden() == false) MainWindow::hide();
    else if(MainWindow::isHidden()) MainWindow::show();
}

//UI
//Page_Main
void MainWindow::on_Main_Task_Shutdown_clicked()
{
    choosenMode = 'w';
    ui->MainWindow_Stack->setCurrentIndex(1);
    ui->When_ForceCB->setEnabled(1);
}

void MainWindow::on_Main_Task_Reboot_clicked()
{
    choosenMode = 'r';
    ui->MainWindow_Stack->setCurrentIndex(1);
    ui->When_ForceCB->setEnabled(1);
}

void MainWindow::on_Main_Task_Sleep_clicked()
{
    choosenMode = 'u';
    ui->MainWindow_Stack->setCurrentIndex(1);
}

void MainWindow::on_Main_Task_Hibernate_clicked()
{
    choosenMode = 'h';
    ui->MainWindow_Stack->setCurrentIndex(1);
}

void MainWindow::on_Main_Task_LogOff_clicked()
{
    choosenMode = 'l';
    ui->MainWindow_Stack->setCurrentIndex(1);
    ui->When_ForceCB->setEnabled(1);
}

void MainWindow::on_Main_Task_Lock_clicked()
{
    choosenMode = 'b';
    ui->MainWindow_Stack->setCurrentIndex(1);
}

void MainWindow::on_Main_Task_MonitorOff_clicked()
{
    choosenMode = 'm';
    ui->MainWindow_Stack->setCurrentIndex(1);
}

void MainWindow::on_Main_Task_Help_clicked()
{
    openInfoSwitchDialog();
}
//
void MainWindow::on_Main_Actions_ActiveTasks_clicked()
{
    ui->MainWindow_Stack->setCurrentIndex(4);
}

void MainWindow::on_Main_Actions_Autorun_clicked()
{
    openAutorunDialog();
}

void MainWindow::on_Main_Actions_Settings_clicked()
{
    openSettingsDialog();
}

void MainWindow::on_Main_Actions_MonitorOff_clicked()
{
    monitorOff();
}

void MainWindow::on_Main_Actions_Hide_clicked()
{
    hideApp();
}

void MainWindow::on_Main_Actions_About_clicked()
{
    ui->MainWindow_Stack->setCurrentIndex(5);
}

void MainWindow::on_Main_Actions_Help_clicked()
{
    openHelpDialog();
}

//Page_When
void MainWindow::resetWhenPage()
{
    ui->When_Mode_SecondsRB->setChecked(0);
    ui->When_Mode_MinutesRB->setChecked(0);
    ui->When_Mode_HoursRB->setChecked(0);
    ui->When_Mode_AtHourRB->setChecked(0);
    ui->When_Mode_CustomRB->setChecked(0);
    ui->When_Mode_AtDateRB->setChecked(0);

    ui->When_Mode_Seconds_Choose->setCurrentIndex(0);
    ui->When_Mode_Minutes_Choose->setCurrentIndex(0);
    ui->When_Mode_Hours_Choose->setCurrentIndex(0);
    ui->When_Mode_Custom_Unit->setCurrentIndex(0);
    ui->When_Mode_Custom_Quantity->setText("");
    whenSyncTime();

    ui->When_ForceCB->setChecked(0);
    ui->When_AddTask->setEnabled(0);
}

void MainWindow::whenSyncTime()
{
    QDateTime dt = QDateTime::currentDateTime();
    ui->When_Mode_AtHour_Time->setTime(dt.time());
    ui->When_Mode_AtDate_Time->setTime(dt.time());
    ui->When_Mode_AtDate_Date->setDate(dt.date());
}

//

void MainWindow::on_When_Mode_Now_clicked()
{
    Settings *s = Settings::getInstance();
    if(s->getPlzLog()) log(choosenMode);
    QString temp;
    bool force = ui->When_ForceCB->isChecked();

    ui->MainWindow_Stack->setCurrentIndex(2);
    setConfirmationIcon(choosenMode);
    switch (choosenMode)
    {
    case 'w': temp += QObject::tr("Komputer został wyłaczony"); break;
    case 'r': temp += QObject::tr("Komputer został zrestartowany"); break;
    case 'u': temp += QObject::tr("Komputer został uśpiony"); break;
    case 'h': temp += QObject::tr("Komputer został zahibernowany"); break;
    case 'l': temp += QObject::tr("Zostałeś wylogowany"); break;
    case 'b': temp += QObject::tr("Komputer został zablokowany"); break;
    case 'm': temp += QObject::tr("Monitor został wyłaczony"); break;
    }
    ui->Confirm_Text->setText(temp);

    switch(choosenMode)
    {
    case 'w': shutdownComputer(force); break;
    case 'r': rebootComputer(force); break;
    case 'u': sleepComputer(); break;
    case 'h': hibernateComputer(); break;
    case 'l': logoffUser(force); break;
    case 'b': lockComputer(); break;
    case 'm': monitorOff(); break;
    }

    resetWhenPage();
}

void MainWindow::on_When_Mode_SecondsRB_pressed()
{
    ui->When_AddTask->setEnabled(1);
}

void MainWindow::on_When_Mode_MinutesRB_pressed()
{
    ui->When_AddTask->setEnabled(1);
}

void MainWindow::on_When_Mode_HoursRB_pressed()
{
    ui->When_AddTask->setEnabled(1);
}

void MainWindow::on_When_Mode_AtHourRB_pressed()
{
    ui->When_AddTask->setEnabled(1);
}

void MainWindow::on_When_Mode_CustomRB_pressed()
{
    ui->When_AddTask->setEnabled(1);
}

void MainWindow::on_When_Mode_AtDateRB_pressed()
{
    ui->When_AddTask->setEnabled(1);
}
//
void MainWindow::on_When_AddTask_clicked()
{
    struct Task task;
    task.mode = choosenMode;

    short choosenOption;
    if (ui->When_Mode_SecondsRB->isChecked()) choosenOption = 1;
    else if (ui->When_Mode_MinutesRB->isChecked()) choosenOption = 2;
    else if (ui->When_Mode_HoursRB->isChecked()) choosenOption = 3;
    else if (ui->When_Mode_AtHourRB->isChecked()) choosenOption = 4;
    else if (ui->When_Mode_CustomRB->isChecked()) choosenOption = 5;
    else choosenOption = 6; //else if (ui->When_Mode_AtDateRB->isChecked()) choosenOption = 6;
    
    task.force = ui->When_ForceCB->isChecked();
    unsigned long long choosenQuantity = 0;
    
    switch (choosenOption)
    {
    case 1:
    {
        choosenQuantity = ui->When_Mode_Seconds_Choose->currentText().toULongLong();
        task.time = countTime(choosenQuantity, 's', task.time);
        preAutomat(task);
    }
        break;
    case 2:
    {
        choosenQuantity = ui->When_Mode_Minutes_Choose->currentText().toULongLong();
        task.time = countTime(choosenQuantity, 'm', task.time);
        preAutomat(task);
    }
        break;
    case 3:
    {
        choosenQuantity = ui->When_Mode_Hours_Choose->currentText().toULongLong();
        task.time = countTime(choosenQuantity, 'h', task.time);
        preAutomat(task);
    }
        break;
    case 4:
    {
        short seconds = static_cast<short>(ui->When_Mode_AtHour_Time->time().second());
        short minutes = static_cast<short>(ui->When_Mode_AtHour_Time->time().minute());
        short hours = static_cast<short>(ui->When_Mode_AtHour_Time->time().hour());

        QDateTime dt = QDateTime::currentDateTime();
        if(hours <= dt.time().hour() && minutes <= dt.time().minute() && seconds < dt.time().second())
        {
            task.time = countDate('d', 1, task.time);
        }
        else task.time.dt.setDate(dt.date());

        task.time.dt.setTime(dt.time());
        preAutomat(task);
    }
        break;
    case 5:
    {
        char timeUnit;
        unsigned long long timeQuantity = ui->When_Mode_Custom_Quantity->text().toULongLong();
        if(ui->When_Mode_Custom_Unit->currentIndex() == 0) timeUnit = 's';
        else if(ui->When_Mode_Custom_Unit->currentIndex() == 1) timeUnit = 'm';
        else timeUnit = 'h'; //else if(ui->When_Mode_Custom_Unit->currentIndex() == 2) timeUnit = 'h';
        task.time = countTime(timeQuantity, timeUnit, task.time);
        preAutomat(task);
    }
        break;
    case 6:
    {
        task.time.dt.setDate(QDate(ui->When_Mode_AtDate_Date->date().year(),
                                   ui->When_Mode_AtDate_Date->date().month(),
                                   ui->When_Mode_AtDate_Date->date().day()));
        task.time.dt.setTime(QTime(ui->When_Mode_AtDate_Time->time().hour(),
                                   ui->When_Mode_AtDate_Time->time().minute(),
                                   ui->When_Mode_AtDate_Time->time().second()));

        preAutomat(task);
    }
        break;
    }

    QString temp;
    QString n2s_temp;

    if(preAutomat_Error == 0)
    {
        temp += QObject::tr("Akcja została zaplanowana: ");
        switch (task.mode)
        {
        case 'w': temp += QObject::tr("Wyłączenie Komputera "); break;
        case 'r': temp += QObject::tr("Restart Komputera "); break;
        case 'u': temp += QObject::tr("Uśpienie Komputera "); break;
        case 'h': temp += QObject::tr("Hibernacja Komputera "); break;
        case 'l': temp += QObject::tr("Wylogowanie Użytkownika "); break;
        case 'b': temp += QObject::tr("Zablokowanie Komputera "); break;
        case 'm': temp += QObject::tr("Wyłączenie Monitora "); break;
        }

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

        temp += QObject::tr(" o godzinie: ");
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

        ui->MainWindow_Stack->setCurrentIndex(2);
        setConfirmationIcon(task.mode);
        ui->Confirm_Text->setText(temp);
    }
    else
    {
        ui->MainWindow_Stack->setCurrentIndex(3);
        ui->Error_Text->setText(QObject::tr("W jednym momencie nie może być aktywne więcej niż 25 zadań. Wyłącz któreś i spróbuj ponownie!"));
        preAutomat_Error = 0;
    }
    resetWhenPage();
}

void MainWindow::on_When_ClearForm_clicked()
{
    resetWhenPage();
}

void MainWindow::on_When_Cancel_clicked()
{
    choosenMode = '\0';
    resetWhenPage();
    ui->MainWindow_Stack->setCurrentIndex(0);
    ui->When_ForceCB->setEnabled(0);
}

void MainWindow::on_When_TimeSyncButton_clicked()
{
    whenSyncTime();
}

//Page_Confirm
void MainWindow::setConfirmationIcon(char mode)
{
    QPixmap pix;
    switch(mode)
    {
    case 'w': pix.load(":/icons/Resources/Icons/shutdown.png"); ui->Confirm_Icon->setPixmap(pix); break;
    case 'r': pix.load(":/icons/Resources/Icons/reboot.png"); ui->Confirm_Icon->setPixmap(pix); break;
    case 'u': pix.load(":/icons/Resources/Icons/hibernate.png"); ui->Confirm_Icon->setPixmap(pix); break;
    case 'h': pix.load(":/icons/Resources/Icons/hibernate.png"); ui->Confirm_Icon->setPixmap(pix); break;
    case 'l': pix.load(":/icons/Resources/Icons/logout.png"); ui->Confirm_Icon->setPixmap(pix); break;
    case 'b': pix.load(":/icons/Resources/Icons/lock.png"); ui->Confirm_Icon->setPixmap(pix); break;
    case 'm': pix.load(":/icons/Resources/Icons/monitor_off.png"); ui->Confirm_Icon->setPixmap(pix); break;
    }
}

void MainWindow::on_Confirm_MenuButton_clicked()
{
    ui->Confirm_Text->setText("");
    ui->MainWindow_Stack->setCurrentIndex(0);
}

void MainWindow::on_Confirm_TaskButton_clicked()
{
    ui->Confirm_Text->setText("");
    ui->MainWindow_Stack->setCurrentIndex(4);
}

//Page_Error
void MainWindow::on_Error_MenuButton_clicked()
{
    ui->Error_Text->setText("");
    ui->MainWindow_Stack->setCurrentIndex(0);
}

void MainWindow::on_Error_ActiveTasks_clicked()
{
    ui->Error_Text->setText("");
    ui->MainWindow_Stack->setCurrentIndex(4);
}

//Page_Tasks
void MainWindow::on_Tasks_CancelAll_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Uwaga!"), tr("Czy jesteś pewny, że chcesz anulować WSZYSTKIE ZADANIA [zadania powtarzające się do ponownego uruchomienia programu]?"));
    if (reply == QMessageBox::Yes)
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("Ostatnia szansa!"), tr("Definitywnie? Tej operacji NIE można cofnąć!"));
        if (reply == QMessageBox::Yes)
        {
            for(int i = 0; i < 25; i++)
            {
                TAB_stanu[i] = 0;
                opis_watku[i] = "";
                opis_watku_dni[i] = "";
                opis_watku_data[i] = "";
                opis_watku_godzina[i] = "";

                tasklist->item(i, 0)->setText("");
                tasklist->item(i, 1)->setText("");
                tasklist->item(i, 2)->setText("");
                tasklist->item(i, 3)->setText("");

                ui->Tasks_Table->viewport()->update();
            }
        }
    }
}

void MainWindow::on_Tasks_Table_cellClicked(int row, int column)
{
    if(column == 4 && TAB_stanu[row])
    {
        QString task = tasklist->item(row, 0)->text();
        QString date = tasklist->item(row, 1)->text();
        QString days = tasklist->item(row, 2)->text();
        QString hour = tasklist->item(row, 3)->text();

        QMessageBox::StandardButton reply;
        if(date.length() > 1)
        {
            reply = QMessageBox::question(this, tr("Uwaga!"), tr("Czy jesteś pewny, że chcesz anulować zadanie: ") + task + " " + date + tr(" o godzinie ") + hour, QMessageBox::Yes|QMessageBox::No);
            if (reply == QMessageBox::Yes)
            {
                TAB_stanu[row] = 0;
                opis_watku[row] = "";
                opis_watku_dni[row] = "";
                opis_watku_data[row] = "";
                opis_watku_godzina[row] = "";

                tasklist->item(row, 0)->setText("");
                tasklist->item(row, 1)->setText("");
                tasklist->item(row, 2)->setText("");
                tasklist->item(row, 3)->setText("");

                ui->Tasks_Table->viewport()->update();
            }
        }
        else if(date.length() == 1)
        {
            reply = QMessageBox::question(this, tr("Uwaga!"), tr("Czy jesteś pewny, że chcesz anulować zadanie: ") + task + tr(" zawsze w dni: ") + days + tr(" o godzinie ") + hour + tr(" (Po ponownym uruchomieniu aplikacji zadanie będzie znów aktywne)"));
            if (reply == QMessageBox::Yes)
            {
                TAB_stanu[row] = 0;
                opis_watku[row] = "";
                opis_watku_dni[row] = "";
                opis_watku_data[row] = "";
                opis_watku_godzina[row] = "";

                tasklist->item(row, 0)->setText("");
                tasklist->item(row, 1)->setText("");
                tasklist->item(row, 2)->setText("");
                tasklist->item(row, 3)->setText("");

                ui->Tasks_Table->viewport()->update();

                QMessageBox::information(this, tr("Informacja"), tr("Zadanie anulowane! Aby anulować to zadanie na stałe przejdź do sekcji \"Autouruchamianie\"."));
            }
        }
    }
}
//
void MainWindow::on_Tasks_Actions_MainMenu_clicked()
{
    ui->MainWindow_Stack->setCurrentIndex(0);
}

void MainWindow::on_Tasks_Actions_MonitorOff_clicked()
{
    monitorOff();
}

void MainWindow::on_Tasks_Actions_Hide_clicked()
{
    hideApp();
}

void MainWindow::on_Tasks_Actions_Autorun_clicked()
{
    openAutorunDialog();
}

void MainWindow::on_Tasks_Actions_Settings_clicked()
{
    openSettingsDialog();
}

void MainWindow::on_Tasks_Actions_About_clicked()
{
    ui->MainWindow_Stack->setCurrentIndex(5);
}

void MainWindow::on_Tasks_Actions_Help_clicked()
{
    openHelpDialog();
}

//Page_About
void MainWindow::on_About_Changelog_clicked()
{
    ResTxtView rv;
    rv.setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);
    rv.setModal(true);
    rv.loadChangelog();
    rv.setWindowTitle(tr("Lista zmian"));
    rv.exec();

}

void MainWindow::on_About_License_clicked()
{
    ResTxtView rv;
    rv.setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);
    rv.setModal(true);
    rv.loadLicense();
    rv.setWindowTitle(tr("Licencja"));
    rv.exec();
}

void MainWindow::on_About_Opinion_clicked()
{
    Opinion op;
    op.setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);
    op.setModal(true);
    op.exec();
}

void MainWindow::on_About_Donate_clicked()
{
    QString link = "https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=TH5ZJ7DGT9DB6";
    QDesktopServices::openUrl(QUrl(link));
}
//
void MainWindow::on_About_Action_MainMenu_clicked()
{
    ui->MainWindow_Stack->setCurrentIndex(0);
}

void MainWindow::on_About_Action_MonitorOff_clicked()
{
    monitorOff();
}

void MainWindow::on_About_Action_Hide_clicked()
{
    hideApp();
}

void MainWindow::on_About_Action_ActiveTasks_clicked()
{
    ui->MainWindow_Stack->setCurrentIndex(4);
}

void MainWindow::on_About_Action_Settings_clicked()
{
    openSettingsDialog();
}

void MainWindow::on_About_Action_Autorun_clicked()
{
    openAutorunDialog();
}

void MainWindow::on_About_Action_Help_clicked()
{
    openHelpDialog();
}
//UI_End
