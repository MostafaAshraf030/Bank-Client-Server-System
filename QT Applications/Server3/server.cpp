#include "server.h"
#include <QJsonArray>

Server::Server(QObject *parent) : QTcpServer(parent) {}

void Server::startServer(const QHostAddress &address, quint16 port) {
    if (this->listen(address, port)) {
        qDebug() << "Server started on" << address << "port" << port;
    } else {
        qDebug() << "Server failed to start.";
    }
}

void Server::incomingConnection(qintptr socketDescriptor) {
    qDebug() << "Incoming connection from socket descriptor:" << socketDescriptor;

    QTcpSocket *socket = new QTcpSocket();
    if (socket->setSocketDescriptor(socketDescriptor)) {
        connect(socket, &QTcpSocket::readyRead, this, &Server::handleClientData);
        connect(socket, &QTcpSocket::disconnected, [this, socketDescriptor]() { DisconnectedHandler(socketDescriptor); });
    } else {
        delete socket;
    }
}

void Server::DisconnectedHandler(qintptr socketDescriptor) {
    QThread *thread = requestThreads.take(socketDescriptor);
    if (thread) {
        thread->quit();
        thread->wait();
        thread->deleteLater();
    }
}

void Server::handleClientData() {
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (!socket) {
        qDebug() << "Error: sender is not a QTcpSocket";
        return;
    }

    QByteArray requestDataByteArray = socket->readAll();
    qDebug() << "Received Data:" << requestDataByteArray;

    QJsonDocument jsonDoc = QJsonDocument::fromJson(requestDataByteArray);
    if (jsonDoc.isNull()) {
        qDebug() << "Failed to parse JSON data";
        sendErrorResponse(socket, "Invalid JSON data");
        return;
    }

    QJsonObject jsonObj = jsonDoc.object();

    QString requestType = jsonObj["type"].toString();
    QJsonObject requestDataObject = jsonObj["data"].toObject();

    if (requestType.isEmpty()) {
        qDebug() << "Empty request type received";
        sendErrorResponse(socket, "Empty request type");
        return;
    }

    qDebug() << "Received request type:" << requestType;

    // Process request based on type
    if (requestType == "LogIn") {
        processLoginRequest(socket, requestDataObject);
    } else if (requestType == "ViewAccountNumber") {
        processAccountNumberRequest(socket, requestDataObject);
    } else if (requestType == "GetBalance") {
        processAccountBalanceRequest(socket, requestDataObject);
    } else if (requestType == "GetTransactionHistory") {
        processTransactionHistoryRequest(socket, requestDataObject);
    } else if (requestType == "MakeTransaction") {
        processMakeTransactionRequest(socket, requestDataObject);
    } else if (requestType == "TransferAmount") {
        processMakeTransferRequest(socket, requestDataObject);
    } else if (requestType == "ReadAllDatabase") {
        processBankDatabaseRequest(socket, requestDataObject);
    } else if (requestType == "CreateUser") {
        processCreateUserRequest(socket, requestDataObject);
    } else if (requestType == "DeleteUser") {
        processDeleteUserRequest(socket, requestDataObject);
    } else if (requestType == "UpdateUser") {
        processUpdateUserRequest(socket, requestDataObject);
    } else if (requestType == "CheckAdminPrivileges") {
        processAdminPrivilegesRequest(socket, requestDataObject);
    } else {
        qDebug() << "Unrecognized request type:" << requestType;
        sendErrorResponse(socket, "Unrecognized request");
    }
}




void Server::sendErrorResponse(QTcpSocket *socket, const QString &message) {
    QJsonObject responseObj;
    responseObj["operation"] = "error";
    responseObj["message"] = message;

    QJsonDocument responseDoc(responseObj);
    QByteArray responseData = responseDoc.toJson();
    qDebug() << "Sending error response:" << responseData;
    socket->write(responseData);
    socket->flush();
    socket->waitForBytesWritten();
    socket->close();
}

void Server::processLoginRequest(QTcpSocket *socket, const QJsonObject &data) {
    QString username = data["username"].toString();
    QString password = data["password"].toString();
    QString role;
    bool isAuthenticated = checkCredentials(username, password, role);

    QJsonObject responseObj;
    responseObj["operation"] = "login_response";
    responseObj["authenticated"] = isAuthenticated;
    responseObj["role"] = role;

    QJsonDocument responseDoc(responseObj);
    QByteArray responseData = responseDoc.toJson();
    qDebug() << "Server sending response:" << responseData;
    socket->write(responseData);
    socket->flush();
    socket->waitForBytesWritten();
    //socket->close();
}

void Server::processAccountNumberRequest(QTcpSocket *socket, const QJsonObject &data) {
    QString username = data["username"].toString();

    int accountNumber = getAccountNumberByUsername(username);

    QJsonObject responseObj;
    responseObj["operation"] = "account_number_response";

    if (accountNumber != -1) {
        responseObj["success"] = true;
        responseObj["account_number"] = QString::number(accountNumber);
    } else {
        responseObj["success"] = false;
        responseObj["error"] = "Account number not found for the given username.";
    }

    QJsonDocument responseDoc(responseObj);
    QByteArray responseData = responseDoc.toJson();
    qDebug() << "Server sending response:" << responseData;
    socket->write(responseData);
    socket->flush();
    socket->waitForBytesWritten();
    socket->close();
}


void Server::processAccountBalanceRequest(QTcpSocket *socket, const QJsonObject &data) {
    QString accountNumber = data["account_number"].toString();
    int balance = getAccountBalanceByNumber(accountNumber);

    QJsonObject responseObj;
    responseObj["operation"] = "balance_response";
    responseObj["success"] = (balance >= 0);
    if (balance >= 0) {
        responseObj["balance"] = balance;
    }

    QJsonDocument responseDoc(responseObj);
    QByteArray responseData = responseDoc.toJson();
    qDebug() << "Server sending response:" << responseData;
    socket->write(responseData);
    socket->flush();
    socket->waitForBytesWritten();
    socket->close();
}

void Server::processTransactionHistoryRequest(QTcpSocket *socket, const QJsonObject &data) {
    QString accountNumber = data["account_number"].toString();
    QString transactionHistory = getTransactionHistoryByNumber(accountNumber, 10);  // Assuming we fetch last 10 transactions

    QJsonObject responseObj;
    responseObj["operation"] = "transaction_history_response";

    QJsonDocument responseDoc = QJsonDocument::fromJson(transactionHistory.toUtf8());
    responseObj["transactions"] = responseDoc.array();

    QJsonDocument responseJsonDoc(responseObj);
    QByteArray responseData = responseJsonDoc.toJson();
    qDebug() << "Server sending transaction history response:" << responseData;
    socket->write(responseData);
    socket->flush();
    socket->waitForBytesWritten();
    socket->close();
}

void Server::processMakeTransactionRequest(QTcpSocket *socket, const QJsonObject &data) {
    QString accountNumber = data["accountNumber"].toString();
    int amount = data["amount"].toInt();
    bool transactionSuccess = makeTransaction(accountNumber, amount);

    QJsonObject responseObj;
    responseObj["operation"] = "transaction_response";
    responseObj["success"] = transactionSuccess;

    QJsonDocument responseDoc(responseObj);
    QByteArray responseData = responseDoc.toJson();
    qDebug() << "Server sending response:" << responseData;
    socket->write(responseData);
    socket->flush();
    socket->waitForBytesWritten();
    socket->close();
}

void Server::processMakeTransferRequest(QTcpSocket *socket, const QJsonObject &data) {
    QString fromAccountNumber = data["fromAccountNumber"].toString();
    QString toAccountNumber = data["toAccountNumber"].toString();
    double amount = data["amount"].toDouble();

    bool isTransferSuccessful = makeTransfer(fromAccountNumber, toAccountNumber, amount);

    QJsonObject responseObj;
    responseObj["operation"] = "transfer_response";
    responseObj["success"] = isTransferSuccessful;

    if (!isTransferSuccessful) {
        responseObj["error"] = "Transfer failed. Check accounts and balance.";
    }

    QJsonDocument responseDoc(responseObj);
    QByteArray responseData = responseDoc.toJson();
    qDebug() << "Server sending response:" << responseData;
    socket->write(responseData);
    socket->flush();
    socket->waitForBytesWritten();
    socket->close();
}

void Server::processBankDatabaseRequest(QTcpSocket *socket, const QJsonObject &data) {
    QString databaseData = getBankDatabase();

    QJsonObject responseObj;
    responseObj["operation"] = "read_database_response";
    responseObj["success"] = !databaseData.isEmpty();

    if (databaseData.isEmpty()) {
        responseObj["error"] = "Failed to retrieve database data.";
    } else {
        QJsonDocument doc = QJsonDocument::fromJson(databaseData.toUtf8());
        responseObj["data"] = doc.object();
    }

    QJsonDocument responseDoc(responseObj);
    QByteArray responseData = responseDoc.toJson();
    qDebug() << "Server sending response:" << responseData;
    socket->write(responseData);
    socket->flush();
    socket->waitForBytesWritten();
    socket->close();
}





void Server::processCreateUserRequest(QTcpSocket *socket, const QJsonObject &data) {
    bool success = createNewUser(data);
    QString error;

    QJsonObject responseObj;
    responseObj["operation"] = "create_user_response";
    responseObj["success"] = success;

    if (!success) {
        error = "Failed to create user. Possible duplicate username or account number.";
        responseObj["error"] = error;
    }

    QJsonDocument responseDoc(responseObj);
    QByteArray responseData = responseDoc.toJson();
    qDebug() << "Server sending response:" << responseData;
    socket->write(responseData);
    socket->flush();
    socket->waitForBytesWritten();
    socket->close();
}


void Server::processDeleteUserRequest(QTcpSocket *socket, const QJsonObject &data) {
    QString username = data["username"].toString();
    bool isDeleted = deleteUser(username);

    QJsonObject responseObj;
    responseObj["operation"] = "delete_user_response";
    responseObj["deleted"] = isDeleted;

    QJsonDocument responseDoc(responseObj);
    QByteArray responseData = responseDoc.toJson();
    qDebug() << "Server sending response:" << responseData;
    socket->write(responseData);
    socket->flush();
    socket->waitForBytesWritten();
    socket->close();
}

void Server::processUpdateUserRequest(QTcpSocket *socket, const QJsonObject &data) {
    QString username = data["username"].toString();
    QString fullname = data["fullname"].toString();
    int age = data["age"].toInt();

    bool isUpdated = updateUser(username, fullname, age);

    QJsonObject responseObj;
    responseObj["operation"] = "update_user_response";
    responseObj["updated"] = isUpdated;

    QJsonDocument responseDoc(responseObj);
    QByteArray responseData = responseDoc.toJson();
    qDebug() << "Server sending response:" << responseData;
    socket->write(responseData);
    socket->flush();
    socket->waitForBytesWritten();
    socket->close();
}

void Server::processAdminPrivilegesRequest(QTcpSocket *socket, const QJsonObject &data) {
    QString username = data["username"].toString();
    bool isAdmin = checkAdminPrivileges(username);

    QJsonObject responseObj;
    responseObj["isAdmin"] = isAdmin;

    QJsonDocument responseDoc(responseObj);
    socket->write(responseDoc.toJson());
    socket->flush();
    socket->waitForBytesWritten();
    socket->close();
}

bool Server::checkCredentials(const QString &username, const QString &password, QString &role) {
    QNetworkAccessManager manager;
    QNetworkRequest request(QUrl(NODE_RED_URL + "/checkCredentials"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject requestBody;
    requestBody["username"] = username;
    requestBody["password"] = password;
    QJsonDocument requestDoc(requestBody);

    QNetworkReply *reply = manager.post(request, requestDoc.toJson());
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

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

int Server::getAccountNumberByUsername(const QString &username) {
    QNetworkAccessManager manager;
    QNetworkRequest request(QUrl(NODE_RED_URL + "/getAccountNumberByUsername"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject requestBody;
    requestBody["username"] = username;
    QJsonDocument requestDoc(requestBody);

    QNetworkReply *reply = manager.post(request, requestDoc.toJson());
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    int accountNumber = -1;
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument responseDoc = QJsonDocument::fromJson(responseData);
        if (responseDoc.object().value("success").toBool()) {
            accountNumber = responseDoc.object().value("account_number").toInt();
        }
    } else {
        qDebug() << "Network error:" << reply->errorString();
    }

    reply->deleteLater();
    return accountNumber;
}





int Server::getAccountBalanceByNumber(const QString &accountNumber) {
    QNetworkAccessManager manager;
    QNetworkRequest request(QUrl(NODE_RED_URL + "/getBalance"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject requestBody;
    requestBody["account_number"] = accountNumber;
    QJsonDocument requestDoc(requestBody);

    QNetworkReply *reply = manager.post(request, requestDoc.toJson());
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    int balance = -1;  // Default to -1 to indicate failure
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument responseDoc = QJsonDocument::fromJson(responseData);
        balance = responseDoc.object().value("balance").toInt();
    } else {
        qDebug() << "Network error:" << reply->errorString();
    }

    reply->deleteLater();
    return balance;
}

QString Server::getTransactionHistoryByNumber(const QString &accountNumber, int count) {
    QNetworkAccessManager manager;
    QNetworkRequest request(QUrl(NODE_RED_URL + "/getTransactionHistory"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject requestBody;
    requestBody["account_number"] = accountNumber;
    requestBody["count"] = count;
    QJsonDocument requestDoc(requestBody);

    QNetworkReply *reply = manager.post(request, requestDoc.toJson());
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    QString responseData;
    if (reply->error() == QNetworkReply::NoError) {
        responseData = reply->readAll();
    } else {
        qDebug() << "Network error:" << reply->errorString();
    }

    reply->deleteLater();
    return responseData;
}

bool Server::makeTransaction(const QString &accountNumber, int amount) {
    QNetworkAccessManager manager;
    QNetworkRequest request(QUrl(NODE_RED_URL + "/makeTransaction"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject requestBody;
    requestBody["accountNumber"] = accountNumber;
    requestBody["amount"] = amount;
    QJsonDocument requestDoc(requestBody);

    QNetworkReply *reply = manager.post(request, requestDoc.toJson());
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    bool transactionSuccess = false;
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument responseDoc = QJsonDocument::fromJson(responseData);
        transactionSuccess = responseDoc.object().value("success").toBool();
    } else {
        qDebug() << "Network error:" << reply->errorString();
    }

    reply->deleteLater();
    return transactionSuccess;
}

bool Server::makeTransfer(const QString &fromAccountNumber, const QString &toAccountNumber, double amount) {
    QNetworkAccessManager manager;
    QNetworkRequest request(QUrl(NODE_RED_URL + "/makeTransfer"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject requestBody;
    requestBody["fromAccountNumber"] = fromAccountNumber;
    requestBody["toAccountNumber"] = toAccountNumber;
    requestBody["amount"] = amount;
    QJsonDocument requestDoc(requestBody);

    QNetworkReply *reply = manager.post(request, requestDoc.toJson());
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    bool isTransferSuccessful = false;
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument responseDoc = QJsonDocument::fromJson(responseData);
        isTransferSuccessful = responseDoc.object().value("success").toBool();
    } else {
        qDebug() << "Network error:" << reply->errorString();
    }

    reply->deleteLater();
    return isTransferSuccessful;
}

QString Server::getBankDatabase() {
    QNetworkAccessManager manager;
    QNetworkRequest request(QUrl(NODE_RED_URL + "/getDatabase"));
    QNetworkReply *reply = manager.get(request);

    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    QString responseData;
    if (reply->error() == QNetworkReply::NoError) {
        responseData = reply->readAll();
    } else {
        qDebug() << "Network error:" << reply->errorString();
    }

    reply->deleteLater();
    return responseData;
}


bool Server::createNewUser(const QJsonObject &userData) {
    QNetworkAccessManager manager;
    QNetworkRequest request(QUrl(NODE_RED_URL + "/createUser"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonDocument requestDoc(userData);

    QNetworkReply *reply = manager.post(request, requestDoc.toJson());
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    bool success = false;
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument responseDoc = QJsonDocument::fromJson(responseData);
        success = responseDoc.object().value("success").toBool();
    } else {
        qDebug() << "Network error:" << reply->errorString();
    }

    reply->deleteLater();
    return success;
}

bool Server::deleteUser(const QString &username) {
    QNetworkAccessManager manager;
    QNetworkRequest request(QUrl(NODE_RED_URL + "/deleteUser"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject requestBody;
    requestBody["username"] = username;
    QJsonDocument requestDoc(requestBody);

    QNetworkReply *reply = manager.post(request, requestDoc.toJson());
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    bool isDeleted = false;
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument responseDoc = QJsonDocument::fromJson(responseData);
        isDeleted = responseDoc.object().value("deleted").toBool();
    } else {
        qDebug() << "Network error:" << reply->errorString();
    }

    reply->deleteLater();
    return isDeleted;
}

bool Server::updateUser(const QString &username, const QString &fullname, int age) {
    QNetworkAccessManager manager;
    QNetworkRequest request(QUrl(NODE_RED_URL + "/updateUser"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject requestBody;
    requestBody["username"] = username;
    requestBody["fullname"] = fullname;
    requestBody["age"] = age;
    QJsonDocument requestDoc(requestBody);

    QNetworkReply *reply = manager.post(request, requestDoc.toJson());
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    bool isUpdated = false;
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument responseDoc = QJsonDocument::fromJson(responseData);
        isUpdated = responseDoc.object().value("updated").toBool();
    } else {
        qDebug() << "Network error:" << reply->errorString();
    }

    reply->deleteLater();
    return isUpdated;
}

bool Server::checkAdminPrivileges(const QString &username) {
    QNetworkAccessManager manager;
    QNetworkRequest request(QUrl(NODE_RED_URL + "/checkAdminPrivileges"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject requestBody;
    requestBody["username"] = username;
    QJsonDocument requestDoc(requestBody);

    QNetworkReply *reply = manager.post(request, requestDoc.toJson());
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    bool isAdmin = false;
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        isAdmin = QJsonDocument::fromJson(responseData).object().value("isAdmin").toBool();
    } else {
        qDebug() << "Network error:" << reply->errorString();
    }

    reply->deleteLater();
    return isAdmin;
}
