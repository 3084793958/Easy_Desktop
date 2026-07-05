#include "multilinetextinputdialog.h"

MultiLineTextInputDialog::MultiLineTextInputDialog(QWidget *parent)
    :QDialog(parent)
{
    setVisible(false);
    setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
    setModal(true);
    m_textEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(info_label);
    layout->addWidget(m_textEdit, 1);
    layout->addWidget(buttonBox, 0);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->setSpacing(6);
}
QString MultiLineTextInputDialog::getText() const
{
    return m_textEdit->toPlainText();
}
QStringList MultiLineTextInputDialog::getLines() const
{
    return getText().split('\n', Qt::SkipEmptyParts);
}
QDialog::DialogCode MultiLineTextInputDialog::Setup(QString Title, QString Info, QString PlainText)
{
    setWindowTitle(Title);
    info_label->setText(Info);
    m_textEdit->setText(PlainText);
    setVisible(true);
    int res = this->exec();
    setVisible(false);
    return static_cast<QDialog::DialogCode>(res);
}
void MultiLineTextInputDialog::p_save(QSettings *settings, QString Token)
{
    m_textEdit->H_save(settings, Token);
}
void MultiLineTextInputDialog::p_load(QSettings *settings, QString Token)
{
    m_textEdit->H_load(settings, Token);
}
void MultiLineTextInputDialog::p_save_no_text(QSettings *settings, QString Token)
{
    m_textEdit->H_save_no_text(settings, Token);
}
void MultiLineTextInputDialog::p_load_no_text(QSettings *settings, QString Token)
{
    m_textEdit->H_load_no_text(settings, Token);
}
void MultiLineTextInputDialog::update_style(QColor theme_color, QColor theme_background_color, QColor theme_text_color, QColor select_text_color, QColor disabled_text_color, QString checked_icon_path)
{
    m_textEdit->setStyleSheet(QString("QMenu{border-radius:10px 10px;background:rgba(%1,%2,%3,%4);margin:0px -1px 0px -1px;padding-top:8px;padding-bottom:8px;icon-size:20px;border-radius:10px 10px}"
                                      "QMenu::item{color:rgba(%5,%6,%7,%8);background:rgba(0,0,0,0);}"
                                      "QMenu::item:disabled{color:rgba(%9,%10,%11,%12)}"
                                      "QMenu::item:selected{color:rgba(%13,%14,%15,%16);background:rgba(%17,%18,%19,%20)}"
                                      "QMenu::indicator{width: 20px; height 20px; background: transparent;}"
                                      "QMenu::indicator:checked{image: url(%21);}"
                                      "QMenu::separator{background:rgba(150,150,150,125)}")
                              .arg(theme_background_color.red()).arg(theme_background_color.green()).arg(theme_background_color.blue()).arg(theme_background_color.alpha())
                              .arg(theme_text_color.red()).arg(theme_text_color.green()).arg(theme_text_color.blue()).arg(theme_text_color.alpha())
                              .arg(disabled_text_color.red()).arg(disabled_text_color.green()).arg(disabled_text_color.blue()).arg(disabled_text_color.alpha())
                              .arg(select_text_color.red()).arg(select_text_color.green()).arg(select_text_color.blue()).arg(select_text_color.alpha())
                              .arg(theme_color.red()).arg(theme_color.green()).arg(theme_color.blue()).arg(theme_color.alpha())
                              .arg(checked_icon_path));
}
void MultiLineTextInputDialog::set_icon(QString checked_icon_path)
{
    m_textEdit->set_icon(checked_icon_path);
}
