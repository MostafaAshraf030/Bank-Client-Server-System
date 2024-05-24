#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QList>
#include <QMap>
#include <QThread>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);
    void startServer(const QHostAddress &address, quint16 port);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private slots:
    void DisconnectedHandler(qintptr socketDescriptor);
    void handleClientData();
    void sendErrorResponse(QTcpSocket *socket, const QString &error);

private:
    const QString NODE_RED_URL = "http://localhost:1880";
    QMap<qintptr, QThread*> requestThreads;

    // Private functions
    void processLoginRequest(QTcpSocket *socket, const QJsonObject &data);
    void processAccountNumberRequest(QTcpSocket *socket, const QJsonObject &data);
    void processAccountBalanceRequest(QTcpSocket *socket, const QJsonObject &data);
    void processTransactionHistoryRequest(QTcpSocket *socket, const QJsonObject &data);
    void processMakeTransactionRequest(QTcpSocket *socket, const QJsonObject &data);
    void processMakeTransferRequest(QTcpSocket *socket, const QJsonObject &data);
    void processBankDatabaseRequest(QTcpSocket *socket, const QJsonObject &data);
    void processCreateUserRequest(QTcpSocket *socket, const QJsonObject &data);
    void processDeleteUserRequest(QTcpSocket *socket, const QJsonObject &data);
    void processUpdateUserRequest(QTcpSocket *socket, const QJsonObject &data);
    void processAdminPrivilegesRequest(QTcpSocket *socket, const QJsonObject &data);

    // Communication with Node-RED
    bool checkCredentials(const QString &username, const QString &password, QString &role);
    int getAccountNumberByUsername(const QString &username);
    int getAccountBalanceByNumber(const QString &accountNumber);
    QString getTransactionHistoryByNumber(const QString &accountNumber, int count);
    bool makeTransaction(const QString &accountNumber, int amount);
    bool makeTransfer(const QString &fromAccountNumber, const QString &toAccountNumber, double amount);
    QString getBankDatabase();
    bool createNewUser(const QJsonObject &userData);
    bool deleteUser(const QString &userData);
    bool updateUser(const QString &username, const QString &fullname, int age);
    bool checkAdminPrivileges(const QString &username);
};

#endif // SERVER_H
