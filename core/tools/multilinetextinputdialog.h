#ifndef MULTILINETEXTINPUTDIALOG_H
#define MULTILINETEXTINPUTDIALOG_H
#include "core/my_lineedit.h"

class MultiLineTextInputDialog : public QDialog
{
    Q_OBJECT
public:
    explicit MultiLineTextInputDialog(QWidget *parent = nullptr);
    QString getText() const;
    QStringList getLines() const;
    QDialog::DialogCode Setup(QString Title, QString Info, QString PlainText);
    void p_save(QSettings *settings, QString Token);
    void p_load(QSettings *settings, QString Token);
    void update_style(QColor theme_color, QColor theme_background_color, QColor theme_text_color, QColor select_text_color, QColor disabled_text_color, QString checked_icon_path);
    void set_icon(QString checked_icon_path);
private:
    Basic_TextEdit *m_textEdit = new Basic_TextEdit(this);
    QLabel *info_label = new QLabel(this);
};

#endif // MULTILINETEXTINPUTDIALOG_H
