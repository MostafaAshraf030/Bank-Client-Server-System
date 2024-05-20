#include "client.h"
#include <iostream>

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

    //connectToHost("192.168.1.9", 80);
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
    outStream << QString::fromStdString(username);

    qInfo() << "Please enter your password:";
    std::string password;
    std::cin >> password;
    outStream << QString::fromStdString(password);
    socket.waitForBytesWritten();
}

void CmyClient::processLoginResponse() {
    bool loginSuccess;
    inStream >> loginSuccess;

    if (loginSuccess) {
        qInfo() << "Login successful!";
        int userRole;
        inStream >> userRole;
        isAdmin = (userRole == 1);
        if (isAdmin) {
            qInfo() << "You are logged in as an admin.";
            showAdminMenu(); // Display admin menu
        } else {
            qInfo() << "You are logged in as a regular user.";
            showUserMenu(); // Display regular user menu
        }
    } else {
        qInfo() << "Login failed. Please try again.";
        login(); // Prompt user to login again
    }
}



void CmyClient::showAdminMenu() {
    qInfo() << "Admin Authorized list:";
    qInfo() << "1. View Account";
    qInfo() << "2. Transfer Amount";
    qInfo() << "3. Make Transaction";
    qInfo() << "4. Get Account Number";
    qInfo() << "5. View Transaction History";
    qInfo() << "6. Add New Account";
    qInfo() << "7. Read All Database";
    qInfo() << "8. Update User";
    qInfo() << "9. Delete User";
    qInfo() << "10. Exit";
    qInfo() << "Enter the number of the operation you want to perform:";
    int operation;
    std::cin >> operation;
    performOperation(operation);
}


void CmyClient::showUserMenu() {
    qInfo() << "User Authorized list:";
    qInfo() << "1. View Account";
    qInfo() << "2. Transfer Amount";
    qInfo() << "3. Make Transaction";
    qInfo() << "4. Get Account Number";
    qInfo() << "5. View Transaction History";
    qInfo() << "6. Exit";
    qInfo() << "Enter the number of the operation you want to perform:";
    int operation;
    std::cin >> operation;
    performOperation(operation);
}

void CmyClient::performOperation(int operation) {
    switch (operation) {
    case 1:
        ViewAccount();
        break;
    case 2:
        TransferAmount();
        break;
    case 3:
        MakeTransaction();
        break;
    case 4:
        GetAccNo();
        break;
    case 5:
        ViewTransactionHistory();
        break;
    case 6:
        qInfo() << "Exiting...";
        disconnect();
        break;
    case 7:
        if (isAdmin) {
            ReadAllDatabase();
        } else {
            qInfo() << "You do not have permission to perform this operation.";
        }
        break;
    case 8:
        if (isAdmin) {
            UpdateUser();
        } else {
            qInfo() << "You do not have permission to perform this operation.";
        }
        break;
    case 9:
        if (isAdmin) {
            DeleteUser();
        } else {
            qInfo() << "You do not have permission to perform this operation.";
        }
        break;
    default:
        qInfo() << "Invalid operation number. Please try again.";
        if (isAdmin) {
            showAdminMenu();
        } else {
            showUserMenu();
        }
        break;
    }
}

void CmyClient::TransferAmount() {
    qInfo() << "Transfer Amount:";

    // Prompt user for source account
    qInfo() << "Enter the source account:";
    std::string fromAccount;
    std::cin >> fromAccount;
    outStream << QString::fromStdString(fromAccount);

    // Prompt user for destination account
    qInfo() << "Enter the destination account:";
    std::string toAccount;
    std::cin >> toAccount;
    outStream << QString::fromStdString(toAccount);

    // Prompt user for transfer amount
    qInfo() << "Enter the transfer amount:";
    qint32 transferAmount;
    std::cin >> transferAmount;
    outStream << transferAmount;

    // Wait for the response from the server to view it in the client
    socket.waitForReadyRead();
    QString response;
    inStream >> response;
    qInfo() << "Server response:" << response;
}


void CmyClient::MakeTransaction() {
    qInfo() << "Make Transaction:";

    // Prompt user for the account name
    qInfo() << "Enter the account name:";
    std::string account;
    std::cin >> account;
    outStream << QString::fromStdString(account);

    // Prompt user for the transaction amount
    qInfo() << "Enter the transaction amount:";
    qint32 transactionAmount;
    std::cin >> transactionAmount;
    outStream << transactionAmount;

    // Wait for the response from the server to view it in the client
    socket.waitForReadyRead();
    QString response;
    inStream >> response;
    qInfo() << "Server response:" << response;
}


void CmyClient::ViewAccount() {
    qInfo() << "View Account:";

    // Prompt user for the username
    qInfo() << "Enter your username:";
    std::string username;
    std::cin >> username;
    outStream << QString::fromStdString(username);

    // Wait for the response from the server to view it in the client
    socket.waitForReadyRead();
    QString response;
    inStream >> response;
    qInfo() << "Server response:" << response;
}


void CmyClient::GetAccNo() {
    qInfo() << "Get Account Number:";

    // Prompt user for the account name
    qInfo() << "Enter the account name:";
    std::string accountName;
    std::cin >> accountName;
    outStream << QString::fromStdString(accountName);

    // Wait for the response from the server to view it in the client
    socket.waitForReadyRead();
    QString response;
    inStream >> response;
    qInfo() << "Account number:" << response;
}


void CmyClient::ViewTransactionHistory() {
    qInfo() << "View Transaction History:";

    // Prompt user for the account number
    qInfo() << "Enter the account number:";
    std::string accountNumber;
    std::cin >> accountNumber;

    // Prompt user for the number of transactions
    qInfo() << "Enter the number of transactions:";
    quint32 count;
    std::cin >> count;

    // Send account number and count to the server
    outStream << QString::fromStdString(accountNumber) << count;

    // Wait for the response from the server to view it in the client
    socket.waitForReadyRead();
    QString response;
    inStream >> response;
    qInfo() << "Transaction history:" << response;
}

void CmyClient::ReadAllDatabase() {
    qInfo() << "Read All Database:";

    // Check if the user is an admin
    if (!isAdmin) {
        qInfo() << "You do not have permission to perform this operation.";
        return;
    }

    // Send request to the server to read all database
    const int READ_DATABASE_OPERATION = 7; // Define the operation here
    outStream << READ_DATABASE_OPERATION;
}

void CmyClient::UpdateUser() {
    qInfo() << "Update User:";

    // Prompt user for user data
    qInfo() << "Enter user data:";
    std::string userData;
    std::getline(std::cin, userData); // Read input as string

    // Convert std::string to QString
    QString userDataQString = QString::fromStdString(userData);

    // Send user data to the server
    outStream << userDataQString;

    // Wait for the response from the server to view it in the client
    socket.waitForReadyRead();
    QString response;
    inStream >> response;
    qInfo() << "Server response:" << response;
}


void CmyClient::DeleteUser() {
    qInfo() << "Delete User:";

    // Prompt user for user data
    qInfo() << "Enter user data:";
    std::string userData;
    std::getline(std::cin, userData); // Read input as string

    // Convert std::string to QString
    QString userDataQString = QString::fromStdString(userData);

    // Send user data to the server
    outStream << userDataQString;

    // Wait for the response from the server to view it in the client
    socket.waitForReadyRead();
    QString response;
    inStream >> response;
    qInfo() << "Server response:" << response;
}





void CmyClient::connected() {
    qInfo() << "Connected to server.";
    printWelcome();
    login();
}

void CmyClient::disconnected() {
    qInfo() << "Disconnected from server.";
}

void CmyClient::error(QAbstractSocket::SocketError socketError) {
    qInfo() << "Error: " << socketError << " - " << socket.errorString();
}

void CmyClient::stateChanged(QAbstractSocket::SocketState socketState) {
    qInfo() << "Socket State: " << socketState;
}

void CmyClient::readyRead() {
    QString response;
    inStream >> response;
    qInfo().noquote() << "Server response: " << response;
}

void CmyClient::connectToHost(const QString &host, quint16 port) {
    if (socket.isOpen()) disconnect();
    qInfo() << "Connecting to host" << host << "on port" << port;
    socket.connectToHost(host, port);
}