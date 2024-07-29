#include "client.h"

/**
 * @brief Constructor Client class.
 */
Client::Client(const QString &host, quint16 port, QObject *parent)
    : QObject(parent), m_host(host), m_port(port), socket(this)
{

    connect(&socket, &QTcpSocket::readyRead, this, &Client::readyRead);
    connect(&socket, &QTcpSocket::connected, this, &Client::connected);
    connect(&socket, &QTcpSocket::disconnected, this, &Client::disconnected);
    connect(&socket, &QTcpSocket::errorOccurred, this, &Client::error);

    socket.connectToHost(host, port);
}

/**
 * @brief Constructor Client class.
 */
Client::Client(QObject *parent)
    : QObject(parent), socket(this)
{
    connect(&socket, &QTcpSocket::readyRead, this, &Client::readyRead);
    connect(&socket, &QTcpSocket::connected, this, &Client::connected);
    connect(&socket, &QTcpSocket::disconnected, this, &Client::disconnected);
    connect(&socket, &QTcpSocket::errorOccurred, this, &Client::error);

}

/**
 * @brief Deconstructor Client class.
 */
void Client::connectToServer()
{

    if(socket.state() == QAbstractSocket::ConnectedState || socket.isOpen())
    {
        qDebug() << "Already connected";
        return;
    }

    socket.connectToHost(m_host, m_port);

}

/**
 * @brief Disconnects from server.
 */
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

/**
 * @brief Sends data to server.
 *
 * @param QByteArray data Data to send
 */
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
}

/**
 * @brief Reads data from server.
 */
void Client::readyRead()
{
    QByteArray data = socket.readAll();
    emit dataReceived(data);
}

/**
 * @brief Connected to server.
 */
void Client::connected()
{
    qDebug() << "Connected to server";
    emit connectedToServer();
}

/**
 * @brief Disconnected from server.
 */
void Client::disconnected()
{
    qDebug() << "Disconnected from server";
    emit disconnectedFromServer();
}

/**
 * @brief Error.
 */
void Client::error()
{
    qDebug() << "Error: " << socket.errorString() << "closing socket";

    socket.close();

    if(socket.state() != QAbstractSocket::UnconnectedState)
    {
        socket.waitForDisconnected();
    }

    emit errorSignal(socket.errorString());
}

/**
 * @brief Returns port.
 * @return quint16 Port
 */
quint16 Client::port() const
{
    return m_port;
}

/**
 * @brief Sets port.
 *
 * @param quint16 newPort New port
 */
void Client::setPort(quint16 newPort)
{
    m_port = newPort;
}

/**
 * @brief Returns host.
 * @return QHostAddress Host
 */
QHostAddress Client::host() const
{
    return m_host;
}

/**
 * @brief Sets host.
 *
 * @param QHostAddress newHost New host
 */
void Client::setHost(const QHostAddress &newHost)
{
    m_host = newHost;

}

/**
 * @brief Returns true if connected, false otherwise.
 * @return bool true if connected, false otherwise
 */
bool Client::isConnected() const
{
    return (socket.state() == QAbstractSocket::ConnectedState)? true : false;
}
