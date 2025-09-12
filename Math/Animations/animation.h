#ifndef ANIMATION_H
#define ANIMATION_H

#include "Scene.h"
#include <QObject>
#include <QQmlEngine>

class Animation
{
public:
    Animation();

    void execute(Scene * canvas){
        // ClickableMobject * mobj = canvas->getMobject(m_obj_id);
        // auto val = new ValueTracker();
        // QObject::connect(val,&ValueTracker::valueChanged,mobj,[this,mobj](qreal v){
        //    auto func= TrackerManager::s_valueTrackerFuncs[m_prop];
        // });
    };

private:
    int m_obj_id;
    QString m_name;
    QString m_prop;
    QString m_target;
    QString m_func;
    qreal m_starttime;
    qreal m_duration;

};

#endif // ANIMATION_H
