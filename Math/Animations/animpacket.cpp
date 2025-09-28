#include "animpacket.h"

AnimPacket::AnimPacket(QObject *parent, qreal startTime)
    : QObject(parent), m_startTime(startTime), m_duration(0)
{
    // Set default name based on creation time or index
    static int packetCounter = 1;
    m_name = QString("Animation Packet %1").arg(packetCounter++);
    m_description = "Animation sequence";
}

void AnimPacket::addAnimation(const QString &type,
                              ClickableMobject *mobj,
                              QVariant startVal,
                              QVariant targetVal,
                              const QString &prop,
                              qreal startOffset,
                              qreal duration)
{
    Animation *anim = nullptr;
    QString t = type.toLower(); // normalize for safety

    qInfo() << "ADD ANIM CALLED";

    if (t == "move")
    {
        anim = new MoveAnimation(
            mobj,
            startVal.toPointF(),
            targetVal.toPointF(),
            startOffset, duration);
    }
    else if (t == "create")
    {
        anim = new CreateAnimation(mobj, startOffset, duration);
    }
    else if (t == "destroy")
    {
        anim = new DestroyAnimation(mobj, startOffset, duration);
    }
    else if (t == "customscalar")
    {
        qInfo() << "SCALR ANIM CALLED";
        anim = new CustomScalarAnimation(
            mobj,
            prop,
            startVal.toReal(),
            targetVal.toReal(),
            startOffset, duration);
    }
    else if (t == "custompoint")
    {
        anim = new CustomPointAnimation(
            mobj,
            prop,
            startVal.toPointF(),
            targetVal.toPointF(),
            startOffset, duration);
    }
    else if (t == "value")
    {
        anim = new ValueAnimation(
            startVal.toReal(),
            targetVal.toReal(),
            startOffset, duration);
        if (mobj && !prop.isEmpty())
        {
            static_cast<ValueAnimation *>(anim)->addTarget(mobj, prop);
        }
    }
    else if (t == "wait")
    {
        anim = new WaitAnimation(startOffset, duration);
    }
    else
    {
        qWarning() << "Unknown animation type:" << type;
        return;
    }

    if (anim)
    {
        m_animations.append(anim);
        qreal endTime = anim->getStartOffset() + anim->getDuration();
        if (endTime > m_duration)
        {
            m_duration = endTime;
            emit durationChanged();
        }
        emit animationCountChanged();
        emit animationTypeChanged();

        // Update description to include the animation type if it's still the default
        if (m_description == "Animation sequence" && m_animations.size() == 1)
        {
            m_description = QString("%1 animation").arg(type);
            emit descriptionChanged();
        }
        else if (m_animations.size() > 1 && m_description.contains("animation") && !m_description.contains("Mixed"))
        {
            m_description = "Mixed animation sequence";
            emit descriptionChanged();
        }
    }
}

QString AnimPacket::getAnimationTypeName(Animation *anim) const
{
    if (!anim)
        return "Unknown";

    // Use dynamic_cast to determine the animation type
    if (dynamic_cast<MoveAnimation *>(anim))
        return "Move";
    else if (dynamic_cast<CreateAnimation *>(anim))
        return "Create";
    else if (dynamic_cast<DestroyAnimation *>(anim))
        return "Destroy";
    else if (dynamic_cast<CustomScalarAnimation *>(anim))
        return "Scalar";
    else if (dynamic_cast<CustomPointAnimation *>(anim))
        return "Point";
    else if (dynamic_cast<ValueAnimation *>(anim))
        return "Value";
    else if (dynamic_cast<WaitAnimation *>(anim))
        return "Wait";
    else
        return "Unknown";
}
