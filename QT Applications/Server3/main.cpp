#include <QCoreApplication>
#include "server.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Server server;

    server.startServer(QHostAddress("192.168.1.2"), 54321);

    return a.exec();
}
