#include "database.h"

Database Database::databaseInstance;

//Database::connect() is in gitignore file (sensitive credentials)
void Database::disconnect(QSqlDatabase db)
{
    db.close();
}

void Database::getGUID()
{
    QSettings reg("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Cryptography", QSettings::Registry64Format);
    guid = reg.value("MachineGuid").toString();
}

QString Database::getLanguage()
{
    return QLocale::system().name();
}

QString Database::getCountry()
{
    return QLocale::countryToString(QLocale::system().country());
}

int Database::lookForUser()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", "main");
    QSqlQuery query(db);
    if (connect(db))
    {
        if (query.exec("SELECT id FROM Users WHERE GUID = ('" + guid + "')"))
        {
            if (query.next())
            {
                int nr = query.value(0).toInt();
                disconnect(db);
                return nr;
            }
            else
            {
                disconnect(db);
                return -1;
            }
        }
        else
        {
            disconnect(db);
            return -1;
        }
    }
    else
    {
        logErrors(query);
        return -2;
    }
}

bool Database::registerUser(bool acceptStats)
{
    QString data = "INSERT INTO Users (GUID, Country, Language, Accept_Stats)"
                   "VALUES ('" + guid + "', '" + getCountry() + "', '" + getLanguage() + "', '" + QString::number(acceptStats) + "')";
    return sendData(data);
}

bool Database::updateUser(int number, bool acceptStats)
{
    QString data = "UPDATE `AutoSwitch`.`Users` "
                   "SET `Accept_Stats`='" + QString::number(acceptStats) + "' "
                   "WHERE `id`='" + QString::number(number) + "'";
    return sendData(data);
}

bool Database::sendStatistics(QString action)
{
    QString dt = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString data = "INSERT INTO Actions (GUID, Action, DateTime) "
                   "VALUES ('" + guid + "', '" + action + "', '" + dt + "')";
    return sendData(data);

}

bool Database::sendOpinion(int mark, QString opinionType, QString email, QString text)
{
    QString data = "INSERT INTO Users (GUID, Mark, Type, e-mail, Text) "
                   "VALUES ('" + guid + "', '" + QString::number(mark) + "', '" + opinionType + "', '" + email + "', '" + text + "')";
    return sendData(data);
}

bool Database::sendData(QString data)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", "data");
    QSqlQuery query(db);
    if (connect(db))
    {
        if(query.exec(data))
        {
            disconnect(db);
            return 1;
        }
        else
        {
            logErrors(query);
            disconnect(db);
            return 0;
        }
    }
    else
    {
        logErrors(query);
        return 0;
    }
}

void Database::logErrors(QSqlQuery query)
{
    if (QDir(QDir::toNativeSeparators(PrgrData + "AutoSwitch Debug")).exists() == false) QDir().mkdir(QDir::toNativeSeparators(PrgrData + "AutoSwitch Debug"));

    QString filename="AutoSwitch error debug log.txt";
    QFile log(QDir::toNativeSeparators(PrgrData + "AutoSwitch Debug\\" + filename));
    if ( log.open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text) )
    {
        QTextStream file( &log );
        file.setCodec("UTF-8");
        file << QObject::tr("Data: ").toUtf8() << QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss").toUtf8() << endl;
        file << "LAST QUERY: " << query.lastQuery() << endl;
        file << "ROWS AFFECTED: " << query.numRowsAffected() << endl;
        file << "LAST ERROR: " << query.lastError().text() << endl << endl;
        log.close();
    }
}
