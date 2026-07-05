#ifndef TRANS_ACTION_H
#define TRANS_ACTION_H
#include "trans_object.h"

#include <QAction>
class Trans_Action : public Trans_Object<QAction>
{
public:
    using Trans_Object<QAction>::Trans_Object;
    virtual void retranslate() override;
};

#include <QMenu>
class Trans_Menu : public Trans_Object<QMenu>
{
public:
    using Trans_Object<QMenu>::Trans_Object;
    virtual void retranslate() override;
};

#include <QPushButton>
class Trans_PushButton : public Trans_Object<QPushButton>
{
public:
    using Trans_Object<QPushButton>::Trans_Object;
    virtual void retranslate() override;
};

#include <QCheckBox>
class Trans_CheckBox : public Trans_Object<QCheckBox>
{
public:
    using Trans_Object<QCheckBox>::Trans_Object;
    virtual void retranslate() override;
};

#include <QLabel>
class Trans_Label : public Trans_Object<QLabel>
{
public:
    using Trans_Object<QLabel>::Trans_Object;
    virtual void retranslate() override;
};

#endif // TRANS_ACTION_H
