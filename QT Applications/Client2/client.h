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

signals:

public slots:
    void performOperation(int operation);
    void disconnect();
    void connectToHost(const QString &host, quint16 port);

private slots:
    void connected();
    void disconnected();
    void error(QAbstractSocket::SocketError socketError);
    void stateChanged(QAbstractSocket::SocketState socketState);
    void readyRead();

private:
    QTcpSocket socket;
    QDataStream outStream;
    QDataStream inStream;
    bool isAdmin;

    void TransferAmount();
    void MakeTransaction();
    void ViewAccount();
    void GetAccNo();
    void ViewTransactionHistory();
    void UpdateUser();
    void DeleteUser();
    void printWelcome();
    void login();
    void showAdminMenu();
    void showUserMenu();
    void processLoginResponse();
    void ReadAllDatabase();

};

#endif // CLIENT_H
