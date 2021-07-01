#ifndef TCPIP_H
#define TCPIP_H

#include "connection.h"
#include <winsock.h>

class TcpIp : public Connection
{
public:
    bool isConnected();
    int drdy();
    int receive(char *data, int size);
    bool send(char *data, int size);
    SOCKET s;
    TcpIp(unsigned ipnr);
    TcpIp(char computerName[]);
    TcpIp(unsigned brol, SOCKET s);
    virtual ~TcpIp();
};

#endif // TCPIP_H
