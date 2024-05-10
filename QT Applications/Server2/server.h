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
<<<<<<< HEAD
    bool checkCredentials(const QString &username, const QString &password, QString &role);
=======
    bool checkCredentials(const QString &username, const QString &password);
>>>>>>> 2d72460567a706e0fcec3850d1755ccaf2744a74
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
<<<<<<< HEAD
    QString getRoleByUsername(const QString &username);
=======
>>>>>>> 2d72460567a706e0fcec3850d1755ccaf2744a74
};

#endif // SERVER_H
