#include <QCoreApplication>
#include "client.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Create an instance of the client
    CmyClient client;

    // Connect to the server
    client.connectToHost("192.168.1.9", 8080);

    return a.exec();
}
