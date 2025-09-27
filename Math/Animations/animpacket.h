#ifndef ANIMPACKET_H
#define ANIMPACKET_H

#include <QObject>
#include <QVector>
#include "Animation.h"

class AnimPacket : public QObject
{
    Q_OBJECT

public:
    explicit AnimPacket(QObject* parent = nullptr, qreal startTime = 0);

    void setStartTime(qreal startTime) {
        m_startTime = startTime;
    }

    qreal startTime() const {
        return m_startTime;
    }

    qreal duration() const {
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
    void update(qreal sceneTime) {
        qreal localTime = sceneTime - m_startTime;
        if (localTime < 0) {
            localTime = 0;
        } else if (localTime > m_duration) {
            localTime = m_duration;
        }
        for (Animation* anim : std::as_const(m_animations)) {
            if(anim==nullptr) continue;
            anim->setLtime(localTime);
            anim->apply();
        }
    }

private:
    qreal m_startTime;
    qreal m_duration;
    QVector<Animation*> m_animations;
};

#endif // ANIMPACKET_H
