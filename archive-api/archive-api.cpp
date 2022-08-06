#include "debug_line.h"
#include "archive_api.h"
#include "local_api.h"
#ifdef QT_STATIC_BUILD
#include <QtCore>
#include "strconvEx.h"
#include "utf8LogHandler.h"
class MyQt
{
public:
    QCoreApplication *app = nullptr;
    explicit MyQt()
    {
        std::vector<char *>argv;
        argv = get_ansi_args();
        int argc = argv.size();
        this->app = new QCoreApplication(argc, &argv[0]);
        qInstallMessageHandler(utf8LogHandler);
        //qDebug() << qApp->arguments();
    }
    virtual ~MyQt()
    {
        delete this->app;
    }
};
#endif
extern "C" __declspec(dllexport) int start(int port)
{
    auto svr = new ArchiveApiServer(port);
    return svr->start();
}
extern "C" __declspec(dllexport) void stop(int port)
{
    udebug_line2("Stopping server...", port);
    HttpApiServer::stop_by_port_number(port);
}
extern "C" __declspec(dllexport) const char *standby(const char *name)
{
    QString strId = name;
    QThread::create([strId]()
    {
        LocalApiServer svr(strId);
        svr.Handle("add2", [](const QString method, const QVariant &args)->QVariant
        {
            QVariantList argList = args.toList();
            if (argList.size() != 2) return 0;
            return argList[0].toDouble() + argList[1].toDouble();
        });
        svr.standby();
    })->start();
    return nullptr;
}
static void dummy()
{
    proto_start start_ = start;
    proto_stop stop_ = stop;
    proto_standby standby_ = standby;
}
