#include "firstrun.h"
#include "ui_firstrun.h"

int selected_language = 1;

FirstRun::FirstRun(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FirstRun)
{
    ui->setupUi(this);

    QString lang = currentSystemLanguage();

    if ("Polish" == lang) ui->First_LanguageSelect->setCurrentIndex(0);
    else ui->First_LanguageSelect->setCurrentIndex(1);
}

FirstRun::~FirstRun()
{
    delete ui;
}

void FirstRun::on_First_LanguageSelect_currentIndexChanged(int index)
{
    if (index == 0) selected_language = 1;
    else if (index == 1) selected_language = 2;

    QTranslator tra;
    if (index == 1)
    {
        if (tra.load(":/translations/Resources/Languages/English.qm"))
        {
            qApp->installTranslator(&tra);
            ui->retranslateUi(this);
        }
    }
    else if (index == 0)
    {
        qApp->removeTranslator(&tra);
        ui->retranslateUi(this);
    }
}

void FirstRun::on_First_Theme_Choose_currentIndexChanged(int index)
{
    int themechoosen = index;

    if(themechoosen == 0)
    {
        qApp->setStyle("fusion");
        QPalette light = QStyleFactory::create("Fusion")->standardPalette();
        qApp->setPalette(light);
        qApp->setStyle("fusion");
        qApp->setPalette(light);
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
    }
    if(themechoosen == 2)
    {
        QPalette light = QStyleFactory::create("Fusion")->standardPalette();
        qApp->setPalette(light);
        qApp->setStyle("WindowsVista");
    }
}

void FirstRun::on_First_NextButton_clicked()
{
    ui->First_NextButton->setEnabled(0);

    QSettings setting(appData + "/config/settings.ini", QSettings::IniFormat);
    setting.beginGroup("Settings");
    setting.setValue("Language", selected_language);
    setting.setValue("WindowStyle", ui->First_Theme_Choose->currentIndex());
    setting.setValue("SendStatistics", ui->First_Statistics->currentIndex());
    setting.sync();
    setting.endGroup();

    Database* db = Database::getInstance();
    if (db->lookForUser() < 0) db->registerUser(ui->First_Statistics->currentIndex());

    restartApp();
}
