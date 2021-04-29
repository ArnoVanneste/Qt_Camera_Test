//#ifndef CONNECTION_H
//#define CONNECTION_H

//typedef struct Fram {
//    unsigned char type;
//    unsigned char framenr;
//    unsigned char acknr;
//    unsigned length;
//    unsigned short checksumData;
//    char *data;
//    struct Fram *volgende;
//} Frame;

//typedef struct {
//    unsigned xsize;
//    unsigned ysize;
//    unsigned ystart;
//    unsigned yend;
//    char *data;
//} UpdateRec;

//typedef struct dirinfo {
//    dirinfo *volgende;
//    char fname[1024];
//    FILETIME date;
//    bool isDir;
//    unsigned size;
//    bool end;
//} DIRINFO;

//class Connection
//{
//public:
//    int fsize;
//        FILETIME fileTime;
//        int fileFrames;
//        FILE * file;
//        HANDLE hDir;
//        bool findFirstRemoteFile(char *map, DIRINFO *info);
//        bool findNextRemoteFile(DIRINFO *info);
//        bool enableLzCompression;
//        bool color16bit;
//        int mouseFrames;
//        char *bitmap;
//        int ysize;
//        int xsize;
//        int decSize;
//        void addFrameBuffer(char type, unsigned int lengte, char *data);
//        void addFrame(char type, unsigned int lengte, char *data);
//        void updateScreen(char *lzBuffer, unsigned lzSize);
//        unsigned long to;
//        bool sendFrame;
//        char recvHeader[24];
//        int recvState;
//        Frame *laatstVerzondenFrame;
//        Frame recvFrame;
//        Frame *eersteFrame;
//        Frame *laatsteFrame;
//        DIRINFO *eersteDirInfo;
//        unsigned char acknr;
//        unsigned char framenr;
//        bool doCommunication();
//        void verwerkRecvFrame();
//        HANDLE hThread;
//        static DWORD WINAPI thread(LPVOID p);
//        void start();
//        void stop();
//        bool stopped;
//        virtual bool isConnected();
//        virtual int drdy();
//        virtual int receive(char *data, int size);
//        virtual bool send(char *data, int size);
//        Connection(HWND hWnd);
//        virtual ~Connection();
//        HWND mainhWnd;
//};

//#endif // CONNECTION_H
