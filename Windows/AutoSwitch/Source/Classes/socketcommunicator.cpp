#include "socketcommunicator.h"

SocketCommunicator::SocketCommunicator(QObject *parent, QMainWindow *mw)
{
    this->mw = mw;
}

void SocketCommunicator::StartServer()
{
    if(!this->listen(QHostAddress::Any,46784))
    {
        qDebug() << "Could not start server";
    }
    else
    {
        qDebug() << "Listening...";
    }
}

void SocketCommunicator::incomingConnection(int socketDescriptor)
{
    qDebug() << socketDescriptor << " Connecting...";
    CommunicatorThread *thread = new CommunicatorThread(socketDescriptor, this, mw);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}
