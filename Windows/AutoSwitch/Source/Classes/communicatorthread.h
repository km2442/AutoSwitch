#pragma once
#define COMMUNICATORTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include "Source/Main/mainwindow.h"
#include "Source/Classes/jsonactionparser.h"

class CommunicatorThread : public QThread
{
    Q_OBJECT
public:
    explicit CommunicatorThread(int iID, QObject *parent, QMainWindow *mw);
    void run();

signals:
    void error(QTcpSocket::SocketError socketerror);

public slots:
    void readyRead();
    void disconnected();

public slots:

private:
    QTcpSocket *socket;
    int socketDescriptor;
    QMainWindow *mw = nullptr;
};
