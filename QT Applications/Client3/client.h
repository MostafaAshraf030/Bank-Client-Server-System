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
    void sendJsonRequest(const QJsonObject &jsonObject);
    void handleJsonResponse(const QJsonObject &jsonObject);

private:
    QTcpSocket socket;
    QDataStream outStream;
    QDataStream inStream;
    bool isAdmin;
    bool isUser;
    QString userRole;

    void TransferAmount();
    void MakeTransaction();
    void ViewAccountNumber();
    void GetAccountBalance();
    void ViewTransactionHistory();
    void CreateNewUser();
    void UpdateUser();
    void DeleteUser();
    void printWelcome();
    void login();
    void showAdminMenu();
    void showUserMenu();
    void ReadAllDatabase();
    void processLoginResponse(const QJsonObject &jsonResponse);
    void processAccountNumberResponse(const QJsonObject &jsonResponse);
    void processBalanceResponse(const QJsonObject &jsonResponse);
    void processTransferResponse(const QJsonObject &jsonResponse);
    void processTransactionHistoryResponse(const QJsonObject &jsonResponse);
    void processTransactionResponse(const QJsonObject &jsonResponse);
    void processBankDatabaseResponse(const QJsonObject &jsonResponse);
    void processCreateUserResponse(const QJsonObject &jsonResponse);
    void processUpdateUserResponse(const QJsonObject &jsonResponse);
    void processDeleteUserResponse(const QJsonObject &jsonResponse);


};

#endif // CLIENT_H
