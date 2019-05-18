#pragma once
#define SOCKETCOMMUNICATOR_H

#include <QTcpServer>

#include "Source/Main/mainwindow.h"

#include "Source/Classes/communicatorthread.h"

class SocketCommunicator : public QTcpServer
{
public:
    explicit SocketCommunicator(QObject *parent, QMainWindow *mw);
    void StartServer();

signals:

public slots:

protected:
    void incomingConnection(int socketDescriptor);
    QMainWindow *mw = nullptr;
};
