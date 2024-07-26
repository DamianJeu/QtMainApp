#include "client.h"


Client::Client(const QString &host, quint16 port, QObject *parent)
    : QObject(parent), m_host(host), m_port(port), socket(this)
{

    connect(&socket, &QTcpSocket::readyRead, this, &Client::readyRead);
    connect(&socket, &QTcpSocket::connected, this, &Client::connected);
    connect(&socket, &QTcpSocket::disconnected, this, &Client::disconnected);
    connect(&socket, &QTcpSocket::errorOccurred, this, &Client::error);

    socket.connectToHost(host, port);
}


Client::Client(QObject *parent)
    : QObject(parent), socket(this)
{
    connect(&socket, &QTcpSocket::readyRead, this, &Client::readyRead);
    connect(&socket, &QTcpSocket::connected, this, &Client::connected);
    connect(&socket, &QTcpSocket::disconnected, this, &Client::disconnected);
    connect(&socket, &QTcpSocket::errorOccurred, this, &Client::error);

}

void Client::connectToServer()
{

    if(socket.state() == QAbstractSocket::ConnectedState || socket.isOpen())
    {

        qDebug() << "Already connected";
        return;
    }

    socket.connectToHost(m_host, m_port);

}

void Client::disconnectFromServer()
{

    if(socket.state() == QAbstractSocket::UnconnectedState)
    {
        qDebug() << "Already disconnected";
        return;
    }


    socket.close();
    socket.waitForDisconnected();


}

void Client::sendDataToServer(const QByteArray &data)
{

    if(socket.state() == QAbstractSocket::UnconnectedState)
    {
        qDebug() << "Not connected";
        return;
    }

    socket.write(data);
    socket.flush();
    socket.waitForBytesWritten();
  //  qDebug() << "Data sent: " << data;

}



void Client::readyRead()
{


    QByteArray data = socket.readAll();
  //  qDebug() << "Data received: " << data;
    //emit signal with data received
    emit dataReceived(data);

}

void Client::connected()
{

    qDebug() << "Connected to server";
}

void Client::disconnected()
{

    qDebug() << "Disconnected from server";

}

void Client::error()
{

    qDebug() << "Error: " << socket.errorString() << "closing socket";

    socket.close();

    if(socket.state() != QAbstractSocket::UnconnectedState)
    {
        socket.waitForDisconnected();
    }

}

quint16 Client::port() const
{
    return m_port;
}

void Client::setPort(quint16 newPort)
{
    m_port = newPort;
}

QHostAddress Client::host() const
{
    return m_host;
}

void Client::setHost(const QHostAddress &newHost)
{
    m_host = newHost;

}
