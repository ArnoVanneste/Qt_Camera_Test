#include "connection.h"
#include <stdio.h>

#include <QThread>

Connection::Connection(void) {
    file=0;
    fileFrames=0;
    framenr=0;
    acknr=0;
    eersteFrame=0;
    laatsteFrame=0;
    laatstVerzondenFrame=0;
    recvState=0;
    sendFrame=false;
    bitmap=0;
    mouseFrames=0;
    hThread=0;
    enableLzCompression=true;
    hDir=CreateMutex(0,false,0);
    eersteDirInfo=0;
    color16bit=false;
    connectionStatus=false;
}

Connection::~Connection() {
    if (hThread)
        WaitForSingleObject(hThread,INFINITE);
    CloseHandle(hDir);
}

bool Connection::isConnected() {
    return false;
}

int Connection::drdy() {
    return -1;	// error
}

bool Connection::send(char *data, int size) {
    return false;
}

int Connection::receive(char *data, int size) {
    return 0;
}

bool Connection::start(void) {
//    unsigned long tid;
//    hThread=CreateThread(0,0,Connection::thread,this,0,&tid);
    /*	char buf[34];
        c->receive(buf,34);

        c->receive((char *)&c->xsize,4);
        c->receive((char *)&c->ysize,4);

        c->bitmap=(char *)calloc(c->ysize,c->xsize);
    */
    this->decSize=0;
    this->stopped=false;
    while ((this->doCommunication()) && (!this->stopped))
        Sleep(10);
    return false;
}

void Connection::stop() {
    stopped=true;
}

unsigned toUnsigned(char *ch,int i) {
    unsigned retval=0;
    while (i--) {
        retval=(retval*10)+*ch-'0';
        ch++;
    }
    return retval;
}

bool Connection::doCommunication() {

   Qt::HANDLE pid = QThread::currentThreadId();

    // ontvang data
    int i;
    while (i=drdy()) {
        if (i<0) {

            if (connectionStatus) {

                emit Connection::sendStatusUpdate(false);
                connectionStatus = false;

            }
            return false;	//error

        }

        if (recvState<25) {
            char ch;
            receive(&ch,1);
            if (ch=='@')
                recvState=1;
            else if (recvState) {
                recvHeader[recvState-1]=ch;
                recvState++;
                if (recvState==25) {

                    if (!connectionStatus) {

                        emit Connection::sendStatusUpdate(true);
                        connectionStatus = true;

                    }
                    //controle checksum,...
                    recvFrame.type=toUnsigned(&recvHeader[0],3);
                    recvFrame.framenr=toUnsigned(&recvHeader[3],3);
                    recvFrame.acknr=toUnsigned(&recvHeader[6],3);
                    recvFrame.length=toUnsigned(&recvHeader[9],5);
                    recvFrame.checksumData=toUnsigned(&recvHeader[14],5);
                    if ((toUnsigned(&recvHeader[19],5)^0xffff)!=(unsigned short)(recvFrame.type+recvFrame.framenr+recvFrame.acknr+recvFrame.length+recvFrame.checksumData))
                        recvState=0;
                    else if (recvFrame.length)
                        recvFrame.data=(char *)malloc(recvFrame.length);
                    else {
                        recvFrame.data=0;
                        recvState=0;
                        verwerkRecvFrame();
                        break;	//so ack can be send
                    }
                }
            }
        } else {
            // ontvangen data
            int i=receive(&recvFrame.data[recvState-25],recvFrame.length+25-recvState);
            if (i) {
                // ok ontvangen
                recvState+=i;
                if ((recvState-25)==(int)recvFrame.length) {
                    recvState=0;
                    verwerkRecvFrame();
                    break;	//so ack can be send
                }
            } else {
                // fout
                // verbinding verbroken???
                return false;
            }
        }
    }


    // verzenden data

    bool ok=true;

    Frame *teVerzendenFrame=0;

    if ((laatstVerzondenFrame==0) || ((GetTickCount()-to)>10000)) {
        // time out
        teVerzendenFrame=eersteFrame;
        if (!eersteFrame) {
            sendFrame=true;
            to=GetTickCount();
        }
    } else if (laatstVerzondenFrame)
        teVerzendenFrame=laatstVerzondenFrame->volgende;

    if (teVerzendenFrame) {
        // verzenden van frame
        laatstVerzondenFrame=teVerzendenFrame;
        char s[100];
        unsigned short sum=teVerzendenFrame->type+teVerzendenFrame->framenr+teVerzendenFrame->acknr+teVerzendenFrame->length+teVerzendenFrame->checksumData;
        sprintf(s,"@%03i%03i%03i%05i%05i%05i",teVerzendenFrame->type,teVerzendenFrame->framenr,teVerzendenFrame->acknr,teVerzendenFrame->length,teVerzendenFrame->checksumData,sum^0xffff);
        to=GetTickCount();
        if (!send(s,25))
            ok=false;
        else if (!send(teVerzendenFrame->data,teVerzendenFrame->length))
            ok=false;
    } else if (sendFrame) {
        // verzenden van leeg frame met ack
        char s[100];
        unsigned short sum=framenr+acknr+65535;
        sprintf(s,"@000%03i%03i0000065535%05i",framenr,acknr,sum^0xffff);
        if (!send(s,25))
            ok=false;
        else
            sendFrame=false;
    }

    return ok;    
}

void Connection::verwerkRecvFrame() {
    if (recvFrame.framenr==acknr) {
        // frame is frame dat verwacht werd

        if (recvFrame.type)
            acknr++;

        switch (recvFrame.type) {
            case 0:	// ack van frames
                    // kijken of frame met dat nr-1 nog in lijst staat
                    // indien ja: die & alle vorige wissen
                    // indien nee: niets doen

                    if (eersteFrame) {
                        Frame *f=eersteFrame;
                        while ((f) && (f->framenr!=(unsigned char)(recvFrame.acknr-1))) {
                            f=f->volgende;
                        }

                        if (f) {
                            // gevonden
                            // opnieuw doorlopen en wissen
                            int nr;
                            do {
                                nr=eersteFrame->framenr;
                                f=eersteFrame;
                                eersteFrame=eersteFrame->volgende;
                                if (f==laatstVerzondenFrame)
                                    laatstVerzondenFrame=0;
                                if (f->type==3)
                                    mouseFrames--;
                                if (f->type==6)
                                    fileFrames--;
                                if (f->data)
                                    free(f->data);
                                free(f);
                            } while (nr!=(unsigned char)(recvFrame.acknr-1));

                            if (!eersteFrame)
                                laatsteFrame=0;
                        }
                    }
                    break;

            case 1:	// commando's
                    if (1) {
                        unsigned short *u=(unsigned short *)recvFrame.data;
                        xsize=u[0];
                        ysize=u[1];

                        if (u[2]&2)
                            enableLzCompression=false;
                        else
                            enableLzCompression=true;

                        if (bitmap)
                            free(bitmap);

                        if (u[2]&4) {
                            color16bit=true;
                            bitmap=(char *)calloc(ysize,xsize*2);
                        } else
                            bitmap=(char *)calloc(ysize,xsize);

                        PostMessage(mainhWnd,WM_USER+2,u[2],*((unsigned *)u));

                        if ((u[2]!=2) && (u[2]!=3) && (u[2]!=6))
                            stopped=true;	//unsupported
                    }
                    break;

            case 7:
            case 2: // screenshot
                    // updateScreen(recvFrame.data,recvFrame.length);
                    break;

            case 3: // muis

                    break;

            case 4: // toetsenbord
                    break;

            case 5: // directory listing antwoord
                    if (recvFrame.length>0) {
                        WaitForSingleObject(hDir,INFINITE);
                        DIRINFO *d=eersteDirInfo;
                        while ((d) && (d->volgende))
                            d=d->volgende;
                        if (d) {
                            d->volgende=(DIRINFO *)calloc(1,sizeof(DIRINFO));
                            d=d->volgende;
                        } else {
                            d=(DIRINFO *)calloc(1,sizeof(DIRINFO));
                            eersteDirInfo=d;
                        }
                        if (recvFrame.data[0]=='0')
                            d->end=true;
                        else {
                            unsigned x=1;
                            while ((x<recvFrame.length) && (recvFrame.data[x]!='\t'))
                                x++;

                            if (x<recvFrame.length)
                                recvFrame.data[x]=0;

                            char *size=&recvFrame.data[x+1];

                            while ((x<recvFrame.length) && (recvFrame.data[x]!='\t'))
                                x++;

                            if (x<recvFrame.length)
                                recvFrame.data[x]=0;

                            char *th=&recvFrame.data[x+1];

                            while ((x<recvFrame.length) && (recvFrame.data[x]!='\t'))
                                x++;

                            if (x<recvFrame.length) {
                                recvFrame.data[x]=0;
                                x++;
                                strcpy(d->fname,&recvFrame.data[1]);
                                if (strcmp(size,"<DIR>"))
                                    d->size=atoi(size);
                                else
                                    d->isDir=true;
                                d->date.dwHighDateTime=(unsigned)_atoi64(th);
                                d->date.dwLowDateTime=0;
                                while (x<recvFrame.length) {
                                    d->date.dwLowDateTime=(d->date.dwLowDateTime*10)+recvFrame.data[x]-'0';
                                    x++;
                                }
                            }
                        }
                        ReleaseMutex(hDir);
                    }
                    break;
            case 6: if (recvFrame.data[0]=='2') {
                        //fout
                        char *s=(char *)malloc(recvFrame.length);
                        memcpy(s,&recvFrame.data[1],recvFrame.length-1);
                        s[recvFrame.length-1]=0;
                        PostMessage(mainhWnd,WM_USER+3,0,(long)s);
//						free(s);
                    } else if (file) {
                        // stukje bestand
                        if (recvFrame.data[0]=='0') {
                            //einde
                            fclose(file);
                            file=0;
                        } else if (recvFrame.data[0]=='3') {
                            fsize=0;
                            fileTime.dwHighDateTime=0;
                            fileTime.dwLowDateTime=0;
                            int x=1;
                            while ((x<recvFrame.length)&&(recvFrame.data[x]!=';')) {
                                fsize=(fsize*10)+recvFrame.data[x]-'0';
                                x++;
                            }
                            x++;
                            while ((x<recvFrame.length)&&(recvFrame.data[x]!=';')) {
                                fileTime.dwHighDateTime=(fileTime.dwHighDateTime*10)+recvFrame.data[x]-'0';
                                x++;
                            }
                            x++;
                            while ((x<recvFrame.length)&&(recvFrame.data[x]!=';')) {
                                fileTime.dwLowDateTime=(fileTime.dwLowDateTime*10)+recvFrame.data[x]-'0';
                                x++;
                            }
                        } else {
                            fwrite(&recvFrame.data[1],1,recvFrame.length-1,file);
                        }
                    }
                    break;
        }
    }

    if (recvFrame.type)
        sendFrame=true;

    if (recvFrame.data)
        free(recvFrame.data);
}


//DWORD WINAPI Connection::thread(LPVOID p) {
//    Connection *c=(Connection *)p;
///*	char buf[34];
//    c->receive(buf,34);

//    c->receive((char *)&c->xsize,4);
//    c->receive((char *)&c->ysize,4);

//    c->bitmap=(char *)calloc(c->ysize,c->xsize);
//*/
//    c->decSize=0;
//    c->stopped=false;
//    while ((c->doCommunication()) && (!c->stopped))
//        Sleep(10);
//    return false;
//}

//void inline memxor(char *d, char data,int lengte) {
//    __asm {
//        mov ecx,lengte
//        mov eax,d
//        mov edi,eax
//        mov esi,eax
//        mov ah,data
//        cld
//lus:	lodsb
//        xor	al,ah
//        stosb
//        loop lus
//    }
//}

//void inline memxor16(unsigned short *d, unsigned short data,int lengte) {
//    __asm {
//        mov ecx,lengte
//        mov eax,d
//        mov edi,eax
//        mov esi,eax
//        mov bx,data
//        cld
//lus2:	lodsw
//        xor	ax,bx
//        stosw
//        loop lus2
//    }
//}

//void Connection::updateScreen(char *lzBuffer, unsigned lzSize) {
//    if (decSize>=xsize*ysize) {
//        // nieuw beeld
//        decSize=0;
//    }

//    unsigned startpos=decSize;


//    if (color16bit) {
//        unsigned short *u=(unsigned short *)lzBuffer;
//        lzSize/=2;
//        while (lzSize) {
//            if (*u==25251) {
//                unsigned size=*(unsigned *)(u+1);
//                memxor16((unsigned short *)&bitmap[decSize*2],u[3],size);
//                u+=4;
//                lzSize-=4;
//                decSize+=size;
//            } else if (*u==25252) {
//                unsigned short size=u[1];
//                memxor16((unsigned short *)&bitmap[decSize*2],u[2],size);
//                u+=3;
//                lzSize-=3;
//                decSize+=size;
//            } else if (*u==25253) {
//                *(unsigned short *)&bitmap[decSize*2]^=u[1];
//                u+=2;
//                lzSize-=2;
//                decSize++;
//            } else {
//                *(unsigned short *)&bitmap[decSize*2]^=u[0];
//                u++;
//                lzSize--;
//                decSize++;
//            }

//        }
//    } else {

//        // verwerken data
//        char *pLzBuffer=lzBuffer;

//        char dicSymbol[255][127];
//        unsigned dicAcces[255];
//        char dicLength[255];
//        int dicSize=0;
//        char decData[128];
//        int decDataLength=0;
//        int lzCounter=0;

//        int vorigSymbolLength=0;
//        char vorigSymbol[128];

//        while (lzCounter<lzSize) {
//            int x=(unsigned char)*pLzBuffer;
//            if (enableLzCompression) {
//                pLzBuffer++;
//                lzCounter++;
//            }
//            if ((enableLzCompression) && (x<255)) {
//                memcpy(decData,dicSymbol[x],dicLength[x]);
//                decDataLength=dicLength[x];
//            } else {
//                // nieuw symbool
//                if (*pLzBuffer==25)
//                    decDataLength=6;
//                else if (*pLzBuffer==26)
//                    decDataLength=5;
//                else if (*pLzBuffer==27)
//                    decDataLength=4;
//                else if (*pLzBuffer==28)
//                    decDataLength=3;
//                else if (*pLzBuffer==29)
//                    decDataLength=2;
//                else
//                    decDataLength=1;
//                memcpy(decData,pLzBuffer,decDataLength);
//                pLzBuffer+=decDataLength;
//                lzCounter+=decDataLength;

//                // toevoegen in dictionary
//                if (dicSize==255) {
//                    // een weinig gebruikt symbool in dictionary vervangen
//                    unsigned min=dicAcces[254];
//                    int sym=254;
//                    for (x=0; x<254; x++) {
//                        if (dicAcces[x]<=min) {
//                            min=dicAcces[x];
//                            sym=x;
//                        }
//                    }
//                    x=sym;
//                } else {
//                    x=dicSize;
//                    dicSize++;
//                }

//                // toevoegen / vervangen in dictionary
//                memcpy(dicSymbol[x],decData,decDataLength);
//                dicLength[x]=decDataLength;
//            }
//            dicAcces[x]=lzCounter;

//            if ((vorigSymbolLength) && ((vorigSymbolLength+decDataLength)<128)) {
//                // toevoegen van vorig symbool + huidig in dic
//                if (dicSize==255) {
//                    // een weinig gebruikt symbool in dictionary vervangen
//                    unsigned min=dicAcces[254];
//                    int sym=254;
//                    for (x=0; x<254; x++) {
//                        if (dicAcces[x]<=min) {
//                            min=dicAcces[x];
//                            sym=x;
//                        }
//                    }
//                    x=sym;
//                } else {
//                    x=dicSize;
//                    dicSize++;
//                }
//                // toevoegen / vervangen in dictionary
//                memcpy(dicSymbol[x],vorigSymbol,vorigSymbolLength);
//                memcpy(&dicSymbol[x][vorigSymbolLength],decData,decDataLength);
//                dicLength[x]=decDataLength+vorigSymbolLength;
//                dicAcces[x]=lzCounter;
//            }

//            vorigSymbolLength=decDataLength;
//            memcpy(vorigSymbol,decData,decDataLength);

//            // verwerken van RLE gecodeerde data in decData
//            x=0;
//            while (x<decDataLength) {
//                if (decData[x]==29) {
//                    bitmap[decSize++]^=decData[x+1];
//                    x+=2;
//                } else if (decData[x]==28) {
//                    memxor(&bitmap[decSize],decData[x+2],(unsigned char)decData[x+1]);
//                    decSize+=(unsigned char)decData[x+1];
//                    x+=3;
//                } else if (decData[x]==27) {
//                    int size=*((unsigned short *)&decData[x+1]);
//                    memxor(&bitmap[decSize],decData[x+3],size);
//                    decSize+=size;
//                    x+=4;
//                } else if (decData[x]==26) {
//                    int size=(*((unsigned*)&decData[x+1]))&0xffffff;
//                    memxor(&bitmap[decSize],decData[x+4],size);
//                    decSize+=size;
//                    x+=5;
//                } else if (decData[x]==25) {
//                    int size=*((unsigned *)&decData[x+1]);
//                    memxor(&bitmap[decSize],decData[x+5],size);
//                    decSize+=size;
//                    x+=6;
//                } else
//                    bitmap[decSize++]^=decData[x++];
//            }
//        }
//    }
//    UpdateRec *u=(UpdateRec *)malloc(sizeof(UpdateRec));
//    u->ystart=startpos/xsize;
//    u->yend=decSize/xsize;
//    u->xsize=xsize;
//    u->ysize=ysize;
//    u->data=bitmap;
//    PostMessage(mainhWnd,WM_USER,0,(long)u);
//}

void Connection::addFrame(char type, unsigned int lengte, char *data) {
    if (lengte) {
        char *d=(char *)malloc(lengte);
        memcpy(d,data,lengte);
        addFrameBuffer(type,lengte,d);
    } else
        addFrameBuffer(type,0,0);
}

void Connection::addFrameBuffer(char type, unsigned int lengte, char *data) {
    Frame *f=(Frame *)calloc(1,sizeof(Frame));
    f->type=type;
    f->length=lengte;
    f->framenr=framenr++;

    unsigned sum=0;

    if (type==3)
        mouseFrames++;
    if (type==6)
        fileFrames++;

    for (unsigned x=0; x<lengte; x++)
        sum+=(unsigned char)data[x];

    f->checksumData=0xffff^sum;
    f->data=data;

    if (laatsteFrame)
        laatsteFrame->volgende=f;
    else
        eersteFrame=f;
    laatsteFrame=f;
}

bool Connection::findFirstRemoteFile(char *map, DIRINFO *info) {
    WaitForSingleObject(hDir,INFINITE);
    DIRINFO *d;
    while (eersteDirInfo) {
        d=eersteDirInfo;
        eersteDirInfo=d->volgende;
        free(d);
    }
    ReleaseMutex(hDir);

    addFrame(5,strlen(map),map);
    return findNextRemoteFile(info);
}

bool Connection::findNextRemoteFile(DIRINFO *info) {
    while (!eersteDirInfo)
        Sleep(100);
    WaitForSingleObject(hDir,INFINITE);
    memcpy(info,eersteDirInfo,sizeof(DIRINFO));
    DIRINFO *d=eersteDirInfo;
    eersteDirInfo=eersteDirInfo->volgende;
    free(d);
    ReleaseMutex(hDir);
    return (info->end==false);
}
