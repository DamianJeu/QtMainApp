#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>


class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(const QString &host, quint16 port, QObject *parent = nullptr);
    explicit Client(QObject *parent = nullptr);

    QHostAddress host() const;
    void setHost(const QHostAddress &newHost);

    bool isConnected() const;

    quint16 port() const;
    void setPort(quint16 newPort);

signals:

    void dataReceived(const QByteArray &data);
    void disconnectedFromServer();

public slots:


    void connectToServer();
    void disconnectFromServer();
    void sendDataToServer(const QByteArray &data);



private slots:

    void readyRead();
    void connected();
    void disconnected();
    void error();
private:


    QTcpSocket socket;
    QHostAddress m_host;
    quint16 m_port;

};

#endif // CLIENT_H
