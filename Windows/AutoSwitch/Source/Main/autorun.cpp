#include "autorun.h"
#include "ui_autorun.h"

Autorun::Autorun(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Autorun)
{
    if (QFile(appData + "/config/autorun.ini").exists() == false)
    {
        QMessageBox::information(this, tr("Brak pliku konfiguracyjnego!"), tr("Plik konfiguracyjny \"autorun.ini\" nie istnieje! Program utworzy nowy plik z domyślnymi ustawieniami"));

        if (QDir(appData).exists() == false) QDir().mkdir(appData);
        if (QDir(appData + "/config").exists() == false) QDir().mkdir(appData + "/config");

        QFile::copy(":/ConfigurationFiles/resources/ConfigurationFiles/autorun.ini", appData + "/config/autorun.ini");
        QFileDevice::Permissions p = QFile(appData + "/config/autorun.ini").permissions();
        if (!(p & QFileDevice::WriteOwner)) QFile(appData + "/config/autorun.ini").setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner);
    }

    ui->setupUi(this);

    ui->Autorun_table->setColumnWidth(0, 250);
    ui->Autorun_table->setColumnWidth(1, 120);
    ui->Autorun_table->setColumnWidth(2, 150);
    ui->Autorun_table->setColumnWidth(3, 120);
    ui->Autorun_table->setColumnWidth(4, 100);

    if (isHibernateAvailable() == 0) ui->AutoRun_boxAdd_ChooseMode->setItemData(3, 0, Qt::UserRole - 1);

    timeSync();

    ui->retranslateUi(this);

    autorunLoad(0);
    changes = 0;

    autorunStateUpdater();
}

Autorun::~Autorun()
{
    delete ui;
}

void Autorun::closeEvent(QCloseEvent * event)
{
    if (changes)
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("Uwaga!"), tr("Czy chcesz zapisać te ustawienia?"));
        if (reply == QMessageBox::Yes)
        {
            autorunSave();
        }
        else
        {
            for(int i = 0; i < 25; i++)
            {
                autoMode[i] = 0;
                autoState[i] = 0;
                for(short x = 0; x < 7; x++)
                {
                    autoDays[i][x] = 0;
                }
                autoRepeat[i] = 0;

                ui->Autorun_table->item(i, 0)->setText("");
                ui->Autorun_table->item(i, 1)->setText("");
                ui->Autorun_table->item(i, 2)->setText("");
                ui->Autorun_table->item(i, 3)->setText("");
            }
            ui->Autorun_table->viewport()->update();
            autorunLoad(0);
        }
    }
    event->accept();
}

void Autorun::autorunSave()
{
    QSettings autorun(appData + "/config/autorun.ini", QSettings::IniFormat);

    autorun.beginGroup("AutourunSettings");
    autorun.setValue("Autorun_hide", autorun_hide);
    autorun.endGroup();

    for (short i = 0; i < 25; i++)
    {
        int liczba = i+1;
        QString liczbaconv = QString::number(liczba);
        autorun.beginGroup("Task_" + liczbaconv);
        autorun.setValue("Mode", autoMode[i]);
        autorun.setValue("Force", autoForce[i]);
        autorun.setValue("Repeat", autoRepeat[i]);
        autorun.setValue("Day", ui->Autorun_table->item(i, 1)->text().mid(0, 2));
        autorun.setValue("Month", ui->Autorun_table->item(i, 1)->text().mid(3, 2));
        autorun.setValue("Year", ui->Autorun_table->item(i, 1)->text().mid(6, 4));
        autorun.setValue("Hour", ui->Autorun_table->item(i, 3)->text().mid(0, 2));
        autorun.setValue("Minute", ui->Autorun_table->item(i, 3)->text().mid(3, 2));
        autorun.setValue("Second", ui->Autorun_table->item(i, 3)->text().mid(6, 2));
        autorun.setValue("Monday", autoDays[i][0]);
        autorun.setValue("Tuesday", autoDays[i][1]);
        autorun.setValue("Wednesday", autoDays[i][2]);
        autorun.setValue("Thursday", autoDays[i][3]);
        autorun.setValue("Friday", autoDays[i][4]);
        autorun.setValue("Saturday", autoDays[i][5]);
        autorun.setValue("Sunday", autoDays[i][6]);
        autorun.endGroup();
        autorun.sync();
    }
    changes = 0;
}

void Autorun::autorunLoad(bool first)
{
    if(autoRunned == 0)
    {
        QSettings autorun(appData + "/config/autorun.ini", QSettings::IniFormat);
        autorun.beginGroup("AutourunSettings");
        autorun_hide = autorun.value("Autorun_hide").toBool();
        autorun.endGroup();
    }
    else
    {
        QSettings autorun(PrgrData + "AutoSwitch/autorun.ini", QSettings::IniFormat);
        autorun.beginGroup("AutourunSettings");
        autorun_hide = autorun.value("Autorun_hide").toBool();
        autorun.endGroup();
    }

    if (autorun_hide == 0) ui->AutoRun_visibility_choose->setCurrentIndex(0);
    else if (autorun_hide) ui->AutoRun_visibility_choose->setCurrentIndex(1);

    short number = 0;

    QString ar;
    if (autoRunned == 0 && autorunGlobal == 0) ar = appData + "/config/autorun.ini";
    else ar = PrgrData + "AutoSwitch/autorun.ini";

    for (short i = 0; i < 25; i++)
    {
        struct Task task;
        task.taskNumber = number;

        int numberToLoad = i+1;
        QSettings autorun(ar, QSettings::IniFormat);
        autorun.beginGroup("Task_" + QString::number(numberToLoad));
        int mode = autorun.value("Mode").toInt();

        switch (mode)
        {
        case 1: task.mode = 'w'; break;
        case 2: task.mode = 'r'; break;
        case 3: task.mode = 'u'; break;
        case 4: task.mode = 'h'; break;
        case 5: task.mode = 'l'; break;
        case 6: task.mode = 'b'; break;
        case 7: task.mode = 'm'; break;
        }

        task.force = autorun.value("Force").toBool();
        task.repeatability = autorun.value("Repeat").toBool();
        QDate d(autorun.value("Year").toInt(),
                autorun.value("Month").toInt(),
                autorun.value("Day").toInt());
        QTime t(autorun.value("Hour").toInt(),
                autorun.value("Minute").toInt(),
                autorun.value("Second").toInt());
        task.time.dt = QDateTime(d, t);
        task.time.monday = autorun.value("Monday").toBool();
        task.time.tuesday = autorun.value("Tuesday").toBool();
        task.time.wednesday = autorun.value("Wednesday").toBool();
        task.time.thursday = autorun.value("Thursday").toBool();
        task.time.friday = autorun.value("Friday").toBool();
        task.time.saturday = autorun.value("Saturday").toBool();
        task.time.sunday = autorun.value("Sunday").toBool();
        autorun.endGroup();

        if(mode != 0)
        {
            addAutoTask(first, task);
            number++;
        }
    }
    changes = 0;
}

void Autorun::addAutoTask(bool first, Task task)
{
    if(task.mode >= 1 && task.mode <= 7)
    {
        QString c1, c2, c3, c4, temp, n2s_temp;

        autoMode[task.taskNumber] = task.mode;
        autoForce[task.taskNumber] = task.force;

        if (first)
        {
            preAutomat(task);
        }
        else changes = 1;

        if (first == 0)
        {
            if(preAutomat_Error == 0)
            {
                switch (task.mode)
                {
                case 'w': c1 = QObject::tr("Wyłączenie Komputera"); break;
                case 'r': c1 = QObject::tr("Restart Komputera"); break;
                case 'u': c1 = QObject::tr("Uśpienie Komputera"); break;
                case 'h': c1 = QObject::tr("Hibernacja Komputera"); break;
                case 'l': c1 = QObject::tr("Wylogowanie Użytkownika"); break;
                case 'b': c1 = QObject::tr("Zablokowanie Komputera"); break;
                case 'm': c1 = QObject::tr("Wyłączenie Monitora"); break;
                }
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
                c4 = temp;
                temp = "";

                if(task.repeatability == 0)
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
                    c2 = temp;
                    temp = "";

                    c3 = "-";
                }

                if (task.repeatability)
                {
                    autoRepeat[task.taskNumber] = 1;

                    if(task.time.monday) c3 = "1";
                    if(task.time.tuesday && c3.length() != 0) c3 += ", 2";
                    else if(task.time.tuesday && c3.length() == 0) c3 = "2";
                    if(task.time.wednesday && c3.length() != 0) c3 += ", 3";
                    else if(task.time.wednesday && c3.length() == 0) c3 = "3";
                    if(task.time.thursday && c3.length() != 0) c3 += ", 4";
                    else if(task.time.thursday && c3.length() == 0) c3 = "4";
                    if(task.time.friday && c3.length() != 0) c3 += ", 5";
                    else if(task.time.friday && c3.length() == 0) c3 = "5";
                    if(task.time.saturday && c3.length() != 0) c3 += ", 6";
                    else if(task.time.saturday && c3.length() == 0) c3 = "6";
                    if(task.time.sunday && c3.length() != 0) c3 += ", 7";
                    else if(task.time.sunday && c3.length() == 0) c3 = "7";

                    if(task.time.monday) autoDays[task.taskNumber][0] = 1;
                    if(task.time.tuesday) autoDays[task.taskNumber][1] = 1;
                    if(task.time.wednesday) autoDays[task.taskNumber][2] = 1;
                    if(task.time.thursday) autoDays[task.taskNumber][3] = 1;
                    if(task.time.friday) autoDays[task.taskNumber][4] = 1;
                    if(task.time.saturday) autoDays[task.taskNumber][5] = 1;
                    if(task.time.sunday) autoDays[task.taskNumber][6] = 1;

                    c2 = "-";
                }
            }

            ui->Autorun_table->item(task.taskNumber, 0)->setText(c1);
            ui->Autorun_table->item(task.taskNumber, 1)->setText(c2);
            ui->Autorun_table->item(task.taskNumber, 2)->setText(c3);
            ui->Autorun_table->item(task.taskNumber, 3)->setText(c4);

            ui->Autorun_table->viewport()->update();

            autoState[task.taskNumber] = 1;
        }
    }
}

void Autorun::on_Autorun_table_cellClicked(int row, int column)
{
    if(column == 4 && autoState[row])
    {
        QString zadanie = ui->Autorun_table->item(row, 0)->text();
        QString data = ui->Autorun_table->item(row, 1)->text();
        QString dni = ui->Autorun_table->item(row, 2)->text();
        QString godzina = ui->Autorun_table->item(row, 3)->text();

        QMessageBox::StandardButton reply;
        if(data.length() > 1)
        {
            reply = QMessageBox::question(this, tr("Uwaga!"), tr("Czy jesteś pewny, że chcesz anulować zadanie: ") + zadanie + " " + data + tr(" o godzinie ") + godzina);
            if (reply == QMessageBox::Yes)
            {
                changes = 1;
                autoMode[row] = 0;
                autoForce[row] = 0;
                autoState[row] = 0;
                for(short i = 0; i < 7; i++)
                {
                    autoDays[row][i] = 0;
                }
                autoRepeat[row] = 0;

                ui->Autorun_table->item(row, 0)->setText("");
                ui->Autorun_table->item(row, 1)->setText("");
                ui->Autorun_table->item(row, 2)->setText("");
                ui->Autorun_table->item(row, 3)->setText("");

                ui->Autorun_table->viewport()->update();
            }
        }
        else if(data.length() == 1)
        {
            reply = QMessageBox::question(this, tr("Uwaga!"), tr("Czy jesteś pewny, że chcesz anulować zadanie powtarzające się: ") + zadanie + tr(" zawsze w dni: ") + dni + tr(" o godzinie ") + godzina);
            if (reply == QMessageBox::Yes)
            {
                changes = 1;
                autoMode[row] = 0;
                autoForce[row] = 0;
                autoState[row] = 0;
                for(short i = 0; i < 7; i++)
                {
                    autoDays[row][i] = 0;
                }
                autoRepeat[row] = 0;

                ui->Autorun_table->item(row, 0)->setText("");
                ui->Autorun_table->item(row, 1)->setText("");
                ui->Autorun_table->item(row, 2)->setText("");
                ui->Autorun_table->item(row, 3)->setText("");

                ui->Autorun_table->viewport()->update();
            }
        }
    }
}

void Autorun::on_AutoRun_CancelAll_clicked()
{
    bool if_active = 0;

    for (short i = 0; i < 24; i++)
    {
        if (autoState[i])
        {
            if_active = 1;
            break;
        }
    }

    if (if_active == 0)
    {
        QMessageBox::warning(this, "Błąd!", "Brak zaplanowanych zadań!");
    }
    else
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("Uwaga!"), tr("Czy jesteś pewny, że chcesz anulować WSZYSTKIE ZADANIA AUTOURUCHOMIENIA?"));
        if (reply == QMessageBox::Yes)
        {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, tr("Ostatnia szansa!"), tr("Definitywnie? Tej operacji NIE można cofnąć!"));
            if (reply == QMessageBox::Yes)
            {
                changes = 1;
                for(int i = 0; i < 25; i++)
                {
                    autoMode[i] = 0;
                    autoForce[i] = 0;
                    autoState[i] = 0;
                    for(short x = 0; x < 7; x++)
                    {
                        autoDays[i][x] = 0;
                    }
                    autoRepeat[i] = 0;

                    ui->Autorun_table->item(i, 0)->setText("");
                    ui->Autorun_table->item(i, 1)->setText("");
                    ui->Autorun_table->item(i, 2)->setText("");
                    ui->Autorun_table->item(i, 3)->setText("");
                }
                ui->Autorun_table->viewport()->update();
            }
        }
        changes = 0;
    }
}

void Autorun::on_AutoRun_boxAdd_when_ChooseHour_pressed()
{
    if(ui->AutoRun_boxAdd_dodaj->isEnabled() == false)
    {
        ui->AutoRun_boxAdd_dodaj->setEnabled(true);
    }
}

void Autorun::on_AutoRun_boxAdd_when_ChooseDT_pressed()
{
    if(ui->AutoRun_boxAdd_dodaj->isEnabled() == false)
    {
        ui->AutoRun_boxAdd_dodaj->setEnabled(true);
    }
}

void Autorun::on_AutoRun_boxAdd_when_ChooseTR_pressed()
{
    if(ui->AutoRun_boxAdd_dodaj->isEnabled() == false)
    {
        ui->AutoRun_boxAdd_dodaj->setEnabled(true);
    }
}

short Autorun::determineNumber()
{
    short number = 255;
    for(short i = 0; i < 25; i++)
    {
        if(autoState[i] == 0)
        {
            number = i;
            return number;
        }
    }
    return number;
}

void Autorun::on_AutoRun_boxAdd_dodaj_clicked()
{
    ui->AutoRun_boxAdd_dodaj->setEnabled(false);
    struct Task task;

    task.taskNumber = determineNumber();
    if(task.taskNumber == 255)
    {
        QMessageBox::warning(this, tr("Uwaga!"), tr("Nie można dodać nowego zagania, gdyż aktualnie jest już zaplanowane 25 zadań autouruchamiania! Aby dodać więcej zadań anuluj któreś zadanie z listy!"));
    }
    else
    {
        changes = 1;
        int mode = ui->AutoRun_boxAdd_ChooseMode->currentIndex() + 1;

        switch (mode)
        {
        case 1: task.mode = 'w'; break;
        case 2: task.mode = 'r'; break;
        case 3: task.mode = 'u'; break;
        case 4: task.mode = 'h'; break;
        case 5: task.mode = 'l'; break;
        case 6: task.mode = 'b'; break;
        case 7: task.mode = 'm'; break;
        }

        short wybor;
        if(ui->AutoRun_boxAdd_when_ChooseHour->isChecked()) wybor = 1;
        else if(ui->AutoRun_boxAdd_when_ChooseDT->isChecked()) wybor = 2;
        else if(ui->AutoRun_boxAdd_when_ChooseTR->isChecked())
        {
            wybor = 3;
            task.repeatability = 1;
        }

        ui->AutoRun_boxAdd_when_ChooseHour->setAutoExclusive(false);
        ui->AutoRun_boxAdd_when_ChooseDT->setAutoExclusive(false);
        ui->AutoRun_boxAdd_when_ChooseTR->setAutoExclusive(false);
        ui->AutoRun_boxAdd_when_ChooseHour->setChecked(false);
        ui->AutoRun_boxAdd_when_ChooseDT->setChecked(false);
        ui->AutoRun_boxAdd_when_ChooseTR->setChecked(false);
        ui->AutoRun_boxAdd_when_ChooseHour->setAutoExclusive(true);
        ui->AutoRun_boxAdd_when_ChooseDT->setAutoExclusive(true);
        ui->AutoRun_boxAdd_when_ChooseTR->setAutoExclusive(true);

        task.force = ui->Automat_ForceClose->isChecked();

        if (wybor == 1)
        {
            task.time.dt.setTime(QTime(ui->AutoRun_boxAdd_when_ChooseHour_hour->time().hour(),
                                       ui->AutoRun_boxAdd_when_ChooseHour_hour->time().minute(),
                                       ui->AutoRun_boxAdd_when_ChooseHour_hour->time().second()));
            task.time.dt.setDate(QDate::currentDate());

            addAutoTask(0, task);
        }
        if (wybor == 2)
        {
            task.time.dt.setTime(QTime(ui->AutoRun_boxAdd_when_ChooseDT_hour->time().hour(),
                                 ui->AutoRun_boxAdd_when_ChooseDT_hour->time().minute(),
                                 ui->AutoRun_boxAdd_when_ChooseDT_hour->time().second()));

            task.time.dt.setDate(QDate(ui->AutoRun_boxAdd_when_ChooseDT_hour->date().year(),
                                       ui->AutoRun_boxAdd_when_ChooseDT_hour->date().month(),
                                       ui->AutoRun_boxAdd_when_ChooseDT_hour->date().day()));

            addAutoTask(0, task);
        }
        if (wybor == 3)
        {
            task.time.dt.setTime(QTime(ui->AutoRun_boxAdd_when_ChooseTR_hour->time().hour(),
                                       ui->AutoRun_boxAdd_when_ChooseTR_hour->time().minute(),
                                       ui->AutoRun_boxAdd_when_ChooseTR_hour->time().second()));

            if(ui->AutoRun_boxAdd_when_ChooseTR_Mon->isChecked()) task.time.monday = 1;
            if(ui->AutoRun_boxAdd_when_ChooseTR_Tue->isChecked()) task.time.tuesday = 1;
            if(ui->AutoRun_boxAdd_when_ChooseTR_Wed->isChecked()) task.time.wednesday = 1;
            if(ui->AutoRun_boxAdd_when_ChooseTR_Thu->isChecked()) task.time.thursday = 1;
            if(ui->AutoRun_boxAdd_when_ChooseTR_Fri->isChecked()) task.time.friday = 1;
            if(ui->AutoRun_boxAdd_when_ChooseTR_Sat->isChecked()) task.time.saturday = 1;
            if(ui->AutoRun_boxAdd_when_ChooseTR_Sun->isChecked()) task.time.sunday = 1;

            if (task.time.monday == 0 && task.time.tuesday == 0 && task.time.wednesday == 0 && task.time.thursday == 0 && task.time.friday == 0 && task.time.saturday == 0 && task.time.sunday == 0)
            {
                QMessageBox::warning(this, tr("Błąd!"), tr("Muszisz wybrać przynajmniej jeden dzień, w którym zadanie zostanie uruchomione!"));
            }
            else
            {
                addAutoTask(0, task);
            }
        }
    }
}

void Autorun::on_AutoRun_visibility_choose_currentIndexChanged(int index)
{
    changes = 1;
    if (index == 0) autorun_hide = 0;
    else if (index == 1) autorun_hide = 1;
}

void Autorun::on_Autorun_SaveQuit_clicked()
{
    autorunSave();
    close();
}

void Autorun::on_Autorun_CancelChanges_clicked()
{
    if (changes)
    {
        for(int i = 0; i < 25; i++)
        {
            autoMode[i] = 0;
            autoState[i] = 0;
            for(short x = 0; x < 7; x++)
            {
                autoDays[i][x] = 0;
            }
            autoRepeat[i] = 0;

            ui->Autorun_table->item(i, 0)->setText("");
            ui->Autorun_table->item(i, 1)->setText("");
            ui->Autorun_table->item(i, 2)->setText("");
            ui->Autorun_table->item(i, 3)->setText("");

            ui->Autorun_table->viewport()->update();
        }

        autorunLoad(0);
        changes = 0;
        trayIcon->showMessage(tr("Informacja"), tr("Ustawienia przywrócone!"));
    }
    else trayIcon->showMessage(tr("Informacja"), tr("Nie wprowadzono żadnych zmian!"));
}

void Autorun::on_Autorun_TimeUpd_clicked()
{
    Autorun::timeSync();
}

void Autorun::timeSync()
{
    QDateTime dt(QDateTime::currentDateTime());
    ui->AutoRun_boxAdd_when_ChooseHour_hour->setTime(dt.time());
    ui->AutoRun_boxAdd_when_ChooseTR_hour->setTime(dt.time());
    ui->AutoRun_boxAdd_when_ChooseDT_hour->setDateTime(dt);
}

void Autorun::on_Autorun_ImportSettings_clicked()
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
            for(int i = 0; i < 25; i++)
            {
                autoMode[i] = 0;
                autoState[i] = 0;
                for(short x = 0; x < 7; x++)
                {
                    autoDays[i][x] = 0;
                }
                autoRepeat[i] = 0;

                ui->Autorun_table->item(i, 0)->setText("");
                ui->Autorun_table->item(i, 1)->setText("");
                ui->Autorun_table->item(i, 2)->setText("");
                ui->Autorun_table->item(i, 3)->setText("");
            }
            ui->Autorun_table->viewport()->update();

            QFile::remove(appData + "/config/autorun.ini");
            QFile::copy(importINI, appData + "/config/autorun.ini");
            QFileDevice::Permissions p = QFile(appData + "/config/autorun.ini").permissions();
            if (!(p & QFileDevice::WriteOwner)) QFile(appData + "/config/autorun.ini").setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner);
            autorunLoad(0);
            changes = 0;
            QMessageBox::information(this, tr("Sukces!"), tr("Ustawienia zostały zaimportowane!"));
        }
    }
}

void Autorun::on_Autorun_ExportSettings_clicked()
{
    autorunSave();
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
        QFile::copy(appData + "/config/autorun.ini", exportINI);
        QMessageBox::information(this, tr("Sukces!"), tr("Ustawienia zostały wyeksportowane!"));
    }
}

void Autorun::on_AutoRun_Info_clicked()
{
    InfoSwitch autoinfo;
    autoinfo.setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);
    autoinfo.setModal(true);
    autoinfo.exec();
}

QSettings bootUp("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
void Autorun::on_AutoRun_Enable_clicked()
{
    bool if_active = 0;

    for (short i = 0; i < 24; i++)
    {
        if (autoState[i])
        {
            if_active = 1;
            break;
        }
    }

    if (if_active)
    {
        bootUp.setValue("/AutoSwitch", QDir::toNativeSeparators("\"" + QDir::currentPath() + "/AutoSwitch.exe\"" + " -a"));
        autorunStateUpdater();
    }
    else QMessageBox::warning(nullptr, tr("Błąd!"), tr("Brak zaplanowanych zadań!"));
}

void Autorun::on_AutoRun_Disable_clicked()
{
    bootUp.remove("/AutoSwitch");
    autorunStateUpdater();
}

void Autorun::on_AutoRun_Enable_global_clicked()
{
    if (AutoSwitchHelper("autorun_global enable"))
    {
        autorunStateUpdater();

        if (autorunGlobal)
        {
            autorunSave();
            if (QDir(PrgrData + "AutoSwitch").exists() == false) QDir().mkdir(PrgrData + "AutoSwitch");

            QFile::remove(PrgrData + "AutoSwitch/autorun.ini");
            QFile::copy(appData + "/config/autorun.ini", PrgrData + "AutoSwitch/autorun.ini");
            QFile::remove(PrgrData + "AutoSwitch/settings.ini");
            QFile::copy(appData + "/config/settings.ini", PrgrData + "AutoSwitch/settings.ini");

            QFile(PrgrData + "AutoSwitch/autorun.ini").setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner);
            QFile(PrgrData + "AutoSwitch/settings.ini").setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner);
        }
    }
    else QMessageBox::critical(this, tr("Błąd!"), tr("Nie udało się uruchomic aplikacji pomocniczej!"));
}

void Autorun::on_AutoRun_Disable_global_clicked()
{
    if (AutoSwitchHelper("autorun_global disable"))
    {
        autorunStateUpdater();

        if (autorunGlobal == 0)
        {
            QDir rem = PrgrData + "AutoSwitch";
            rem.removeRecursively();

            for(int i = 0; i < 25; i++)
            {
                autoMode[i] = 0;
                autoState[i] = 0;
                for(short x = 0; x < 7; x++)
                {
                    autoDays[i][x] = 0;
                }
                autoRepeat[i] = 0;

                ui->Autorun_table->item(i, 0)->setText("");
                ui->Autorun_table->item(i, 1)->setText("");
                ui->Autorun_table->item(i, 2)->setText("");
                ui->Autorun_table->item(i, 3)->setText("");
            }
            ui->Autorun_table->viewport()->update();
            autorunLoad(0);
            changes = 0;
        }
    }
    else QMessageBox::critical(this, tr("Błąd!"), tr("Nie udało się uruchomic aplikacji pomocniczej!"));
}

void Autorun::autorunStateUpdater()
{
    QSettings bootUpAdm("HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);

    if (bootUpAdm.contains("/AutoSwitch"))
    {
        autorunGlobal = 1;

        ui->AutoRun_State->setText(tr("Aktywne globalnie"));
        ui->AutoRun_State->setStyleSheet("QLabel { color : green; }");
        ui->AutoRun_Enable_global->setEnabled(0);
        ui->AutoRun_Disable_global->setEnabled(1);
        ui->AutoRun_Enable->setEnabled(0);
        ui->AutoRun_Disable->setEnabled(0);

        ui->AutoRun_boxAddToAutorun->setEnabled(0);
        ui->AutoRun_visibility->setEnabled(0);
        ui->AutoRun_CancelAll->setEnabled(0);
        ui->Autorun_table->setEnabled(0);
        ui->Autorun_ImportSettings->setEnabled(0);
        ui->Autorun_ExportSettings->setEnabled(0);
        ui->Autorun_CancelChanges->setEnabled(0);
    }

    else if (bootUp.contains("/AutoSwitch"))
    {
        autorunGlobal = 0;

        ui->AutoRun_State->setText(tr("Aktywne"));
        ui->AutoRun_State->setStyleSheet("QLabel { color : green; }");
        ui->AutoRun_Enable_global->setEnabled(0);
        ui->AutoRun_Disable_global->setEnabled(0);
        ui->AutoRun_Enable->setEnabled(0);
        ui->AutoRun_Disable->setEnabled(1);

        ui->AutoRun_boxAddToAutorun->setEnabled(0);
        ui->AutoRun_visibility->setEnabled(0);
        ui->AutoRun_CancelAll->setEnabled(0);
        ui->Autorun_table->setEnabled(0);
        ui->Autorun_ImportSettings->setEnabled(0);
        ui->Autorun_ExportSettings->setEnabled(0);
        ui->Autorun_CancelChanges->setEnabled(0);
    }
    else
    {
        autorunGlobal = 0;

        ui->AutoRun_State->setText(tr("Nieaktywne"));
        ui->AutoRun_State->setStyleSheet("QLabel { color : red; }");
        ui->AutoRun_Enable_global->setEnabled(1);
        ui->AutoRun_Disable_global->setEnabled(0);
        ui->AutoRun_Enable->setEnabled(1);
        ui->AutoRun_Disable->setEnabled(0);

        ui->AutoRun_boxAddToAutorun->setEnabled(1);
        ui->AutoRun_visibility->setEnabled(1);
        ui->AutoRun_CancelAll->setEnabled(1);
        ui->Autorun_table->setEnabled(1);
        ui->Autorun_ImportSettings->setEnabled(1);
        ui->Autorun_ExportSettings->setEnabled(1);
        ui->Autorun_CancelChanges->setEnabled(1);
    }
}
