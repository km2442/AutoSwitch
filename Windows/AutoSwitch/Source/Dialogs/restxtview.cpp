#include "restxtview.h"
#include "ui_restxtview.h"

ResTxtView::ResTxtView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ResTxtView)
{
    //ui->setupUi(this);
}

ResTxtView::~ResTxtView()
{
    delete ui;
}

void ResTxtView::on_TxtView_CloseButton_clicked()
{
    close();
}

void ResTxtView::loadTxt(QString t)
{
    QFile file(t);
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(0, tr("Błąd"), file.errorString());
        QTimer::singleShot(0, this, SLOT(close()));
    }
    else
    {
        ui->setupUi(this);
        QTextStream in(&file);
        QString temp = in.readAll();
        ui->txtView->setText(temp);
    }
}

void ResTxtView::loadLicense()
{
    loadTxt(":/texts/Resources/Texts/LGPLv3.txt");
}

void ResTxtView::loadChangelog()
{
    loadTxt(":/texts/Resources/Texts/changelog.txt");
}
