#pragma once
#define FUNKCJE

#include "Source/Main/structures.h"

extern struct TaskTime countTime(unsigned long long quantity, char unit, TaskTime time);
extern struct TaskTime countDate(char unit, int quantity, TaskTime time);
extern void automat(Task task);
extern void preAutomat(Task task);
extern void log(char);
extern void addTaskDescription(bool powtarzalnosc, short numer, QString tryb, QString data, QString godzina, QString weekDays);
extern void traymessage(QString msg);
extern void shutdownComputer(bool force);
extern void rebootComputer(bool force);
extern void sleepComputer();
extern void hibernateComputer();
extern void logoffUser(bool force);
extern void monitorOff();
extern void lockComputer();
extern void restartApp();
extern bool isHibernateAvailable();
extern void trayAction(char mode, char unit, unsigned long long quantity);
extern QString qstringToSha1(QString);
extern bool AutoSwitchHelper(QString param);
extern bool authorize();
extern QString currentSystemLanguage();
