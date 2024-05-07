#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QList>

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);
    ~Server();

    void start(const QHostAddress &address, quint16 port);
    void quit();

private slots:
    void newConnection();
    void handleClientData();
    void socketDisconnected();

private:
    // Private variables
    const QString NODE_RED_URL = "http://localhost:1880"; // Adjust with your Node-RED server URL
    QTcpServer *tcpServer;
    QList<QTcpSocket*> connectedSockets;

    // Private functions
    void processLoginRequest(QTcpSocket *socket, const QStringList &parts);
    void processAccountNumberRequest(QTcpSocket *socket, const QStringList &parts);
    void processAccountBalanceRequest(QTcpSocket *socket, const QStringList &parts);
    void processTransactionHistoryRequest(QTcpSocket *socket, const QStringList &parts);
    void processMakeTransactionRequest(QTcpSocket *socket, const QStringList &parts);
    void processMakeTransferRequest(QTcpSocket *socket, const QStringList &parts);
    void processBankDatabaseRequest(QTcpSocket *socket, const QStringList &parts);
    void processCreateUserRequest(QTcpSocket *socket, const QStringList &parts);
    void processDeleteUserRequest(QTcpSocket *socket, const QStringList &parts);
    void processUpdateUserRequest(QTcpSocket *socket, const QStringList &parts);
    void processAdminPrivilegesRequest(QTcpSocket *socket, const QStringList &parts);

    // Communication with Node-RED
    bool checkCredentials(const QString &username, const QString &password);
    QString getAccountNumberByUsername(const QString &username);
    int getAccountBalanceByNumber(const QString &accountNumber);
    QString getTransactionHistoryByNumber(const QString &accountNumber, int count);
    bool makeTransaction(const QString &accountNumber, int amount);
    bool makeTransfer(const QString &fromAccountNumber, const QString &toAccountNumber, int amount);
    QString getBankDatabase();
    bool createNewUser(const QString &userData);
    bool deleteUser(const QString &userData);
    bool updateUser(const QString &userData);
    bool checkAdminPrivileges(const QString &username);
};

#endif // SERVER_H
