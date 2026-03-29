#ifndef FILE_CONTROL_H
#define FILE_CONTROL_H
#include <QFile>
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

struct File_Control
{
    static bool CopyWithCopyFileRange(const QString &srcPath, const QString &dstPath);
};

#endif // FILE_CONTROL_H
