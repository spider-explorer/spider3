#include <winSock2.h>
//#include "jinstaller.h"
#include <QApplication>
#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include <singleapplication.h>
#include "debug_line.h"
#include "jnetwork.h"
#include "junctionmanager.h"
#include "jarchiver.h"
//#include "MemoryModule.h"
//#include "archive_api.h"
#include "utf8LogHandler.h"
void myCallback(void *data, int64_t extractSizeTotal)
{
    //qDebug() << "myCallback() called.";
    QSplashScreen *splash = (QSplashScreen *)data;
    QLocale locale;
    splash->showMessage(
        QString("Spider本体を展開中...%1").arg(locale.formattedDataSize(extractSizeTotal)),
        Qt::AlignLeft, Qt::white);
}
static QString prepareMain(QSplashScreen &splash)
{
    qdebug_line();
    JNetworkManager nm;
    QLocale locale;
    QString spider2Json = nm.getBatchAsText(QUrl("https://raw.githubusercontent.com/spider-explorer/spider3/main/spider.json"));
    qDebug().noquote() << spider2Json;
    QJsonParseError error;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(spider2Json.trimmed().toUtf8(), &error);
    qDebug() << error.errorString();
    qDebug().noquote() << jsonDoc.toJson();
    QJsonObject object = jsonDoc.object();
    qDebug() << object.keys();
    QString version = object["version"].toString();
    qDebug() << version;
    QJsonArray array = object["url"].toArray();
    QString urlString = array[0].toString();
    qDebug() << urlString;
    splash.showMessage("Spider本体を準備中...", Qt::AlignLeft, Qt::white);
    QString dlPath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) +
                     QString("/.spider-explorer3/.install/%1.7z").arg(version);
    qdebug_line();
    if (!QFileInfo(dlPath).exists())
    {
        qdebug_line();
        if(true)/**/
        {
            QString parentPath = QFileInfo(dlPath).absolutePath();
            QDir(parentPath).removeRecursively();
        }
        qDebug() << nm.getBatchAsFile(
            urlString, dlPath,
            [&splash, &locale](QNetworkReply *reply)
        {
            splash.showMessage(
                QString("Spider本体をダウンロード中...%1")
                .arg(locale.formattedDataSize(reply->bytesAvailable())),
                Qt::AlignLeft, Qt::white);
        });
    }
    qdebug_line();
    if(!QFileInfo(dlPath).exists())
    {
        qdebug_line();
        qDebug() << u8"本体のダウンロードが失敗しました";
        QMessageBox::information(nullptr, "確認", "本体のダウンロードが失敗しました");
    }
    QString installDir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) +
                         QString("/.spider-explorer3/.install/%1").arg(version);
    QString junctionDir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) +
                          QString("/.spider-explorer3/.install/current");
    qdebug_line();
    if (!QFileInfo(installDir).exists())
    {
        qdebug_line();
#if 0x1
        qDebug() << extract_archive(dlPath, installDir, [&splash, &locale](qint64 extractSizeTotal)
        {
            splash.showMessage(
                QString("Spider本体を展開中...%1").arg(locale.formattedDataSize(extractSizeTotal)),
                Qt::AlignLeft, Qt::white);
        });
#else
        qDebug() << "dlPath:" << dlPath;
        qDebug() << "installDir:" << installDir;
        std::size_t archive_id = cli.extract_archive(
            dlPath.toStdString(),
            installDir.toStdString());
        qdebug_line();
        while(true)
        {
            std::int64_t progress = cli.extract_progress(archive_id);
            splash.showMessage(
                QString("Spider本体を展開中...%1").arg(locale.formattedDataSize(progress)),
                Qt::AlignLeft, Qt::white);
            qdebug_line2("progress=", progress);
            QThread::msleep(100);
            if (progress < 0) break;
        }
#endif
        qdebug_line();
        JunctionManager().remove(junctionDir);
        bool created = JunctionManager().create(junctionDir, installDir);
        qdebug_line1(created);
    }
    qdebug_line();
    if (!QFileInfo(junctionDir).exists())
    {
        qdebug_line();
        bool created = JunctionManager().create(junctionDir, installDir);
        qdebug_line1(created);
    }
    qdebug_line();
#ifdef QT_STATIC_BUILD
    QString mainDll = junctionDir + "/main-x86_64-static.dll";
#else
    QString mainDll = junctionDir + "/main-x86_64.dll";
#endif
    qdebug_line();
    return mainDll;
}
int main(int argc, char *argv[])
{
    qDebug() << "main(begin)";
    SingleApplication app(argc, argv, true);
    qInstallMessageHandler(utf8LogHandler);
    if (app.isSecondary())
    {
        qDebug() << "[boot] (app.isSecondary())";
        app.sendMessage("dummy");
        return 0;
    }
    QString mainDll;
    // mainDll = QFileInfo(app.applicationDirPath() + "/main-x86_64-static.dll").absoluteFilePath();
    bool force = (app.arguments().size() >= 2 && app.arguments()[1]=="--force");
    mainDll = app.applicationDirPath() + "/main-x86_64-static.dll";
    // QMessageBox::information(nullptr, "確認", mainDll);
    if (force || !QFileInfo(mainDll).exists())
    {
        QSplashScreen splash(QPixmap(":/splash.png"));
        splash.show();
#if 0x0
        auto readFileBytes = [](const QString &dllPath)->QByteArray
                             {
                                 QFile file(dllPath);
                                 if (!file.open(QIODevice::ReadOnly)) return "";
                                 return file.readAll();
                             };
#ifdef QT_STATIC_BUILD
        QByteArray bytes = readFileBytes(qApp->applicationDirPath() + "/archive-api-x86_64-static.dll");
        ArchiveApiClient cli(bytes.data(), bytes.size());
#else
        QByteArray bytes = readFileBytes(qApp->applicationDirPath() + "/archive-api-x86_64.dll");
        ArchiveApiClient cli(bytes.data(), bytes.size());
#endif
#endif
        mainDll = prepareMain(splash);
        splash.finish(nullptr);
    }
    // exit(0);
    QLibrary lib(mainDll);
    typedef void (*prote_entry1)(const char *main_dll_path, int argc, char **argv);
    prote_entry1 entry1 = (prote_entry1)lib.resolve("entry1");
    typedef void (*prote_entry2)();
    prote_entry2 entry2 = (prote_entry2)lib.resolve("entry2");
    QObject::connect(&app, &SingleApplication::receivedMessage,
                     [entry2](quint32 instanceId, QByteArray message)
    {
        qDebug() << "[boot] before calling entry2()";
        entry2();
    });
    QThread *thread =
        QThread::create([entry1, mainDll, argc, argv]
    {
        entry1(mainDll.toStdString().c_str(), argc, argv);
    });
    QObject::connect(thread, &QThread::finished, [&app]()
    {
        app.exit(0);
    });
    thread->start();
    return app.exec();
}
