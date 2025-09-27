#ifndef ANIMATIONMANAGER_H
#define ANIMATIONMANAGER_H

#include "animpacket.h"
#include <QObject>
#include <QVector>


class AnimationManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(AnimPacket* activePacket READ activePacket NOTIFY activePacketChanged)
    Q_PROPERTY(AnimPacket* packetToAdd READ packetToAdd WRITE setPacketToAdd NOTIFY packetToAddChanged)

public:
    explicit AnimationManager(QObject *parent = nullptr)
        : QObject(parent), m_activePacket(nullptr)
    {
        m_packetToAdd = new AnimPacket(this);
    }

    // Add the packetToAdd to list and reset it to new instance
    Q_INVOKABLE void add()
    {
        if (!m_packetToAdd) {
            m_packetToAdd = new AnimPacket(this);
        }
        m_packets.append(m_packetToAdd);
        m_packetToAdd->setParent(this);

        m_packetToAdd->setStartTime(progressTime);
        progressTime+=m_packetToAdd->duration();

        // Optional: sort by start time
        std::sort(m_packets.begin(), m_packets.end(),
                  [](AnimPacket* a, AnimPacket* b){
                      return a->startTime() < b->startTime();
                  });

        setActivePacket(m_packetToAdd);

        // Create new packet for modification/editing
        m_packetToAdd = new AnimPacket(this);
        m_packetToAdd->setParent(this);
        qInfo()<<m_packets;
        emit packetToAddChanged();
    }

    AnimPacket* activePacket() const {
        return m_activePacket;
    }

    void setTime(qreal v) {
        AnimPacket* current = nullptr;

        for (AnimPacket* packet : std::as_const(m_packets)) {
            qreal start = packet->startTime();
            qreal end = start + packet->duration();

            if (v >= start && v <= end) {
                current = packet;
                break;  // found the active one
            }
        }

        // Update the active packet
        if (current) {
            if (m_activePacket != current) {
                setActivePacket(current); // triggers signal
            }
            current->update(v);
        }
    }


    AnimPacket* packetToAdd() const {
        return m_packetToAdd;
    }

    void setPacketToAdd(AnimPacket* packet) {
        if (m_packetToAdd == packet)
            return;
        m_packetToAdd = packet;
        emit packetToAddChanged();
    }

signals:
    void activePacketChanged();
    void packetToAddChanged();

private:
    QVector<AnimPacket*> m_packets;
    AnimPacket* m_activePacket;
    AnimPacket* m_packetToAdd;

    qreal progressTime=0;

    void setActivePacket(AnimPacket* packet)
    {
        if (m_activePacket == packet)
            return;
        m_activePacket = packet;
        emit activePacketChanged();
    }
};

#endif // ANIMATIONMANAGER_H
