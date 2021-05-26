#include "udp.h"
#include <QThread>
#include <iostream>

#include <windows.h>

Udp::Udp()
{
    socket = new QUdpSocket(this);
    socket->bind(QHostAddress("192.168.2.1"), 34952);

    connect(socket, &QUdpSocket::readyRead, this, &Udp::readyRead);

    this->fpsCounter = new QTimer(this);

    connect(this->fpsCounter, &QTimer::timeout, this, [&]() {

        this->fps = this->counter;
        this->counter = 0;

    });

    this->fpsCounter->start(1000);

    this->deltaCounter = new QTimer(this);

    connect(this->deltaCounter, &QTimer::timeout, this, [&]() {

        this->dCounter = this->delta;

    });

    this->deltaCounter->start(10);

}

Udp::~Udp() {



}

void Udp::sendDataImg(QString addr)
{
    writeReg(0xD,1);	// reset camera
    writeReg(0xD,0);
    writeReg(7,3);	// don't execute changes

        char Data[4];
        Data[0] = 0;
        Data[1] = 0x11;
        Data[2] = 0xC6;
        Data[3] = 0xC0;

        socket->writeDatagram(Data, 4, QHostAddress(addr), 34952);

        writeReg(0x11,0xC6);

    writeReg(7,2);  // execute changes
}

void Udp::sendDataLaser(QString addr) {

    writeReg(0xD,1);	// reset camera
    writeReg(0xD,0);
    writeReg(7,3);	// don't execute changes

        char Data[4];
        Data[0] = 0;
        Data[1] = 0x11;
        Data[2] = 0x61;
        Data[3] = 0x60;

        socket->writeDatagram(Data, 4, QHostAddress(addr), 34952);

    writeReg(7,2);  // execute changes
}

int Udp::getFps(void) {
    return this->fps;
}

int Udp::getDelta(void) {
    return this->dCounter;
}

void Udp::readyRead()
{
    mutex.lock();

    quint16 senderPort;

    int i = 0;

    i=socket->readDatagram(((char *)s), socket->pendingDatagramSize(), &sender, &senderPort);

    timeStamp();

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
//                memcpy(&bitmap[lijnnr*1280],&s[1],i);

                int index = ((getTopleftY() + lijnnr) * 1280) + getTopleftX();

                memcpy(&bitmap[index], &s[1], i);

//                memcpy(&bitmap[index], &bitmap[index + (getBottomrightX()- getTopleftX())], 255);

                if (lijnnr<vorigelijnnr) {
                    emit Udp::hasToRender();
                    this->counter++;

                    this->curTime = QTime::currentTime().toString("zzz");

                    if((this->curTime.toInt() - this->prevTime.toInt()) < 0) {
                        this->delta = 1000 - this->prevTime.toInt() + this->curTime.toInt();
                    } else {
                        this->delta = this->curTime.toInt() - this->prevTime.toInt();
                    }

//                    qDebug() << delta;
                    this->prevTime = this->curTime;
                }
            }
            vorigelijnnr=lijnnr;
        }
    }
    mutex.unlock();
}

void Udp::chooseCam() {
//    this->sendDataImg(topaddr);
//    this->sendDataImg(leftaddr);
//    this->sendDataImg(rightaddr);

    setRoi(0, 1280, 0, 1024,1,1,6,0, Udp::SEND_PICTURE|Udp::SEND_LASER);
}

const char * Udp::getBuffer(void) const {
    return (const char *)&this->bitmap[0];
}

const char * Udp::getLaser(void) const {
    return (const char *)&this->laser[0];
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

void Udp::writeReg(int address, unsigned short data) {

    char s[4];
    s[0]=1;
    s[1]=address;
    s[2]=data&255;
    s[3]=data>>8;

    int teller=0;
    do {

        socket->writeDatagram(s, 4, QHostAddress(getAddr()), 34952);

        // timeout
        Sleep(20);

        teller++;
        if (teller==5) {
            geenAntwoord=true;	// om compatible te zijn met eerste versie camera's die geen antwoord terug sturen
        }

    } while ((!geenAntwoord));
}

void Udp::setRoi(int xstart, int xsize, int ystart, int ysize, int xinc, int yinc, int interl, int gain, int mode) {
    writeReg(0xD,1);	// reset camera
    writeReg(0xD,1);
    writeReg(0xD,1);
    writeReg(0xD,1);
    writeReg(0xD,1);
    writeReg(0xD,0);
    writeReg(7,3);	// don't execute changes
    writeReg(1,(unsigned int)ystart+12);
    writeReg(2,(unsigned int)(xstart+20)&0xfffe);
    writeReg(3,(unsigned int)(ysize-1));
    writeReg(4,(unsigned int)(xsize-1)|1);

    writeReg(9,(unsigned int)ysize+25); // 25 van vertical blanking
    setGain(gain);

    writeReg(7,2);  // execute changes

    if (threshold<0)
        threshold=0;

    if (threshold>1023)
        threshold=1023;

    char s[5];
    s[0]=0;
    s[1]=(yinc<<4) | (xinc&15);
    s[2]=(mode<<4) | (interl&15);
    s[3]=mode>>4;

    int teller=0;
    do {

        socket->writeDatagram(s, 4, QHostAddress("192.168.2.2"), 34952);

        teller++;
        if (teller==5) {
            geenAntwoord=true;	// om compatible te zijn met eerste versie camera's die geen antwoord terug sturen
        }

    } while ((!geenAntwoord));
}

void Udp::setGain(int gain) {
    if (!gain)
        gain=this->gain;

    int g=0;
    if ((gain<0) || (gain>47))
        gain=0;

    if (gain<=24)
        g=gain+8;
    else
        g=gain-24+0x51;
    writeReg(0x35,(unsigned int)g);
}

int Udp::getGain(void) {
    return gain;
}

void Udp::setThreshold(int threshold) {

    if (!threshold)
        threshold=this->threshold;

    char s[4];
    s[0]=3;
    s[1]=threshold;
    s[2]=threshold>>8;
    s[3]=0;

    int teller=0;
    do {

        socket->writeDatagram(s, 4, QHostAddress(getAddr()), 34952);

        // timeout
        Sleep(200);

        teller++;
        if (teller==5) {
            geenAntwoord=true;	// om compatible te zijn met eerste versie camera's die geen antwoord terug sturen
        }

    } while ((!geenAntwoord));
}

void Udp::setTopleftX (int topleft_x) {
    this->topleft_x = topleft_x;
}
int Udp::getTopleftX (void) {
    return topleft_x;
}

void Udp::setTopleftY (int topleft_y) {
    this->topleft_y = topleft_y;
}
int Udp::getTopleftY (void) {
    return topleft_y;
}

void Udp::setBottomrightX (int bottomright_x) {
    this->bottomright_x = bottomright_x;
}
int Udp::getBottomrightX (void) {
    return bottomright_x;
}

void Udp::setBottomrightY (int bottomright_y) {
    this->bottomright_y = bottomright_y;
}
int Udp::getBottomrightY (void) {
    return bottomright_y;
}

void Udp::timeStamp(void) {

    Udp::currentTime = time.time().toString("hh:mm:ss.zzz");

}
    
