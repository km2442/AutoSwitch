#include "communicatorthread.h"


CommunicatorThread::CommunicatorThread(int iID, QObject *parent, QMainWindow *mw)
{
    this->socketDescriptor = iID;
    this->mw = mw;
}

void CommunicatorThread::run()
{
    // thread starts here
    qDebug() << socketDescriptor << " Starting thread";
    socket = new QTcpSocket();
    if(!socket->setSocketDescriptor(this->socketDescriptor))
    {
        emit error(socket->error());
        return;
    }

    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()),Qt::DirectConnection);
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()),Qt::DirectConnection);

    qDebug() << socketDescriptor << " Client connected";

    // make this thread a loop
    exec();
}

void CommunicatorThread::readyRead()
{
    QByteArray Data = socket->readAll();

    qDebug() << socketDescriptor << " Data in: " << Data;

    //socket->write(Data);

    QJsonDocument jsonResponse = QJsonDocument::fromJson(Data);
    QJsonObject jsonObject = jsonResponse.object();

    JsonActionParser *jap = JsonActionParser::getInstance();
    if(jsonObject.value("Exec").toString() == "AddTask") jap->parseNewAction(jsonObject);

}

void CommunicatorThread::disconnected()
{
    qDebug() << socketDescriptor << " Disconnected";
    socket->deleteLater();
    exit(0);
}
