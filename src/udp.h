#ifndef UDPSERVER_H
#define UDPSERVER_H

#include <QTime>
#include <QDateTime>
#include <QObject>
#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QThread>
#include <QMutex>
#include <QTimer>
#include <QDateTime>
#include <QTime>
class Udp : public QObject
{
    Q_OBJECT


public:
    Udp();
    ~Udp();
    const char * getBuffer(void) const;
    const char * getLaser(void) const;
    void getBufferCoppy(char * copy);
    QMutex mutex;

    int getFps(void);
    int getDelta(void);

signals:
    void hasToRender();
    void testSignal();

public slots:
    void sendDataImg(QString addr);
    void sendDataLaser(QString addr);
    void readyRead();
    void chooseCam();
    void testCallback();

public:
    int fwVersion = 0;
    int hwVersion = 0;

    QByteArray getData(void);
    void setAddr(QHostAddress addr);
    QString getAddr(void);
    QString getPosition(QString addr);
    void writeReg(int address, unsigned short data);
    void setRoi(int xstart, int xsize, int ystart, int ysize, int xinc, int yinc, int interl, int gain, int mode);
    void setGain(int gain);
    int getGain(void);
    void setThreshold(int threshold);
    void timeStamp(void);

public:
    int topleft_x = 0;
    int topleft_y = 0;
    int bottomright_x = 0;
    int bottomright_y = 0;

    void setTopleftX (int topleft_x);
    int getTopleftX (void);

    void setTopleftY (int topleft_y);
    int getTopleftY (void);

    void setBottomrightX (int bottomright_x);
    int getBottomrightX (void);

    void setBottomrightY (int bottomright_y);
    int getBottomrightY (void);

public:
    enum {
        SEND_VLASER=1,
        SET_THRESHOLD=2,
        SEND_LASER=4,
        SEND_PICTURE=8,
        FIND_MIDDLE=16,
    } Mode;

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
    int counter = 0;
    int badPixels = 0;
    int gain = 0;
    int threshold;
    int delta = 0;
    int dCounter = 0;
    QString prevTime ="0";
    QString curTime = "0";

    QDateTime time;

    QTimer *fpsCounter;
    QTimer *deltaCounter;

    int ipnr;
    bool geenAntwoord;

    QString currentTime;

    QThread testThread;
};

#endif // UDPSERVER_H


