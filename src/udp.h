#ifndef UDPSERVER_H
#define UDPSERVER_H

#include <QObject>
#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QThread>
#include <QMutex>
#include <QTimer>
class Udp : public QObject
{
    Q_OBJECT


public:
    Udp();
    const char * getBuffer(void) const;
    void getBufferCoppy(char * copy);
    QMutex mutex;
    QString getFps(void);

signals:
    void hasToRender();

public slots:
    void sendData(QString addr);
    void readyRead();
    void chooseCam();

public:
    int fwVersion = 0;
    int hwVersion = 0;

    QByteArray getData(void);
    void setAddr(QHostAddress addr);
    QString getAddr(void);
    QString getPosition(QString addr);

private:
    QUdpSocket *socket;
    char bitmap[1280*1024];
    unsigned short laser[1280];
    bool progok = false;
    int vorigelijnnr = 0;
    int nextframe = 0;
    unsigned short s[1000];

    QHostAddress sender;
    QString topaddr = "192.168.2.2";
    QString leftaddr = "192.168.2.3";
    QString rightaddr = "192.168.2.4";

    int fps = 0;
    int badPixels = 0;
};

#endif // UDPSERVER_H


