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
    else if(jsonObject.value("Action").toString() == "Lock") task.mode = 'l';
    else if(jsonObject.value("Action").toString() == "Block") task.mode = 'b';
    else if(jsonObject.value("Action").toString() == "MonitorOff") task.mode = 'm';
    else return false;

    if(jsonObject.value("TimeMode").toString() == "Now")
    {
        switch(task.mode)
        {
        case 'm': monitorOff(); break;
        default: return false;
        }
        return true;
    }
    else if(jsonObject.value("TimeMode").toString() == "Seconds")
    {

    }
    else return false;

    return true;
}
