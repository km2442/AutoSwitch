#pragma once
#include <QDateTime>

struct TaskTime
{
    QDateTime dt;
    bool monday = 0;
    bool tuesday = 0;
    bool wednesday = 0;
    bool thursday = 0;
    bool friday = 0;
    bool saturday = 0;
    bool sunday = 0;
};

struct Task
{
    char mode;
    bool repeatability = 0;
    struct TaskTime time;
    bool force = 0;
    short taskNumber = 0;
};

//TODO
//struct Tasks
//{
//    char mode;
//    bool repeatability = 0;
//    struct TaskTime time;
//    bool force = 0;
//    short taskNumber = 0;
//};
