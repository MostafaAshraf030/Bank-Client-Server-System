#include "client.h"
#include <iostream>


CmyClient::CmyClient(QObject *parent) : QObject(parent) {
    outStream.setDevice(&socket);
    inStream.setDevice(&socket);
    outStream.setVersion(QDataStream::Qt_6_0);
    inStream.setVersion(QDataStream::Qt_6_0);

    connect(&socket, &QTcpSocket::connected, this, &CmyClient::connected);
    connect(&socket, &QTcpSocket::disconnected, this, &CmyClient::disconnected);
    connect(&socket, &QTcpSocket::stateChanged, this, &CmyClient::stateChanged);
    connect(&socket, &QTcpSocket::readyRead, this, &CmyClient::readyRead);
    connect(&socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::errorOccurred), this, &CmyClient::error);

    m_client.connectToHost("192.168.1.9", 8080);
}

void CmyClient::disconnectFromHost() {
    socket.close();
}

void CmyClient::run() {
    printWelcome();
}

void CmyClient::printWelcome() {
    qInfo() << "Welcome!";
    enterUsernameAndPassword();
}

void CmyClient::enterUsernameAndPassword() {
    QTextStream stream(stdin);
    QString username, password;

    // Prompt user to enter username and password
    qInfo() << "Please enter your username:";
    stream >> username;
    qInfo() << "Please enter your password:";
    stream >> password;

    // Construct JSON object
    QJsonObject jsonObject;
    jsonObject["username"] = username;
    jsonObject["password"] = password;

    // Convert JSON object to JSON document
    QJsonDocument jsonDoc(jsonObject);

    // Convert JSON document to QByteArray
    QByteArray jsonData = jsonDoc.toJson(QJsonDocument::Compact);

    // Send JSON data to the server
    socket.write(jsonData);
    socket.flush();
    socket.waitForBytesWritten();
}



void CmyClient::showAdminMenu() {
    qInfo() << "************** Enter your choice ************";
    qInfo() << "\t\t1. View Account";
    qInfo() << "\t\t2. Transfer Amount";
    qInfo() << "\t\t3. Make Transaction";
    qInfo() << "\t\t4. Get Account Number";
    qInfo() << "\t\t5. View Transaction History";
    qInfo() << "\t\t6. Add New Account";
    qInfo() << "\t\t7. Read All Database";
    qInfo() << "\t\t8. Update User";
    qInfo() << "\t\t9. Delete User";
    qInfo() << "\t\t10. Exit";
}

void CmyClient::showUserMenu() {
    qInfo() << "************** Enter your choice ************";
    qInfo() << "\t\t1. View Account";
    qInfo() << "\t\t2. Transfer Amount";
    qInfo() << "\t\t3. Make Transaction";
    qInfo() << "\t\t4. Get Account Number";
    qInfo() << "\t\t5. View Transaction History";
    qInfo() << "\t\t6. Exit";
}

void CmyClient::connected() {
    qInfo() << "Connected to server.";
    qInfo() << "Welcome to the banking system!";
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
    if (socket.isOpen()) disconnectFromHost();
    qInfo() << "Connecting to host" << host << "on port" << port;
    socket.connectToHost(host, port);
}
