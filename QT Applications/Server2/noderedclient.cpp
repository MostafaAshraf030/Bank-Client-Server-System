#include "noderedclient.h"
#include <QNetworkAccessManager>

NodeRedClient::NodeRedClient(const QString &hostAddress, quint16 port, QObject *parent)
    : QObject(parent), m_hostAddress(hostAddress), m_port(port), m_socket(new QTcpSocket(this))
{
    connect(m_socket, &QTcpSocket::connected, this, &NodeRedClient::socketConnected);
    connect(m_socket, &QTcpSocket::disconnected, this, &NodeRedClient::socketDisconnected);
    connect(m_socket, &QTcpSocket::readyRead, this, &NodeRedClient::socketReadyRead);
}

void NodeRedClient::connectToServer()
{
    m_socket->connectToHost(m_hostAddress, m_port);
}

void NodeRedClient::disconnectFromServer()
{
    m_socket->disconnectFromHost();
}

bool NodeRedClient::isConnected() const
{
    return m_socket->state() == QAbstractSocket::ConnectedState;
}

QByteArray NodeRedClient::sendRequest(const QByteArray &requestData)
{
    QByteArray responseData;

    if (isConnected())
    {
        m_socket->write(requestData);
        m_socket->waitForBytesWritten();
        m_socket->waitForReadyRead();
        responseData = m_socket->readAll();
    }
    else
    {
        qDebug() << "Not connected to server";
    }

    return responseData;
}

void NodeRedClient::socketConnected()
{
    qDebug() << "Connected to server";
}

void NodeRedClient::socketDisconnected()
{
    qDebug() << "Disconnected from server";
}

void NodeRedClient::socketReadyRead()
{
    qDebug() << "Data received from server:" << m_socket->readAll();
}
