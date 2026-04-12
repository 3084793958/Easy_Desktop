#include "file_control.h"
#include <QFileInfo>
#include <QDir>
#include <QAbstractButton>
#include <QPushButton>
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
bool File_Control::CopyRecursively(const QString &src, const QString &dst)
{
    QFileInfo srcInfo(src);
    if (srcInfo.isDir())
    {
        QDir dstDir(dst);
        if (!dstDir.exists() && !dstDir.mkpath("."))
        {
            return false;
        }
        QDir srcDir(src);
        QStringList entries = srcDir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden);
        for (const QString& entry : entries)
        {
            QString newSrc = src + QDir::separator() + entry;
            QString newDst = dst + QDir::separator() + entry;
            if (!File_Control::CopyRecursively(newSrc, newDst))
            {
                return false;
            }
        }
        return true;
    }
    else
    {
        return QFile::copy(src, dst);
    }
}
void File_Control::Copy_File(const QString &srcPath, QString dstPath, const bool &cut, int *autoConflictStrategy)
{
    QFileInfo srcInfo(srcPath);
    if (srcInfo.isDir())
    {
        QString absSrc = srcInfo.absoluteFilePath();
        QString absDst = QFileInfo(dstPath).absoluteFilePath();
        absSrc = QDir::cleanPath(absSrc);
        absDst = QDir::cleanPath(absDst);
        if (absDst.startsWith(absSrc + QDir::separator()))
        {
            return;
        }
    }
    QFileInfo dstInfo;
    dstInfo.setFile(dstPath);
    if (dstInfo.exists())
    {
        int starategy = -1;
        if (*autoConflictStrategy != -1)
        {
            starategy = *autoConflictStrategy;
        }
        else
        {
            File_MessageBox *msgBox = new File_MessageBox(nullptr, dstPath, &starategy, autoConflictStrategy);
            msgBox->Setup();
            msgBox->deleteLater();
        }
        if (starategy == 2 || (starategy == 0 && srcPath == dstPath))
        {
            return;
        }
        else if (starategy == 1)
        {
            if (dstInfo.exists())
            {
                dstPath += ".copy";
                dstInfo.setFile(dstPath);
            }
            unsigned int copy_add_num = 0;
            QString sec_dst_path = dstPath;
            while (dstInfo.exists())
            {
                copy_add_num++;
                dstPath = sec_dst_path + QString::number(copy_add_num);
                dstInfo.setFile(dstPath);
                if (copy_add_num == 0)
                {
                    return;
                }
            }
        }
    }
    if (cut)
    {
        QFileInfo srcInfo(srcPath);
        if (File_Control::CopyWithCopyFileRange(srcPath, dstPath))
        {
            if (QFileInfo(dstPath).exists())
            {
                if (srcInfo.isDir())
                {
                    QDir(srcPath).removeRecursively();
                }
                else
                {
                    QFile::remove(srcPath);
                }
            }
        }
        else
        {
            if (File_Control::CopyRecursively(srcPath, dstPath))
            {
                if (dstInfo.exists())
                {
                    if (srcInfo.isDir())
                    {
                        QDir(srcPath).removeRecursively();
                    }
                    else
                    {
                        QFile::remove(srcPath);
                    }
                }
            }
        }
    }
    else
    {
        if (!File_Control::CopyWithCopyFileRange(srcPath, dstPath))
        {
            File_Control::CopyRecursively(srcPath, dstPath);
        }
    }
}
QString File_Control::FilenameForBash(QString filename)
{
    if (!filename.contains('\''))
    {
        return QString("'%1'").arg(filename);
    }
    QString escaped = filename;
    escaped.replace("'", "'\\''");   // 将 ' 替换为 '\''
    return QString("'%1'").arg(escaped);
}
File_MessageBox::File_MessageBox(QWidget *parent, QString filename, int *strategy, int *autoConflictStrategy)
    :QMessageBox(parent)
    ,m_strategy(strategy)
    ,m_autoConflictStrategy(autoConflictStrategy)
{
    this->setWindowTitle("该文件已存在");
    this->setText(QString("\"%1\" 已存在").arg(filename));
    this->setInformativeText("选择操作:");
    buttonOverwrite = this->addButton("覆盖", QMessageBox::AcceptRole);
    buttonCoexist = this->addButton("共存", QMessageBox::ActionRole);
    buttonCancel = this->addButton("取消", QMessageBox::RejectRole);
    this->setCheckBox(checkAuto);
}
void File_MessageBox::Setup()
{
    this->exec();
    if (!m_strategy) return;
    if (!m_autoConflictStrategy) return;
    if (this->clickedButton() == nullptr)
    {
        *m_strategy = 2;
    }
    if (this->clickedButton() == dynamic_cast<QAbstractButton *>(buttonOverwrite))
    {
        *m_strategy = 0;
    }
    else if (this->clickedButton() == dynamic_cast<QAbstractButton *>(buttonCoexist))
    {
        *m_strategy = 1;
    }
    else
    {
        *m_strategy = 2;
    }
    if (checkAuto->isChecked())
    {
        *m_autoConflictStrategy = *m_strategy;
    }
}
