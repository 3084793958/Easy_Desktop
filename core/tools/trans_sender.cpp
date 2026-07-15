#include "trans_sender.h"

Trans_Sender *Trans_Sender::m_instance = nullptr;
QMutex Trans_Sender::m_mutex;

Trans_Sender * Trans_Sender::instance()
{
    if (m_instance == nullptr)
    {
        QMutexLocker locker(&m_mutex);
        if (m_instance == nullptr)
        {
            m_instance = new Trans_Sender();
        }
    }
    return m_instance;
}
void Trans_Sender::cleanup()
{
    QMutexLocker locker(&m_mutex);
    if (m_instance)
    {
        m_instance->disconnect();
        delete m_instance;
        m_instance = nullptr;
    }
}

Trans_Sender::Trans_Sender()
    :QObject(nullptr)
{}

void Trans_Sender::Trans_All()
{
    Trans_Action::retranslateAll();
    Trans_Menu::retranslateAll();
    Trans_PushButton::retranslateAll();
    Trans_CheckBox::retranslateAll();
    Trans_Label::retranslateAll();
    emit instance()->Trans_sig();
}
