#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QLatin1String>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.setFixedSize(QSize(1200, 675));

    QFile file("D:\\repos\\Qt_Camera_Test\\style\\SpyBot.qss");
    file.open(QFile::ReadOnly);

    QString styleSheet { QLatin1String(file.readAll()) };

    //setup stylesheet
    a.setStyleSheet(styleSheet);

    w.show();
    return a.exec();
}
