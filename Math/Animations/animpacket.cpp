#include "animpacket.h"

AnimPacket::AnimPacket(QObject *parent, qreal startTime)
    : QObject(parent), m_startTime(startTime), m_duration(0)
{
}

void AnimPacket::addAnimation(const QString &type,
                              ClickableMobject* mobj,
                              QVariant startVal,
                              QVariant targetVal,
                              const QString &prop,
                              qreal startOffset,
                              qreal duration)
{
    Animation* anim = nullptr;
    QString t = type.toLower(); // normalize for safety

    qInfo()<<"ADD ANIM CALLED";

    if (t == "move") {
        anim = new MoveAnimation(
            mobj,
            startVal.toPointF(),
            targetVal.toPointF(),
            startOffset, duration
            );
    }
    else if (t == "create") {
        anim = new CreateAnimation(mobj, startOffset, duration);
    }
    else if (t == "destroy") {
        anim = new DestroyAnimation(mobj, startOffset, duration);
    }
    else if (t == "customscalar") {
        qInfo()<<"SCALR ANIM CALLED";
        anim = new CustomScalarAnimation(
            mobj,
            prop,
            startVal.toReal(),
            targetVal.toReal(),
            startOffset, duration
            );
    }
    else if (t == "custompoint") {
        anim = new CustomPointAnimation(
            mobj,
            prop,
            startVal.toPointF(),
            targetVal.toPointF(),
            startOffset, duration
            );
    }
    else if (t == "value") {
        anim = new ValueAnimation(
            startVal.toReal(),
            targetVal.toReal(),
            startOffset, duration
            );
        if (mobj && !prop.isEmpty()) {
            static_cast<ValueAnimation*>(anim)->addTarget(mobj, prop);
        }
    }
    else {
        qWarning() << "Unknown animation type:" << type;
        return;
    }

    if (anim) {
        m_animations.append(anim);
        qreal endTime = anim->getStartOffset() + anim->getDuration();
        if (endTime > m_duration) {
            m_duration = endTime;
        }
    }
}

