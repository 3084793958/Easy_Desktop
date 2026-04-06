#ifndef FILE_CONTROL_H
#define FILE_CONTROL_H
#include <QFile>
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <QMessageBox>
#include <QCheckBox>
struct File_Control
{
    static bool CopyWithCopyFileRange(const QString &srcPath, const QString &dstPath);
    static bool CopyRecursively(const QString &src, const QString &dst);
    static void Copy_File(const QString &srcPath, QString dstPath, const bool &cut, int *autoConflictStrategy);
    static QString FilenameForBash(QString filename);
};
class File_MessageBox : public QMessageBox
{
public:
    explicit File_MessageBox(QWidget *parent, QString filename, int *strategy, int *autoConflictStrategy);
    void Setup();
private:
    QCheckBox *checkAuto = new QCheckBox("记住我的选择，以后自动处理", this);
    QPushButton *buttonOverwrite = nullptr;
    QPushButton *buttonCoexist = nullptr;
    QPushButton *buttonCancel = nullptr;
    int *m_strategy = nullptr;
    int *m_autoConflictStrategy = nullptr;
};
#endif // FILE_CONTROL_H
