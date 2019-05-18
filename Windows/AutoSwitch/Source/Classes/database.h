#pragma once
#define DATABASE_H

#include <QString>
#include <QSqlDatabase>
#include <QSettings>
#include <QLocale>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QIODevice>
#include <QTextStream>
#include <QTranslator>

extern QString PrgrData;

class Database
{
public:
    static Database* getInstance() { return &databaseInstance; }
    int lookForUser();
    bool registerUser(bool acceptStats);
    bool updateUser(int number, bool acceptStats);
    bool sendStatistics(QString action);
    bool sendOpinion(int mark, QString opinionType, QString email, QString text);
    void getGUID();

private:
    QString guid;

    static Database databaseInstance;
    bool connect(QSqlDatabase db);
    void disconnect(QSqlDatabase db);
    bool sendData(QString data);
    QString getLanguage();
    QString getCountry();
    void logErrors(QSqlQuery query);
};
