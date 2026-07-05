#include "trans_action.h"
#include <QDebug>
void Trans_Action::retranslate()
{
    QByteArray ctxData = m_context.isEmpty() ? QByteArray("Trans_Action") : m_context.toUtf8();
    QByteArray textData = m_originalText.toUtf8();
    this->setText(QCoreApplication::translate(ctxData, textData));
}
void Trans_Menu::retranslate()
{
    QByteArray ctxData = m_context.isEmpty() ? QByteArray("Trans_Menu") : m_context.toUtf8();
    QByteArray textData = m_originalText.toUtf8();
    this->setTitle(QCoreApplication::translate(ctxData, textData));
}
void Trans_PushButton::retranslate()
{
    QByteArray ctxData = m_context.isEmpty() ? QByteArray("Trans_PushButton") : m_context.toUtf8();
    QByteArray textData = m_originalText.toUtf8();
    this->setText(QCoreApplication::translate(ctxData, textData));
}
void Trans_CheckBox::retranslate()
{
    QByteArray ctxData = m_context.isEmpty() ? QByteArray("Trans_CheckBox") : m_context.toUtf8();
    QByteArray textData = m_originalText.toUtf8();
    this->setText(QCoreApplication::translate(ctxData, textData));
}
void Trans_Label::retranslate()
{
    QByteArray ctxData = m_context.isEmpty() ? QByteArray("Trans_Label") : m_context.toUtf8();
    QByteArray textData = m_originalText.toUtf8();
    this->setText(QCoreApplication::translate(ctxData, textData));
}
