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
    Q_INVOKABLE void addAnimation(const QString &type,
                                  ClickableMobject *mobj,
                                  QVariant startVal,
                                  QVariant targetVal,
                                  const QString &prop,
                                  qreal duration,
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
        QJsonObject toJSON() const
        {
            QJsonObject obj;
            obj["type"] = type;
            obj["targetName"] = targetName;
            obj["property"] = property;
            obj["startValue"] = QJsonValue::fromVariant(startValue);
            obj["targetValue"] = QJsonValue::fromVariant(targetValue);
            obj["duration"] = duration;
            obj["easingType"] = easingType;
            return obj;
        }

        static AnimInfo fromJSON(const QJsonObject &obj)
        {
            AnimInfo info;
            info.type = obj.value("type").toString();
            info.targetName = obj.value("targetName").toString();
            info.property = obj.value("property").toString();
            info.startValue = obj.value("startValue").toVariant();
            info.targetValue = obj.value("targetValue").toVariant();
            info.duration = obj.value("duration").toDouble();
            info.easingType = obj.value("easingType").toString();
            return info;
        }
    };

    struct AnimpacketData
    {
        QString name;
        QString description;
        qreal startTime;
        qreal duration;
        QVector<AnimInfo> animations;
        QJsonObject toJSON() const;

        static AnimpacketData fromJSON(const QJsonObject &obj);
    };

    AnimpacketData getData() const;

    void setFromJSON(const QJsonObject &obj)
    {
        AnimpacketData data = AnimpacketData::fromJSON(obj);
        m_name = data.name;
        m_description = data.description;
        m_startTime = data.startTime;
        m_duration = data.duration;
        m_animations.clear();
        m_animInfos.clear();
        for (const AnimInfo &anim : std::as_const(data.animations))
        {
            // Note: You would need to find the ClickableMobject by name in your scene
            ClickableMobject *mobj = nullptr; // Replace with actual lookup
            if (!anim.targetName.isEmpty())
            {
                // TODO: Lookup mobj by name, if available
            }
            addAnimation(anim.type, mobj, anim.startValue, anim.targetValue, anim.property, anim.duration, anim.easingType);
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
    QVector<AnimInfo> m_animInfos;

    QString getAnimationTypeName(Animation *anim) const;
};

#endif // ANIMPACKET_H
