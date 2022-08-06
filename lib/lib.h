#ifndef LIB_H
#define LIB_H
#include <stdint.h>
typedef void (*ArchiveProgressCallback)(void *data, int64_t extractSizeTotal);
typedef bool (*proto_extract_archive)(const char *archivePath,
                                      const char *outputDir,
                                      void *data,
                                      ArchiveProgressCallback callback);
#ifdef Q_UNUSED
#include "MemoryModule.h"
static bool extractArchive(const char *archivePath,
                           const char *outputDir,
                           void *data,
                           ArchiveProgressCallback callback)
{
    qDebug() << "extractArchive(1)";
    static HMEMORYMODULE h = nullptr;
    if(!h)
    {
        QFile dll(":/lib-x86_64-static.dll");
        if(dll.open(QIODevice::ReadOnly))
        {
            qDebug() << "dll opened.";
            QByteArray bytes = dll.readAll();
            h = MemoryLoadLibrary(bytes.data(), bytes.size());
            qDebug() << h;
        }
    }
    if(!h) return false;
    proto_extract_archive extract_archive = (proto_extract_archive)MemoryGetProcAddress(h, "extract_archive");
    qDebug() << (void *)extract_archive;
    if(!extract_archive) return false;
    return extract_archive(archivePath, outputDir, data, callback);
}
#endif
#endif // LIB_H
