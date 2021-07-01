#include "tcpip.h"

#include <winsock2.h>

#define PORT	54319
//#define PORT	54320

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TcpIp::TcpIp(char computerName[]) : Connection() {
    SOCKADDR_IN dest_sin;
    dest_sin.sin_port=htons(PORT);
    dest_sin.sin_family=AF_INET;
    dest_sin.sin_addr.s_addr=inet_addr(computerName);
    if ((!dest_sin.sin_addr.s_addr)||(dest_sin.sin_addr.s_addr== 0xFFFFFFFF)){
        hostent *h=gethostbyname(computerName);
        if (h) {
            dest_sin.sin_addr.s_addr=*((unsigned *)h->h_addr_list[0]);
        }
    }
    if (dest_sin.sin_addr.s_addr) {
        s = socket (AF_INET, SOCK_STREAM, 0);
        if	(WSAConnect(s, (SOCKADDR *)&dest_sin, sizeof(dest_sin), NULL, NULL, NULL, NULL)) {
            //gefaald
            closesocket(s);
            s=INVALID_SOCKET;
        }
    } else
        s=INVALID_SOCKET;
}

TcpIp::TcpIp(unsigned ipnr) : Connection() {
    SOCKADDR_IN dest_sin;
    dest_sin.sin_port=htons(PORT);
    dest_sin.sin_family=AF_INET;
    dest_sin.sin_addr.s_addr=ipnr;
//	ipnr=inet_addr("100.100.50.10");

    s = socket (AF_INET, SOCK_STREAM, 0);
    int i=WSAGetLastError();
    if	(WSAConnect(s, (SOCKADDR *)&dest_sin, sizeof(dest_sin), NULL, NULL, NULL, NULL)) {
        //gefaald
        int error=WSAGetLastError();
        closesocket(s);
        s=INVALID_SOCKET;
    }
}

TcpIp::TcpIp(unsigned brol, SOCKET s) : Connection() {
    this->s = s;
}

TcpIp::~TcpIp() {
    if (s!=INVALID_SOCKET)
        closesocket(s);
}

bool TcpIp::send(char *data, int size) {
    //verzenden in paketjes van 1kb
    int i=0;
    do {
        if (::send(s,&data[i],std::min(1024,size-i),0)==SOCKET_ERROR)
            return false;
        i+=1024;
    } while (i<size);

//	return (::send(s,data,size,0)!=SOCKET_ERROR);
    return true;
}

int TcpIp::receive(char *data, int size) {
    int i=::recv(s,data,size,0);
    if (i==SOCKET_ERROR)
        return 0;
    else
        return i;
}

int TcpIp::drdy() {
    unsigned long u;
    int i=ioctlsocket(s,FIONREAD,&u);

    if (i==SOCKET_ERROR) {
        i=WSAGetLastError();
        return -1;
    }
    return u;
}

bool TcpIp::isConnected() {
    if (s==INVALID_SOCKET)
        return false;
    return true;
}
