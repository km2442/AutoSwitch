#include "jsonactionparser.h"

JsonActionParser JsonActionParser::japInstance;

JsonActionParser::JsonActionParser()
{

}

bool JsonActionParser::parseNewAction(QJsonObject jsonObject)
{
    Task task;
    if(jsonObject.value("Action").toString() == "Shutdown") task.mode = 'w';
    else if(jsonObject.value("Action").toString() == "Reboot") task.mode = 'r';
    else if(jsonObject.value("Action").toString() == "Sleep") task.mode = 'u';
    else if(jsonObject.value("Action").toString() == "Hibernate") task.mode = 'h';
    else if(jsonObject.value("Action").toString() == "Logout") task.mode = 'l';
    else if(jsonObject.value("Action").toString() == "Block") task.mode = 'b';
    else if(jsonObject.value("Action").toString() == "MonitorOff") task.mode = 'm';
    else return false;

    if(jsonObject.value("TimeMode").toString() == "Now")
    {
        switch(task.mode)
        {
        case 'w': shutdownComputer(true); break;
        case 'r': rebootComputer(true); break;
        case 'u': sleepComputer(); break;
        case 'h': hibernateComputer(); break;
        case 'l': logoffUser(true); break;
        case 'b': lockComputer(); break;
        case 'm': monitorOff(); break;
        default: return false;
        }
    }
    else if(jsonObject.value("TimeMode").toString() == "Postponed")
    {
        if(jsonObject.contains("dt"))
        {
            QJsonObject dtObj = jsonObject.value("dt").toObject();
            QTime t;
            t.setHMS(dtObj.value("hourOfDay").toInt(), dtObj.value("minute").toInt(), 0);
            task.time.dt.setTime(t);

            if(dtObj.value("year").toInt() != 1900)
            {
                QDate d;
                d.setDate(dtObj.value("year").toInt(), dtObj.value("month").toInt() + 1, dtObj.value("dayOfMonth").toInt());
                task.time.dt.setDate(d);
            }
            else
            {
                QDateTime dt = QDateTime::currentDateTime();
                if(dtObj.value("hourOfDay").toInt() <= dt.time().hour() && dtObj.value("minute").toInt() <= dt.time().minute() && 0 < dt.time().second())
                {
                    task.time = countDate('d', 1, task.time);
                }
                else task.time.dt.setDate(dt.date());
            }

            qDebug() << task.time.dt.date() << task.time.dt.time();
            preAutomat(task);
        }
        else if(jsonObject.contains("seconds"))
        {
            task.time = countTime(jsonObject.value("seconds").toInt(), 's', task.time);
            preAutomat(task);
        }
    }
    else return false;
    return true;
}
