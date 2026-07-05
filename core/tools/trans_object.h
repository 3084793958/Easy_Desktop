#ifndef TRANS_OBJECT_H
#define TRANS_OBJECT_H

#include <QObject>
#include <QCoreApplication>
#include <QList>

template<typename T>
class Trans_Object : public T
{
public:
    Trans_Object(const QString, const QString &text, const QString &context = QString(), QWidget *parent = nullptr);
    virtual ~Trans_Object();

    virtual void retranslate();
    static void retranslateAll();
protected:
    QString m_originalText;
    QString m_context;
private:
    static QList<T *> s_instances;
};


template<typename T>
QList<T *> Trans_Object<T>::s_instances;

template<typename T>
Trans_Object<T>::Trans_Object(const QString t, const QString &text, const QString &context, QWidget *parent)
    : T(t, parent)
    , m_originalText(text)
    , m_context(context)
{
    s_instances.append(static_cast<T *>(this));
    retranslate();
}

template<typename T>
Trans_Object<T>::~Trans_Object()
{
    s_instances.removeAll(static_cast<T *>(this));
}

template<typename T>
void Trans_Object<T>::retranslate()
{}

template<typename T>
void Trans_Object<T>::retranslateAll()
{
    for (T *obj : s_instances)
    {
        if (auto *t = dynamic_cast<Trans_Object<T> *>(obj))
        {
            t->retranslate();
        }
    }
}


#endif // TRANS_OBJECT_H
