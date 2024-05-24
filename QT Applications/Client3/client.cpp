#include "client.h"
#include <iostream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

CmyClient::CmyClient(QObject *parent) : QObject(parent), isAdmin(false) {
    outStream.setDevice(&socket);
    inStream.setDevice(&socket);
    outStream.setVersion(QDataStream::Qt_6_0);
    inStream.setVersion(QDataStream::Qt_6_0);

    connect(&socket, &QTcpSocket::connected, this, &CmyClient::connected);
    connect(&socket, &QTcpSocket::disconnected, this, &CmyClient::disconnected);
    connect(&socket, &QTcpSocket::stateChanged, this, &CmyClient::stateChanged);
    connect(&socket, &QTcpSocket::readyRead, this, &CmyClient::readyRead);
    connect(&socket, &QTcpSocket::errorOccurred, this, &CmyClient::error);

}

void CmyClient::disconnect() {
    socket.close();
    socket.disconnectFromHost();
    socket.waitForDisconnected();
}

void CmyClient::printWelcome() {
    qInfo() << "Welcome to the banking system!";
}

void CmyClient::login() {
    qInfo() << "Please enter your username:";
    std::string username;
    std::cin >> username;

    qInfo() << "Please enter your password:";
    std::string password;
    std::cin >> password;

    QJsonObject loginRequest;
    loginRequest["type"] = "LogIn";

    QJsonObject dataObject;
    dataObject["username"] = QString::fromStdString(username);
    dataObject["password"] = QString::fromStdString(password);

    loginRequest["data"] = dataObject;

    sendJsonRequest(loginRequest);
}


void CmyClient::sendJsonRequest(const QJsonObject &jsonObject) {
    QJsonDocument jsonDoc(jsonObject);
    QByteArray jsonData = jsonDoc.toJson(QJsonDocument::Compact);

    //qDebug() << "Sending JSON Data:" << jsonData;

    socket.write(jsonData);
    socket.waitForBytesWritten();
}


void CmyClient::processLoginResponse(const QJsonObject &jsonResponse) {
    bool loginSuccess = jsonResponse["authenticated"].toBool();

    if (loginSuccess) {
        qInfo() << "Login successful!";
        userRole = jsonResponse["role"].toString();
        isAdmin = (userRole == "admin"); // Set admin status based on role
        isUser = (userRole == "user");
        if (isAdmin) {
            qInfo() << "You are logged in as an admin.";
            showAdminMenu();
        } else {
            qInfo() << "You are logged in as a regular user.";
            showUserMenu();
        }
    } else {
        qInfo() << "Login failed. Please try again.";
        login();
    }
}


void CmyClient::processAccountNumberResponse(const QJsonObject &jsonResponse) {
    bool success = jsonResponse["success"].toBool();

    if (success) {
        QString accountNumber = jsonResponse["account_number"].toString();
        qInfo() << "Account number:" << accountNumber;
    } else {
        qInfo() << "Failed to retrieve account number. Reason:" << jsonResponse["error"].toString();
    }

    login();

}


void CmyClient::showAdminMenu() {
    qInfo() << "Admin Authorized list:";
    qInfo() << "1. View Account";
    qInfo() << "2. Get Account Balance";
    qInfo() << "3. View Transaction History";
    qInfo() << "4. Read All Database";
    qInfo() << "5. Update User";
    qInfo() << "6. Delete User";
    qInfo() << "7. Create New User";
    qInfo() << "8. Exit";
    qInfo() << "Enter the number of the operation you want to perform:";
    int operation;
    std::cin >> operation;
    performOperation(operation);
}

void CmyClient::showUserMenu() {
    qInfo() << "User Authorized list:";
    qInfo() << "1. View Account Number";
    qInfo() << "2. View Account Balance";
    qInfo() << "3. View Transaction History";
    qInfo() << "4. Make Transaction";
    qInfo() << "5. Transfer Amount";
    qInfo() << "6. Exit";
    qInfo() << "Enter the number of the operation you want to perform:";
    int operation;
    std::cin >> operation;
    performOperation(operation);
}

void CmyClient::performOperation(int operation) {

    if (isUser) {
        switch (operation) {
        case 1:
            ViewAccountNumber();
            break;
        case 2:
            GetAccountBalance();
            break;
        case 3:
            ViewTransactionHistory();
            break;
        case 4:
            MakeTransaction();
            break;
        case 5:
            TransferAmount();
            break;
        case 6:
            disconnected();
            break;
        default:
            qInfo() << "Invalid operation number. Please try again.";
            showUserMenu();
            break;
        }
    } else if (isAdmin) {
        switch (operation) {
        case 1:
            ViewAccountNumber();
            break;
        case 2:
            GetAccountBalance();
            break;
        case 3:
            ViewTransactionHistory();
            break;
        case 4:
            ReadAllDatabase();
            break;
        case 5:
            UpdateUser();
            break;
        case 6:
            DeleteUser();
            break;
        case 7:
            CreateNewUser();
            break;
        case 8:
            disconnected();
            break;
        default:
            qInfo() << "Invalid operation number. Please try again.";
            showAdminMenu();
            break;
        }
    } else {
        qInfo() << "You do not have permission to perform this operation.";
        disconnected();
    }
}

void CmyClient::TransferAmount() {
    qInfo() << "Please enter your account number:";
    std::string fromAccountNumber;
    std::cin >> fromAccountNumber;

    qInfo() << "Please enter the destination account number:";
    std::string toAccountNumber;
    std::cin >> toAccountNumber;

    qInfo() << "Please enter the amount to transfer:";
    double amount;
    std::cin >> amount;

    QJsonObject transferRequest;
    transferRequest["type"] = "TransferAmount";

    QJsonObject dataObject;
    dataObject["fromAccountNumber"] = QString::fromStdString(fromAccountNumber);
    dataObject["toAccountNumber"] = QString::fromStdString(toAccountNumber);
    dataObject["amount"] = amount;

    transferRequest["data"] = dataObject;

    sendJsonRequest(transferRequest);
}

void CmyClient::processTransferResponse(const QJsonObject &jsonResponse) {
    bool transferSuccess = jsonResponse["success"].toBool();

    if (transferSuccess) {
        qInfo() << "Transfer successful!";
    } else {
        qInfo() << "Transfer failed. Reason:" << jsonResponse["error"].toString();
        TransferAmount();
    }

    login();
}

void CmyClient::MakeTransaction() {
    qInfo() << "Please enter your account number:";
    std::string accountNumber;
    std::cin >> accountNumber;

    qInfo() << "Please enter the amount to be transferred:";
    int amount;
    std::cin >> amount;

    QJsonObject transactionRequest;
    transactionRequest["type"] = "MakeTransaction";

    QJsonObject dataObject;
    dataObject["accountNumber"] = QString::fromStdString(accountNumber);
    dataObject["amount"] = amount;

    transactionRequest["data"] = dataObject;

    sendJsonRequest(transactionRequest);
}

void CmyClient::processTransactionResponse(const QJsonObject &jsonResponse) {
    bool transactionSuccess = jsonResponse["success"].toBool();

    if (transactionSuccess) {
        qInfo() << "Transaction successful!";
    } else {
        qInfo() << "Transaction failed. Please try again.";
    }

    login();

}

void CmyClient::ViewAccountNumber() {
    qInfo() << "Please enter the username:";
    std::string username;
    std::cin >> username;

    QJsonObject accountNumberRequest;
    accountNumberRequest["type"] = "ViewAccountNumber";

    QJsonObject dataObject;
    dataObject["username"] = QString::fromStdString(username);

    accountNumberRequest["data"] = dataObject;

    sendJsonRequest(accountNumberRequest);
}


void CmyClient::GetAccountBalance() {
    qInfo() << "Please enter your account number:";
    std::string accountNumber;
    std::cin >> accountNumber;

    QJsonObject balanceRequest;
    balanceRequest["type"] = "GetBalance";

    QJsonObject dataObject;
    dataObject["account_number"] = QString::fromStdString(accountNumber);

    balanceRequest["data"] = dataObject;

    sendJsonRequest(balanceRequest);
}

void CmyClient::processBalanceResponse(const QJsonObject &jsonResponse) {
    bool success = jsonResponse["success"].toBool();

    if (success) {
        double balance = jsonResponse["balance"].toDouble();
        qInfo() << "Your account balance is:" << balance;
    } else {
        qInfo() << "Failed to get account balance. Please try again.";
        GetAccountBalance();
    }

    login();

}

void CmyClient::ViewTransactionHistory() {
    qInfo() << "Please enter your account number:";
    std::string accountNumber;
    std::cin >> accountNumber;

    QJsonObject transactionHistoryRequest;
    transactionHistoryRequest["type"] = "GetTransactionHistory";

    QJsonObject dataObject;
    dataObject["account_number"] = QString::fromStdString(accountNumber);

    transactionHistoryRequest["data"] = dataObject;

    sendJsonRequest(transactionHistoryRequest);
}

void CmyClient::processTransactionHistoryResponse(const QJsonObject &jsonResponse) {
    QJsonArray transactions = jsonResponse["transactions"].toArray();
    qInfo() << "Transaction History:";
    for (const QJsonValue &value : transactions) {
        QJsonObject transaction = value.toObject();
        qInfo() << "Time:" << transaction["transaction_time"].toString()
                << "Amount:" << transaction["transaction_amount"].toDouble();
    }

    login();
}

void CmyClient::ReadAllDatabase() {
    QJsonObject readRequest;
    readRequest["type"] = "ReadAllDatabase";

    sendJsonRequest(readRequest);
}
void CmyClient::processBankDatabaseResponse(const QJsonObject &jsonResponse) {
    bool success = jsonResponse["success"].toBool();

    if (success) {
        qInfo() << "Database data retrieved successfully!";
        QJsonObject data = jsonResponse["data"].toObject();
        //qDebug() << "Database Data:" << QJsonDocument(data).toJson(QJsonDocument::Indented);
    } else {
        qInfo() << "Failed to retrieve database data. Reason:" << jsonResponse["error"].toString();
    }

    login();
}

void CmyClient::CreateNewUser() {
    qInfo() << "Please enter the new user's details.";

    qInfo() << "Username:";
    std::string username;
    std::cin >> username;

    qInfo() << "Password:";
    std::string password;
    std::cin >> password;

    qInfo() << "Account Number:";
    std::string accountNumber;
    std::cin >> accountNumber;

    qInfo() << "Full Name:";
    std::string fullname;
    std::cin.ignore(); // To clear the newline character from the buffer
    std::getline(std::cin, fullname);

    qInfo() << "Initial Balance:";
    double balance;
    std::cin >> balance;

    qInfo() << "Age:";
    int age;
    std::cin >> age;

    qInfo() << "Role (user/admin):";
    std::string role;
    std::cin >> role;

    QJsonObject createUserRequest;
    createUserRequest["type"] = "CreateUser";

    QJsonObject dataObject;
    dataObject["username"] = QString::fromStdString(username);
    dataObject["password"] = QString::fromStdString(password);
    dataObject["account_number"] = QString::fromStdString(accountNumber);
    dataObject["fullname"] = QString::fromStdString(fullname);
    dataObject["balance"] = balance;
    dataObject["age"] = age;
    dataObject["role"] = QString::fromStdString(role);

    createUserRequest["data"] = dataObject;

    sendJsonRequest(createUserRequest);
}


void CmyClient::processCreateUserResponse(const QJsonObject &jsonResponse) {
    bool success = jsonResponse["success"].toBool();

    if (success) {
        qInfo() << "User created successfully!";
    } else {
        qInfo() << "Failed to create user: " << jsonResponse["error"].toString();
    }

    login();
}

/*void CmyClient::processCreateUserRequest(QTcpSocket *socket, const QJsonObject &data) {
    bool creationSuccess = createNewUser(data);

    QJsonObject responseObj;
    responseObj["operation"] = "create_user_response";
    responseObj["created"] = creationSuccess;

    QJsonDocument responseDoc(responseObj);
    QByteArray responseData = responseDoc.toJson();
    qDebug() << "Server sending response:" << responseData;
    socket->write(responseData);
    socket->flush();
    socket->waitForBytesWritten();
    socket->close();
}*/

void CmyClient::UpdateUser() {
    qInfo() << "Enter username to update:";
    std::string username;
    std::cin >> username;

    qInfo() << "Enter new fullname:";
    std::string fullname;
    std::cin >> fullname;

    qInfo() << "Enter new age:";
    int age;
    std::cin >> age;

    QJsonObject updateRequest;
    updateRequest["type"] = "UpdateUser";

    QJsonObject dataObject;
    dataObject["username"] = QString::fromStdString(username);
    dataObject["fullname"] = QString::fromStdString(fullname);
    dataObject["age"] = age;

    updateRequest["data"] = dataObject;

    sendJsonRequest(updateRequest);
}

void CmyClient::processUpdateUserResponse(const QJsonObject &jsonResponse) {
    bool updateSuccess = jsonResponse["updated"].toBool();

    if (updateSuccess) {
        qInfo() << "User updated successfully!";
    } else {
        qInfo() << "Failed to update user. Please try again.";
    }

    login();
}

void CmyClient::DeleteUser() {
    qInfo() << "Please enter the username of the user to delete:";
    std::string username;
    std::cin >> username;

    QJsonObject deleteUserRequest;
    deleteUserRequest["type"] = "DeleteUser";

    QJsonObject dataObject;
    dataObject["username"] = QString::fromStdString(username);

    deleteUserRequest["data"] = dataObject;

    sendJsonRequest(deleteUserRequest);
}

void CmyClient::processDeleteUserResponse(const QJsonObject &jsonResponse) {
    bool deleteSuccess = jsonResponse["deleted"].toBool();

    if (deleteSuccess) {
        qInfo() << "User deletion successful!";
    } else {
        qInfo() << "User deletion failed. Please try again.";
    }

    login();
}

void CmyClient::readyRead() {
    QByteArray data = socket.readAll();
    //qDebug() << "Received Data:" << data;

    QJsonDocument jsonDoc = QJsonDocument::fromJson(data);

    if (jsonDoc.isNull()) {
        qInfo() << "Failed to parse JSON data.";
        return;
    }

    QJsonObject jsonObject = jsonDoc.object();
    handleJsonResponse(jsonObject);
}


void CmyClient::handleJsonResponse(const QJsonObject &jsonObject) {
    QString operation = jsonObject["operation"].toString();

    if (operation == "login_response") {
        processLoginResponse(jsonObject);
    } else if (operation == "account_number_response") {
        processAccountNumberResponse(jsonObject);
    } else if (operation == "balance_response") {
        processBalanceResponse(jsonObject);
    } else if (operation == "transaction_history_response") {
        processTransactionHistoryResponse(jsonObject);
    } else if (operation == "transaction_response") {
        processTransactionResponse(jsonObject);
    } else if (operation == "transfer_response") {
        processTransferResponse(jsonObject);
    } else if (operation == "read_database_response") {
        processBankDatabaseResponse(jsonObject);
    } else if (operation == "create_user_response") {
        processCreateUserResponse(jsonObject);
    } else if (operation == "delete_user_response") {
        processDeleteUserResponse(jsonObject);
    } else if (operation == "update_user_response") {
        processUpdateUserResponse(jsonObject);
    } else {
        qInfo() << "Unhandled JSON response.";
    }
}

void CmyClient::connected() {
    qInfo() << "Connected to server.";
    login();
}

void CmyClient::disconnected() {
    qInfo() << "Disconnected from server.";
}

void CmyClient::stateChanged(QAbstractSocket::SocketState state) {
    qInfo() << "Socket state changed:" << state;
}

void CmyClient::error(QAbstractSocket::SocketError socketError) {
    qWarning() << "Socket error occurred:" << socketError;
}

void CmyClient::connectToHost(const QString &host, quint16 port) {
    socket.connectToHost(host, port);
}
