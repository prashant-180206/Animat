#ifndef ANIMPACKET_H
#define ANIMPACKET_H

#include <QObject>
#include <QVector>
#include "animation.h"
#include <QJsonArray>

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
    void setName(const QString &name);

    QString description() const { return m_description; }
    void setDescription(const QString &description);

    QString animationType() const;

    int animationCount() const { return m_animations.size(); }

    void setStartTime(qreal startTime);

    qreal startTime() const;

    qreal duration() const
    {
        return m_duration;
    }

    // Add animation and update duration accordingly
    Q_INVOKABLE void addAnimation(
        const QString &type = "Wait",
        ClickableMobject *mobj = nullptr,
        QVariant startVal = 0,
        QVariant targetVal = 0,
        const QString &prop = "",
        qreal duration = 1,
        const QString &easingType = "InOutQuad");

    // Update animations with current scene time
    void update(qreal sceneTime);

    struct AnimInfo
    {
        QString type;
        QString targetName;
        QString property;
        QVariant startValue;
        QVariant targetValue;
        qreal duration;
        QString easingType;
        QJsonObject toJSON() const;
        static AnimInfo fromJSON(const QJsonObject &obj);
    };

    struct AnimpacketData
    {
        QString name;
        QString description;
        qreal startTime;
        qreal duration;
        QVector<AnimInfo> animations;
        QJsonObject toJSON() const;
        static AnimpacketData fromJSON(const QJsonObject &obj);;
    };

    AnimpacketData getData() const
    {
        AnimpacketData d;
        d.duration = duration();
        d.description = description();
        d.name = name();
        d.startTime = startTime();
        d.animations = m_animInfos;
        return d;
    }

    void setFromJSON(const QJsonObject &obj, Scene *c);

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
    QVector<AnimInfo> m_animInfos;

    QString getAnimationTypeName(Animation *anim) const;
};

#endif // ANIMPACKET_H
