#ifndef ANIMATION_H
#define ANIMATION_H

#include "Scene.h"
#include "funcmap.h"
#include <QObject>
#include <QQmlEngine>

class Animation
{
public:
    Animation();

    void execute(Scene * canvas){
        ClickableMobject * mobj = canvas->getMobject(m_obj_id);
        auto val = new ValueTracker();
        QObject::connect(val, &ValueTracker::valueChanged, mobj, [this,mobj](qreal v){
            auto func = FuncMap::ConnectFunc[m_prop];
            if (!func) return;
            func(mobj,v)
        });
        val->updateVal(m_target.toDouble());

        // Disconnect all connections originating from val
        QObject::disconnect(val, nullptr, nullptr, nullptr);

        val->deleteLater();
        val = nullptr; // optional, since val is local
    }


private:
    QString m_obj_id;
    QString m_name;
    QString m_prop;
    QString m_target;
    QString m_func;
    qreal m_starttime;
    qreal m_duration;

};

#endif // ANIMATION_H
