#include <QCoreApplication>

#include <coapendpoint.h>
#include <coaprequest.h>

#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

CoapMessage m;
m.setPayload("hello");

CoapMessage m2;
m2 = m;
qDebug() << m2.payload();
m2.setPayload("hey");
qDebug() << m.payload() << m2.payload();

    return a.exec();
}
