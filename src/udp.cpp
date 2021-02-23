#include "udp.h"
#include <QThread>
#include <iostream>

Udp::Udp()
{
    socket = new QUdpSocket(this);
    socket->bind(QHostAddress("192.168.2.1"), 34952);

    connect(socket, &QUdpSocket::readyRead, this, &Udp::readyRead);
}

void Udp::sendData(QString addr)
{
        char Data[4];
        Data[0] = 0;
        Data[1] = 0x11;
        Data[2] = 0xC6;

        socket->writeDatagram(Data, 4, QHostAddress(addr), 34952);

        qDebug() << "sending...";
}

QString Udp::getFps(void) {
    QString tmpFps = QString::number(fps);
    fps = 0;
    return tmpFps;
}

void Udp::readyRead()
{
    mutex.lock();

    quint16 senderPort;

    int i = 0;

    i=socket->readDatagram(((char *)s), socket->pendingDatagramSize(), &sender, &senderPort);

    if (i==4) {
        if ((s[0]&0xFF00)==0xFF00) {
            progok=true;
            nextframe=s[1];
        } else {
            char *ver=(char *)s;
            fwVersion=(ver[1]<<8)|ver[0];
            hwVersion=(ver[3]<<8)|ver[2];
        }
    } else {
        int lijnnr=s[0];
        i-=2;
        if (lijnnr==0xAAAA) {
            memcpy(laser,&s[1],i);
            emit Udp::hasToRender();

        } else if (lijnnr==0x5555) {
            memcpy(&laser[700],&s[1],i);
        } else {
            if (((lijnnr*1280)+i)<=(1024*1280)) {

                memcpy(&bitmap[lijnnr*1280],&s[1],i);
                if (lijnnr<vorigelijnnr) {
                    emit Udp::hasToRender();
                    fps++;
                }
            }
            vorigelijnnr=lijnnr;
        }
    }
    mutex.unlock();
}

void Udp::chooseCam() {

    Udp::sendData(topaddr);
    // Udp::sendData(leftaddr);
    // Udp::sendData(rightaddr);
}

const char * Udp::getBuffer(void) const {
    return (const char *)&this->bitmap[0];
}

// Blocking
void Udp::getBufferCoppy(char * copy) {

    if (this->mutex.tryLock()) {

        memcpy(copy, &this->bitmap, sizeof(this->bitmap));

        this->mutex.unlock();

    }

}

void Udp::setAddr(QHostAddress addr) {
    this->sender = addr;
}

QString Udp::getAddr() {
    return sender.toString();
}

QString Udp::getPosition(QString addr) {
   if (addr == "192.168.2.2") {
       return "Top camera";
   } else if (addr == "192.168.2.3") {
       return "Left camera";
   } else if (addr == "192.168.2.4") {
       return "Right camera";
   } else {
       return "No camera found";
   }
}

QByteArray Udp::getData() {
    QByteArray data;
    data.resize(socket->pendingDatagramSize());

    QHostAddress sender;
    quint16 senderPort;
    socket->readDatagram(data.data(), data.size(), &sender, &senderPort);

    return data;
    qDebug() << "Message: " << data;
}
