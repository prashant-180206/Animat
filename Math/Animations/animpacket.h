#ifndef ANIMPACKET_H
#define ANIMPACKET_H

#include <QObject>
#include <QVector>
#include "animation.h"

class AnimPacket : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QString animationType READ animationType NOTIFY animationTypeChanged)
    Q_PROPERTY(int animationCount READ animationCount NOTIFY animationCountChanged)
    Q_PROPERTY(qreal startTime READ startTime WRITE setStartTime NOTIFY startTimeChanged)
    Q_PROPERTY(qreal duration READ duration NOTIFY durationChanged)

public:
    explicit AnimPacket(QObject *parent = nullptr, qreal startTime = 0);

    QString name() const { return m_name; }
    void setName(const QString &name)
    {
        if (m_name != name)
        {
            m_name = name;
            emit nameChanged();
        }
    }

    QString description() const { return m_description; }
    void setDescription(const QString &description)
    {
        if (m_description != description)
        {
            m_description = description;
            emit descriptionChanged();
        }
    }

    QString animationType() const
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

    int animationCount() const { return m_animations.size(); }

    void setStartTime(qreal startTime)
    {
        if (m_startTime != startTime)
        {
            m_startTime = startTime;
            emit startTimeChanged();
        }
    }

    qreal startTime() const
    {
        return m_startTime;
    }

    qreal duration() const
    {
        return m_duration;
    }

    // Add animation and update duration accordingly
    Q_INVOKABLE void addAnimation(const QString &type,
                                  ClickableMobject *mobj,
                                  QVariant startVal,
                                  QVariant targetVal,
                                  const QString &prop,
                                  qreal startOffset,
                                  qreal duration);

    // Update animations with current scene time
    void update(qreal sceneTime)
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

signals:
    void nameChanged();
    void descriptionChanged();
    void animationTypeChanged();
    void animationCountChanged();
    void startTimeChanged();
    void durationChanged();

private:
    QString m_name;
    QString m_description;
    qreal m_startTime;
    qreal m_duration;
    QVector<Animation *> m_animations;

    QString getAnimationTypeName(Animation *anim) const;
};

#endif // ANIMPACKET_H
