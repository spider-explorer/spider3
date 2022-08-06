#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "utf8LogHandler.h"
#include "debug_line.h"
//#include "jnetwork.h"
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    qInstallMessageHandler(utf8LogHandler);
    qdebug_line1("Hello World!");
    //JNetworkManager nm;
    //qDebug().noquote() << nm.getBatchAsText(QUrl("https://raw.githubusercontent.com/cyginst/cyginst-v1/master/cyginst.bat"));
    return 0; // return app.exec();
}
