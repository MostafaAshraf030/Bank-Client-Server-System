#include "server.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QString>
#include <QJsonArray>



//const QString NODE_RED_URL = "http://localhost:1880";

Server::Server(QObject *parent) : QObject(parent), tcpServer(new QTcpServer(this))
{
    connect(tcpServer, &QTcpServer::newConnection, this, &Server::newConnection);
}

Server::~Server()
{
    if (tcpServer->isListening()) {
        tcpServer->close();
        qInfo() << "Server stopped";
    }
    delete tcpServer;
}

void Server::start(const QHostAddress &address, quint16 port)
{
    if (!tcpServer->listen(address, port)) {
        qWarning() << "Failed to start server:" << tcpServer->errorString();
        return;
    }
    qInfo() << "Server listening on" << address.toString() << ":" << port;
}

void Server::quit()
{
    if (tcpServer->isListening()) {
        tcpServer->close();
        qInfo() << "Server stopped";
    }
}

void Server::newConnection()
{
    while (tcpServer->hasPendingConnections()) {
        QTcpSocket *socket = tcpServer->nextPendingConnection();
        if (socket) {
            qInfo() << "New connection from:" << socket->peerAddress().toString() << ":" << socket->peerPort();
            connectedSockets.append(socket);
            connect(socket, &QTcpSocket::disconnected, this, &Server::socketDisconnected);
            connect(socket, &QTcpSocket::readyRead, this, &Server::handleClientData);
        }
    }
}

void Server::handleClientData()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (!socket)
        return;

    QByteArray requestDataByteArray = socket->readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(requestDataByteArray);

    if (jsonDoc.isNull()) {
        qDebug() << "Failed to parse JSON data";
        socket->write("Invalid JSON data");
        socket->flush();
        socket->close();
        return;
    }

    QJsonObject jsonObj = jsonDoc.object();

    QString requestType = jsonObj["type"].toString();
    QJsonObject requestDataObject = jsonObj["data"].toObject();
    QString role;
    QString response;

    if (requestType == "LogIn") {
        QString username = requestDataObject["username"].toString();
        QString password = requestDataObject["password"].toString();
        bool isAuthenticated = checkCredentials(username, password, role);
        response = isAuthenticated ? "true" : "false";
    } else if (requestType == "GetAccountNumber") {
        QString username = requestDataObject["username"].toString();
        QString accountNumber = getAccountNumberByUsername(username);
        response = accountNumber;
    } else if (requestType == "GetAccountNumberAdmin") {
        QString username = requestDataObject["username"].toString();
        bool isAdmin = checkAdminPrivileges(username);
        if (isAdmin) {
            QString accountNumber = getAccountNumberByUsername(username);
            response = accountNumber;
        } else {
            response = "Insufficient privileges";
        }
    } else if (requestType == "ViewAccountBalance") {
        QString accountNumber = requestDataObject["accountNumber"].toString();
        int balance = getAccountBalanceByNumber(accountNumber);
        response = QString::number(balance);
    } else if (requestType == "ViewTransactionHistory") {
        QString accountNumber = requestDataObject["accountNumber"].toString();
        int count = requestDataObject["count"].toInt();
        QString transactionHistory = getTransactionHistoryByNumber(accountNumber, count);
        response = transactionHistory;
    } else if (requestType == "MakeTransaction") {
        QString accountNumber = requestDataObject["accountNumber"].toString();
        int transactionAmount = requestDataObject["amount"].toInt();
        bool success = makeTransaction(accountNumber, transactionAmount);
        response = success ? "true" : "false";
    } else if (requestType == "MakeTransfer") {
        QString fromAccountNumber = requestDataObject["fromAccountNumber"].toString();
        QString toAccountNumber = requestDataObject["toAccountNumber"].toString();
        int transferAmount = requestDataObject["amount"].toInt();
        bool success = makeTransfer(fromAccountNumber, toAccountNumber, transferAmount);
        response = success ? "true" : "false";
    } else if (requestType == "ViewBankDatabase") {
        QString bankDatabase = getBankDatabase();
        response = bankDatabase;
    } else if (requestType == "CreateNewUser") {
        QString userData = requestDataObject["userData"].toString();
        bool success = createNewUser(userData);
        response = success ? "true" : "false";
    } else if (requestType == "DeleteUser") {
        QString userData = requestDataObject["userData"].toString();
        bool success = deleteUser(userData);
        response = success ? "true" : "false";
    } else if (requestType == "UpdateUser") {
        QString userData = requestDataObject["userData"].toString();
        bool success = updateUser(userData);
        response = success ? "true" : "false";
    } else {
        response = "Unrecognized request";
    }

    // Send response
    socket->write(response.toUtf8());
    socket->flush();
    socket->waitForBytesWritten();
    socket->close();
}

/*QString Server::getRoleByUsername(const QString &username)
{
    QNetworkAccessManager manager;
    QNetworkRequest request(QUrl(NODE_RED_URL + "/getRoleByUsername"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Construct JSON request object
    QJsonObject requestBody;
    requestBody["username"] = username;
    QJsonDocument requestDoc(requestBody);

    // Send the request
    QNetworkReply *reply = manager.post(request, requestDoc.toJson());
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    // Process the response
    QString role;
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        role = QJsonDocument::fromJson(responseData).object().value("role").toString();
    } else {
        qDebug() << "Network error:" << reply->errorString();
    }

    reply->deleteLater();
    return role;
}*/



void Server::socketDisconnected()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (!socket)
        return;

    connectedSockets.removeAll(socket);
    socket->deleteLater();
}

void Server::processLoginRequest(QTcpSocket *socket, const QStringList &parts)
{
    if (parts.size() < 3) {
        socket->write("Invalid request format");
        socket->flush();
        socket->close();
        return;
    }

    QString username = parts[1];
    QString password = parts[2];
    QString role; // Variable to store the user's role

    // Call checkCredentials with username, password, and role as parameters
    bool isAuthenticated = checkCredentials(username, password, role);

    // Construct JSON response
    QJsonObject responseObject;
    responseObject["authenticated"] = isAuthenticated;
    responseObject["role"] = role;

    QJsonDocument responseDoc(responseObject);

    // Send the response
    socket->write(responseDoc.toJson());
    socket->flush();
    socket->close();
}


void Server::processAccountNumberRequest(QTcpSocket *socket, const QStringList &parts)
{
    if (parts.size() < 2) {
        socket->write("Invalid request format");
        socket->flush();
        socket->close();
        return;
    }

    QString username = parts[1];
    QString accountNumber = getAccountNumberByUsername(username);
    socket->write(accountNumber.toUtf8());
    socket->flush();
    socket->close();
}

void Server::processAccountBalanceRequest(QTcpSocket *socket, const QStringList &parts)
{
    if (parts.size() < 2) {
        socket->write("Invalid request format");
        socket->flush();
        socket->close();
        return;
    }

    QString accountNumber = parts[1];
    int balance = getAccountBalanceByNumber(accountNumber);
    socket->write(QString::number(balance).toUtf8());
    socket->flush();
    socket->close();
}

void Server::processTransactionHistoryRequest(QTcpSocket *socket, const QStringList &parts)
{
    if (parts.size() < 3) {
        socket->write("Invalid request format");
        socket->flush();
        socket->close();
        return;
    }

    QString accountNumber = parts[1];
    int count = parts[2].toInt();
    QString history = getTransactionHistoryByNumber(accountNumber, count);
    socket->write(history.toUtf8());
    socket->flush();
    socket->close();
}

void Server::processMakeTransactionRequest(QTcpSocket *socket, const QStringList &parts)
{
    if (parts.size() < 3) {
        socket->write("Invalid request format");
        socket->flush();
        socket->close();
        return;
    }

    QString accountNumber = parts[1];
    int amount = parts[2].toInt();
    bool success = makeTransaction(accountNumber, amount);
    socket->write(success ? "true" : "false");
    socket->flush();
    socket->close();
}

void Server::processMakeTransferRequest(QTcpSocket *socket, const QStringList &parts)
{
    if (parts.size() < 4) {
        socket->write("Invalid request format");
        socket->flush();
        socket->close();
        return;
    }

    QString fromAccountNumber = parts[1];
    QString toAccountNumber = parts[2];
    int amount = parts[3].toInt();
    bool success = makeTransfer(fromAccountNumber, toAccountNumber, amount);
    socket->write(success ? "true" : "false");
    socket->flush();
    socket->close();
}

void Server::processBankDatabaseRequest(QTcpSocket *socket, const QStringList &parts)
{
    if (parts.size() < 1) {
        socket->write("Invalid request format");
        socket->flush();
        socket->close();
        return;
    }

    QString bankDatabase = getBankDatabase();
    socket->write(bankDatabase.toUtf8());
    socket->flush();
    socket->close();
}

void Server::processCreateUserRequest(QTcpSocket *socket, const QStringList &parts)
{
    if (parts.size() < 2) {
        socket->write("Invalid request format");
        socket->flush();
        socket->close();
        return;
    }

    QString userData = parts[1];
    bool success = createNewUser(userData);
    socket->write(success ? "true" : "false");
    socket->flush();
    socket->close();
}

void Server::processDeleteUserRequest(QTcpSocket *socket, const QStringList &parts)
{
    if (parts.size() < 2) {
        socket->write("Invalid request format");
        socket->flush();
        socket->close();
        return;
    }

    QString userData = parts[1];
    bool success = deleteUser(userData);
    socket->write(success ? "true" : "false");
    socket->flush();
    socket->close();
}

void Server::processUpdateUserRequest(QTcpSocket *socket, const QStringList &parts)
{
    if (parts.size() < 2) {
        socket->write("Invalid request format");
        socket->flush();
        socket->close();
        return;
    }

    QString userData = parts[1];
    bool success = updateUser(userData);
    socket->write(success ? "true" : "false");
    socket->flush();
    socket->close();
}

void Server::processAdminPrivilegesRequest(QTcpSocket *socket, const QStringList &parts)
{
    if (parts.size() < 2) {
        socket->write("Invalid request format");
        socket->flush();
        socket->close();
        return;
    }

    QString username = parts[1];
    bool isAdmin = checkAdminPrivileges(username);
    socket->write(isAdmin ? "true" : "false");
    socket->flush();
    socket->close();
}


bool Server::checkCredentials(const QString &username, const QString &password, QString &role)
{
    QNetworkAccessManager manager;
    QNetworkRequest request(QUrl(NODE_RED_URL + "/checkCredentials"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Construct JSON request object
    QJsonObject requestBody;
    requestBody["username"] = username;
    requestBody["password"] = password;
    QJsonDocument requestDoc(requestBody);

    // Send the request
    QNetworkReply *reply = manager.post(request, requestDoc.toJson());
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    // Process the response
    bool isAuthenticated = false;
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument responseDoc = QJsonDocument::fromJson(responseData);
        isAuthenticated = responseDoc.object().value("authenticated").toBool();
        role = responseDoc.object().value("role").toString();
    } else {
        qDebug() << "Network error:" << reply->errorString();
    }

    reply->deleteLater();
    return isAuthenticated;
}


QString Server::getAccountNumberByUsername(const QString &username)
{
    QNetworkAccessManager manager;
    QNetworkRequest request(QUrl(NODE_RED_URL + "/getAccountNumberByUsername"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Construct JSON request object
    QJsonObject requestBody;
    requestBody["username"] = username;
    QJsonDocument requestDoc(requestBody);

    // Send the request
    QNetworkReply *reply = manager.post(request, requestDoc.toJson());
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    // Process the response
    QString accountNumber;
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        accountNumber = QJsonDocument::fromJson(responseData).object().value("accountNumber").toString();
    } else {
        qDebug() << "Network error:" << reply->errorString();
    }

    reply->deleteLater();
    return accountNumber;
}


int Server::getAccountBalanceByNumber(const QString &accountNumber)
{
    QNetworkAccessManager manager;
    QNetworkRequest request(QUrl(NODE_RED_URL + "/getAccountBalanceByNumber"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Construct JSON request object
    QJsonObject requestBody;
    requestBody["accountNumber"] = accountNumber;
    QJsonDocument requestDoc(requestBody);

    // Send the request
    QNetworkReply *reply = manager.post(request, requestDoc.toJson());
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    // Process the response
    int balance = -1; // Default value for error
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        balance = QJsonDocument::fromJson(responseData).object().value("balance").toInt();
    } else {
        qDebug() << "Network error:" << reply->errorString();
    }

    reply->deleteLater();
    return balance;
}


QString Server::getTransactionHistoryByNumber(const QString &accountNumber, int count)
{
    QNetworkAccessManager manager;
    QNetworkRequest request(QUrl(NODE_RED_URL + "/getTransactionHistoryByNumber"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Construct JSON request object
    QJsonObject requestBody;
    requestBody["accountNumber"] = accountNumber;
    requestBody["count"] = count;
    QJsonDocument requestDoc(requestBody);

    // Send the request
    QNetworkReply *reply = manager.post(request, requestDoc.toJson());
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    // Process the response
    QString history;
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        history = QJsonDocument::fromJson(responseData).object().value("history").toString();
    } else {
        qDebug() << "Network error:" << reply->errorString();
    }

    reply->deleteLater();
    return history;
}


bool Server::makeTransaction(const QString &accountNumber, int amount)
{
    QNetworkAccessManager manager;
    QNetworkRequest request(QUrl(NODE_RED_URL + "/makeTransaction"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Construct JSON request object
    QJsonObject requestBody;
    requestBody["accountNumber"] = accountNumber;
    requestBody["amount"] = amount;
    QJsonDocument requestDoc(requestBody);

    // Send the request
    QNetworkReply *reply = manager.post(request, requestDoc.toJson());
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    // Process the response
    bool success = false;
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        success = QJsonDocument::fromJson(responseData).object().value("success").toBool();
    } else {
        qDebug() << "Network error:" << reply->errorString();
    }

    reply->deleteLater();
    return success;
}


bool Server::makeTransfer(const QString &fromAccountNumber, const QString &toAccountNumber, int amount)
{
    QNetworkAccessManager manager;
    QNetworkRequest request(QUrl(NODE_RED_URL + "/makeTransfer"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Construct JSON request object
    QJsonObject requestBody;
    requestBody["fromAccountNumber"] = fromAccountNumber;
    requestBody["toAccountNumber"] = toAccountNumber;
    requestBody["amount"] = amount;
    QJsonDocument requestDoc(requestBody);

    // Send the request
    QNetworkReply *reply = manager.post(request, requestDoc.toJson());
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    // Process the response
    bool success = false;
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        success = QJsonDocument::fromJson(responseData).object().value("success").toBool();
    } else {
        qDebug() << "Network error:" << reply->errorString();
    }

    reply->deleteLater();
    return success;
}


QString Server::getBankDatabase()
{
    QNetworkAccessManager manager;
    QNetworkRequest request(QUrl(NODE_RED_URL + "/getBankDatabase"));

    // Send the request
    QNetworkReply *reply = manager.get(request);
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    // Process the response
    QString bankDatabase;
    if (reply->error() == QNetworkReply::NoError) {
        bankDatabase = reply->readAll();
    } else {
        qDebug() << "Network error:" << reply->errorString();
    }

    reply->deleteLater();
    return bankDatabase;
}


bool Server::createNewUser(const QString &userData)
{
    QNetworkAccessManager manager;
    QNetworkRequest request(QUrl(NODE_RED_URL + "/createUser"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Construct JSON data
    QJsonObject jsonObject;
    jsonObject["userData"] = userData;
    QJsonDocument jsonDocument(jsonObject);
    QByteArray jsonData = jsonDocument.toJson();

    // Send the request
    QNetworkReply *reply = manager.post(request, jsonData);
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    // Process the response
    bool success = false;
    if (reply->error() == QNetworkReply::NoError) {
        QString response = reply->readAll();
        success = (response == "true");
    } else {
        qDebug() << "Network error:" << reply->errorString();
    }

    reply->deleteLater();
    return success;
}


bool Server::deleteUser(const QString &userData)
{
    QNetworkAccessManager manager;
    QNetworkRequest request(QUrl(NODE_RED_URL + "/deleteUser"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Construct JSON data
    QJsonObject jsonObject;
    jsonObject["userData"] = userData;
    QJsonDocument jsonDocument(jsonObject);
    QByteArray jsonData = jsonDocument.toJson();

    // Send the request
    QNetworkReply *reply = manager.post(request, jsonData);
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    // Process the response
    bool success = false;
    if (reply->error() == QNetworkReply::NoError) {
        QString response = reply->readAll();
        success = (response == "true");
    } else {
        qDebug() << "Network error:" << reply->errorString();
    }

    reply->deleteLater();
    return success;
}


bool Server::updateUser(const QString &userData)
{
    QNetworkAccessManager manager;
    QNetworkRequest request(QUrl(NODE_RED_URL + "/updateUser"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Construct JSON data
    QJsonObject jsonObject;
    jsonObject["userData"] = userData;
    QJsonDocument jsonDocument(jsonObject);
    QByteArray jsonData = jsonDocument.toJson();

    // Send the request
    QNetworkReply *reply = manager.post(request, jsonData);
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    // Process the response
    bool success = false;
    if (reply->error() == QNetworkReply::NoError) {
        QString response = reply->readAll();
        success = (response == "true");
    } else {
        qDebug() << "Network error:" << reply->errorString();
    }

    reply->deleteLater();
    return success;
}


bool Server::checkAdminPrivileges(const QString &username)
{
    QNetworkAccessManager manager;
    QNetworkRequest request(QUrl(NODE_RED_URL + "/checkAdminPrivileges"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Construct JSON data
    QJsonObject jsonObject;
    jsonObject["username"] = username;
    QJsonDocument jsonDocument(jsonObject);
    QByteArray jsonData = jsonDocument.toJson();

    // Send the request
    QNetworkReply *reply = manager.post(request, jsonData);
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    // Process the response
    bool isAdmin = false;
    if (reply->error() == QNetworkReply::NoError) {
        QString response = reply->readAll();
        isAdmin = (response == "true");
    } else {
        qDebug() << "Network error:" << reply->errorString();
    }

    reply->deleteLater();
    return isAdmin;
}