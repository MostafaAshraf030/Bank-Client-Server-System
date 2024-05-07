#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QDataStream>


class CmyClient : public QObject
{
    Q_OBJECT
public:
    explicit CmyClient(QObject *parent = nullptr);

    void connectToHost(const QString &host, quint16 port);
    void disconnectFromHost();

signals:

public slots:
    void connected();
    void disconnected();
    void error(QAbstractSocket::SocketError socketError);
    void stateChanged(QAbstractSocket::SocketState socketState);
    void readyRead();
    void run();
    void printWelcome();
    void enterUsernameAndPassword();
    void showAdminMenu();
    void showUserMenu();

private:
    QTcpSocket socket;
    QDataStream outStream;
    QDataStream inStream;
    QTcpSocket m_client;
};

#endif // CLIENT_H
