#include <QCoreApplication>

#include "client.h"

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    CmyClient client;
    client.connectToHost("192.168.1.9", 8080);

    return app.exec();
}
