#include <QCoreApplication>
#include "server.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Server server;

    server.start(QHostAddress("192.168.1.9"), 8080);

    return a.exec();
}
