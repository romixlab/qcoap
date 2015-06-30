#include <QCoreApplication>
#include <coapendpoint.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    CoapEndpoint endpoint;

    return a.exec();
}
