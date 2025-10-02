#include "animpacket.h"
#include "animTypes/MoveAnimation.h"
#include "animTypes/CreateAnimation.h"
#include "animTypes/DestroyAnimation.h"
#include "animTypes/CustomScalarAnimation.h"
#include "animTypes/CustomPointAnimation.h"
#include "animTypes/ValueAnimation.h"
#include "animTypes/WaitAnimation.h"

AnimPacket::AnimPacket(QObject *parent, qreal startTime)
    : QObject(parent), m_startTime(startTime), m_duration(0)
{
    // Set default name based on creation time or index
    static int packetCounter = 1;
    m_name = QString("Animation Packet %1").arg(packetCounter++);
    m_description = "Animation sequence";
}

void AnimPacket::setName(const QString &name)
{
    if (m_name != name)
    {
        m_name = name;
        emit nameChanged();
    }
}

void AnimPacket::setDescription(const QString &description)
{
    if (m_description != description)
    {
        m_description = description;
        emit descriptionChanged();
    }
}

QString AnimPacket::animationType() const
{
    if (m_animations.isEmpty())
        return "Empty";
    if (m_animations.size() == 1)
    {
        // Return single animation type based on the animation class
        return getAnimationTypeName(m_animations.first());
    }
    return "Mixed"; // Multiple different types
}

void AnimPacket::setStartTime(qreal startTime)
{
    if (m_startTime != startTime)
    {
        m_startTime = startTime;
        emit startTimeChanged();
    }
}

qreal AnimPacket::startTime() const
{
    return m_startTime;
}

void AnimPacket::addAnimation(const QString &type,
                              ClickableMobject *mobj,
                              QVariant startVal,
                              QVariant targetVal,
                              const QString &prop,
                              qreal duration,
                              const QString &easingType)
{
    Animation *anim = nullptr;
    QString t = type.toLower(); // normalize for safety

    // Convert easing type string to QEasingCurve::Type
    QEasingCurve::Type easingCurve = QEasingCurve::InOutQuad; // default
    if (easingType == "Linear")
        easingCurve = QEasingCurve::Linear;
    else if (easingType == "InQuad")
        easingCurve = QEasingCurve::InQuad;
    else if (easingType == "OutQuad")
        easingCurve = QEasingCurve::OutQuad;
    else if (easingType == "InOutQuad")
        easingCurve = QEasingCurve::InOutQuad;
    else if (easingType == "InBounce")
        easingCurve = QEasingCurve::InBounce;
    else if (easingType == "OutBounce")
        easingCurve = QEasingCurve::OutBounce;

    qInfo() << "ADD ANIM CALLED";

    if (t == "move")
    {
        anim = new MoveAnimation(
            mobj,
            targetVal.toPointF(),
            duration, easingCurve);
    }
    else if (t == "create")
    {
        anim = new CreateAnimation(mobj, duration, easingCurve);
    }
    else if (t == "destroy")
    {
        anim = new DestroyAnimation(mobj, duration, easingCurve);
    }
    else if (t == "customscalar")
    {
        qInfo() << "SCALR ANIM CALLED";
        anim = new CustomScalarAnimation(
            mobj,
            prop,
            targetVal.toReal(),
            duration, easingCurve);
    }
    else if (t == "custompoint")
    {
        anim = new CustomPointAnimation(
            mobj,
            prop,
            targetVal.toPointF(),
            duration, easingCurve);
    }
    else if (t == "value")
    {
        anim = new ValueAnimation(
            startVal.toReal(),
            targetVal.toReal(),
            duration, easingCurve);
        if (mobj && !prop.isEmpty())
        {
            static_cast<ValueAnimation *>(anim)->addTarget(mobj, prop);
        }
    }
    else if (t == "wait")
    {
        anim = new WaitAnimation(duration, easingCurve);
    }
    else
    {
        qWarning() << "Unknown animation type:" << type;
        return;
    }

    if (anim)
    {
        m_animations.append(anim);
        qreal endTime = anim->getDuration();
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
        m_animInfos.append({type, mobj ? mobj->getProperties()->base()->name() : "N/A", prop, startVal, targetVal, duration, easingType});
    }
}

void AnimPacket::update(qreal sceneTime)
{
    qreal localTime = sceneTime - m_startTime;
    if (localTime < 0)
    {
        localTime = 0;
    }
    else if (localTime > m_duration)
    {
        localTime = m_duration;
    }
    for (Animation *anim : std::as_const(m_animations))
    {
        if (anim == nullptr)
            continue;
        anim->setLtime(localTime);
        anim->apply();
    }
}



AnimPacket::AnimpacketData AnimPacket::getData() const
{
    AnimpacketData data;
    data.name = m_name;
    data.description = m_description;
    data.startTime = m_startTime;
    data.duration = m_duration;
    data.animations = m_animInfos;
    return data;
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

QJsonObject AnimPacket::AnimpacketData::toJSON() const
{
    QJsonObject obj;
    obj["name"] = name;
    obj["description"] = description;
    obj["startTime"] = startTime;
    obj["duration"] = duration;

    QJsonArray animArray;
    for (const AnimInfo &anim : animations)
    {
        animArray.append(anim.toJSON());
    }
    obj["animations"] = animArray;
    return obj;
}

AnimPacket::AnimpacketData AnimPacket::AnimpacketData::fromJSON(const QJsonObject &obj)
{
    AnimpacketData data;
    data.name = obj.value("name").toString();
    data.description = obj.value("description").toString();
    data.startTime = obj.value("startTime").toDouble();
    data.duration = obj.value("duration").toDouble();
    if (obj.contains("animations") && obj["animations"].isArray())
    {
        QJsonArray animArray = obj["animations"].toArray();
        for (const QJsonValue &val : animArray)
        {
            if (val.isObject())
            {
                data.animations.append(AnimInfo::fromJSON(val.toObject()));
            }
        }
    }
    return data;
}
