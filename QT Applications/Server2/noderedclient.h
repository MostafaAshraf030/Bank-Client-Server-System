#ifndef NODEREDCLIENT_H
#define NODEREDCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QDebug>

class NodeRedClient : public QObject
{
    Q_OBJECT
public:
    explicit NodeRedClient(const QString &hostAddress, quint16 port, QObject *parent = nullptr);
    void connectToServer();
    void disconnectFromServer();
    bool isConnected() const;
    QByteArray sendRequest(const QByteArray &requestData);

signals:

private slots:
    void socketConnected();
    void socketDisconnected();
    void socketReadyRead();

private:
    QString m_hostAddress;
    quint16 m_port;
    QTcpSocket *m_socket;
};

#endif // NODEREDCLIENT_H
