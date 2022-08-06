#include "lib.h"
#include "strconvEx.h"
#include <archive.h>
#include <archive_entry.h>
#include <sys/utime.h> // _utimbuf
#include <filesystem>
#include "wstrutil.h"
static bool api_archive_entry_write(archive *a,
                                    archive_entry *entry,
                                    const std::wstring &target,
                                    int64_t *extractSizeTotal,
                                    void *data,
                                    ArchiveProgressCallback callback)
{
    std::wstring pathname = archive_entry_pathname_w(entry);
    std::wstring writePath = target + L"/" + pathname;
    bool isDir = strutil::ends_with(pathname, L"/");
    if(isDir) {
        return std::filesystem::create_directories(writePath);
    }
    time_t mtime = archive_entry_mtime(entry);
    std::filesystem::path file = writePath;
    std::filesystem::path dir = file.parent_path();
    std::filesystem::create_directories(dir);
    FILE *fp;;
    if((fp = _wfopen(writePath.c_str(), L"wb"))) {
        int fd = fileno(fp);
        archive_read_data_into_fd(a, fd);
        fclose(fp);
        _utimbuf utbuff;
        utbuff.actime = mtime;
        utbuff.modtime = mtime;
        _wutime(writePath.c_str(), &utbuff);
        if(callback)
        {
            (*extractSizeTotal) += archive_entry_size(entry);
            callback(data, (*extractSizeTotal));
        }
    }
    return true;
}
extern "C" __declspec(dllexport) bool extract_archive(const char *archivePath,
                                                      const char *outputDir,
                                                      void *data,
                                                      ArchiveProgressCallback callback = nullptr)
{
    int64_t extractSizeTotal = 0;
    int r;
    struct archive *a = archive_read_new();
    archive_read_support_format_all(a);
    archive_read_support_filter_all(a);
    r = archive_read_open_filename_w(a, utf8_to_wide(archivePath).c_str(), 10240);
    if (r)
    {
        return false;
    }
    for (;;)
    {
        struct archive_entry *entry;
        r = archive_read_next_header(a, &entry);
        if (r == ARCHIVE_EOF)
            break;
        if (r < ARCHIVE_OK)
            fprintf(stderr, "%s\n", archive_error_string(a));
        if (r < ARCHIVE_WARN)
            return false;
        if(!api_archive_entry_write(a,
                                    entry,
                                    utf8_to_wide(outputDir),
                                    &extractSizeTotal,
                                    data,
                                    callback))
        {
            //return false;
        }
    }
    archive_read_close(a);
    archive_read_free(a);
    return true;
}
static void dummy()
{
    proto_extract_archive f1 = extract_archive;
}
