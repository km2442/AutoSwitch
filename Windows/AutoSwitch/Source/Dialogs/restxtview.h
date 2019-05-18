#pragma once
#define RESTXTVIEW_H

#include <QDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QTimer>

namespace Ui {
class ResTxtView;
}

class ResTxtView : public QDialog
{
    Q_OBJECT

public:
    explicit ResTxtView(QWidget *parent = 0);
    void loadLicense();
    void loadChangelog();
    ~ResTxtView();

private slots:
    void on_TxtView_CloseButton_clicked();

private:
    Ui::ResTxtView *ui;
    void loadTxt(QString t);
};
