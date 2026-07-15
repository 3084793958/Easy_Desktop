#ifndef TRANS_SENDER_H
#define TRANS_SENDER_H

#include "trans_action.h"
#include <QMutex>

class Trans_Sender : public QObject
{
    Q_OBJECT
public:
    explicit Trans_Sender();
    static void Trans_All();

    static Trans_Sender *instance();
    static void cleanup();

    static Trans_Sender *m_instance;
    static QMutex m_mutex;
signals:
    void Trans_sig();
};

#endif // TRANS_SENDER_H
