#include "opinion.h"
#include "ui_opinion.h"

Opinion::Opinion(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Opinion)
{
    ui->setupUi(this);
    em = ui->Opinion_Email->palette();
}

Opinion::~Opinion()
{
    delete ui;
}

void Opinion::closeEvent(QCloseEvent * event)
{
    if (changes == 1)
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("Uwaga!"), tr("Czy jesteś pewny, że chcesz anulować? Wprowadzone zmiany zostaną utracone!"));
        if (reply == QMessageBox::Yes) event->accept();
        else event->ignore();
    }
    else event->accept();
}

void Opinion::on_Opinion_CancelButton_clicked()
{
   close();
}

bool Opinion::emailValidator(QString email)
{
    QRegExp mailREX("\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}\\b");
    mailREX.setCaseSensitivity(Qt::CaseInsensitive);
    mailREX.setPatternSyntax(QRegExp::RegExp);
    return mailREX.exactMatch(email);
}

void Opinion::on_Opinion_ClearButton_clicked()
{
    ui->Opinion_Mark->setValue(8);
    ui->Opinion_Email->setText("");
    ui->Opinion_Type->setCurrentIndex(0);
    ui->Opinion_Text->setPlainText("");
}

bool Opinion::checkChanges()
{
    if (ui->Opinion_Mark->value() == 8 &&
            ui->Opinion_Email->text().length() == 0 &&
            ui->Opinion_Type->currentIndex() == 0 &&
            ui->Opinion_Text->toPlainText().length() == 0)
    {
        ui->Opinion_ClearButton->setEnabled(0);
        return 0;
    }
    else return 1;
}

void Opinion::on_Opinion_Mark_valueChanged(int value)
{
    if (value != 8)
    {
        changes = 1;
        ui->Opinion_ClearButton->setEnabled(1);
    }
    else if (checkChanges() == 0) changes = 0;
}

void Opinion::on_Opinion_Type_currentIndexChanged(int index)
{
    if (index != 0)
    {
        changes = 1;
        ui->Opinion_ClearButton->setEnabled(1);
    }
    else if (checkChanges() == 0) changes = 0;
}

void Opinion::on_Opinion_Email_textChanged(const QString &arg1)
{
    if (arg1.length() != 0)
    {
        changes = 1;
        ui->Opinion_ClearButton->setEnabled(1);
    }
    else if (checkChanges() == 0) changes = 0;
}

void Opinion::on_Opinion_Text_textChanged()
{
    int counter = ui->Opinion_Text->toPlainText().length();

    ui->Opinion_CharLimit->setText(QString::number(counter) + "/1000");
    if (counter > 0 && counter <= 1000)
    {
        changes = 1;
        ui->Opinion_ClearButton->setEnabled(1);
        ui->Opinion_SendButton->setEnabled(1);
    }
    else
    {
        ui->Opinion_SendButton->setEnabled(0);
        if (checkChanges() == 0) changes = 0;
    }
}

void Opinion::on_Opinion_SendButton_clicked()
{
    if (ui->Opinion_Email->text().length() != 0 && emailValidator(ui->Opinion_Email->text()) == 0)
    {
        QMessageBox::warning(this, tr("Błąd!"), tr("Adres e-mail jest nieprawidłowy!"));
    }
    else
    {
        short mark = ui->Opinion_Mark->value();
        QString type, email, text = ui->Opinion_Text->toPlainText();

        switch (ui->Opinion_Type->currentIndex())
        {
        case 0: type = "Opinion"; break;
        case 1: type = "Bug"; break;
        case 2: type = "NewFunction"; break;
        case 3: type = "Translation"; break;
        }

        if (ui->Opinion_Email->text().length() != 0) email = ui->Opinion_Email->text();

        Database *db = Database::getInstance();
        if (db->sendOpinion(mark, type, email, text))
        {
            QMessageBox::information(this, tr("Sukces!"), tr("Opinia została wysłana!"));
            changes = 0;
            close();
        }
        else QMessageBox::warning(this, tr("Błąd!"), tr("Wystąpił błąd podczas wysyłania opinii. Spróbuj ponownie później."));
    }
}
