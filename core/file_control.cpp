#include "file_control.h"
bool File_Control::CopyWithCopyFileRange(const QString &srcPath, const QString &dstPath)
{
    QFile srcFile(srcPath);
    QFile dstFile(dstPath);
    if (!srcFile.open(QIODevice::ReadOnly))
    {
        return false;
    }
    if (!dstFile.open(QIODevice::WriteOnly))
    {
        return false;
    }
    int srcFd = srcFile.handle();
    int dstFd = dstFile.handle();
    qint64 total = srcFile.size();
    qint64 copied = 0;
    while (copied < total)
    {
        size_t chunkSize = static_cast<size_t>(qMin<qint64>(64 * 1024 * 1024, total - copied));
        ssize_t ret = copy_file_range(srcFd, nullptr, dstFd, nullptr, chunkSize, 0);
        if (ret < 0)
        {
            return false;
        }
        if (ret == 0)
        {
            break;
        }
        copied += ret;
    }
    dstFile.resize(total);
    return true;
}
