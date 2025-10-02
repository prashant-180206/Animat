#ifndef ANIMATIONMANAGER_H
#define ANIMATIONMANAGER_H

#include "animpacket.h"
#include <QObject>
#include <QVector>

// Doubly linked list node for AnimPacket
struct AnimPacketNode
{
    AnimPacket *packet;
    AnimPacketNode *next;
    AnimPacketNode *prev;

    explicit AnimPacketNode(AnimPacket *p) : packet(p), next(nullptr), prev(nullptr) {}

    ~AnimPacketNode()
    {
        // Note: We don't delete the packet here as it should be managed by Qt's parent-child system
        // The packet will be deleted by AnimationManager when appropriate
        packet = nullptr; // Just clear the pointer for safety
        next = prev = nullptr;
    }
};

class AnimationManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(AnimPacket *activePacket READ activePacket NOTIFY activePacketChanged)
    Q_PROPERTY(AnimPacket *packetToAdd READ packetToAdd WRITE setPacketToAdd NOTIFY packetToAddChanged)
    Q_PROPERTY(QVector<AnimPacket *> packets READ packets NOTIFY packetsChanged)

public:
    explicit AnimationManager(QObject *parent = nullptr);

    ~AnimationManager();

    // Add the packetToAdd to list and reset it to new instance
    Q_INVOKABLE void add();

    // Navigation methods for better animation control
    Q_INVOKABLE bool playNext();

    Q_INVOKABLE bool playPrevious();

    Q_INVOKABLE void jumpToFirst();

    Q_INVOKABLE void jumpToLast();

    Q_INVOKABLE void jumpToIndex(int index);

    Q_INVOKABLE int getCurrentIndex() const;

    Q_INVOKABLE int size() const { return m_size; }

    // Safety and debugging methods
    Q_INVOKABLE bool validateList() const;

    Q_INVOKABLE void debugPrintList() const;

    AnimPacket *activePacket() const;

    void setTime(qreal v);

    AnimPacket *packetToAdd() const;

    void setPacketToAdd(AnimPacket *packet);

    QVector<AnimPacket *> packets() const;

    Q_INVOKABLE void removePacket(AnimPacket *packet);

    struct AnimManagerData
    {
        int size = 0;
        qreal progressTime = 0;
        QString activePacketName = "";
        QVector<QJsonObject> packetJsons; // Store serialized packets
        QJsonDocument toJson() const
        {
            QJsonObject o;
            o["size"] = size;
            o["progressTime"] = progressTime;
            o["activePacketName"] = activePacketName;
            QJsonArray packetArray;
            for (const QJsonObject &packetObj : packetJsons)
            {
                packetArray.append(packetObj);
            }
            o["packets"] = packetArray;
            return QJsonDocument(o);
        }

        static AnimManagerData fromJSON(const QJsonObject &o)
        {
            AnimManagerData d;
            d.size = o["size"].toInt();
            d.progressTime = o["progressTime"].toDouble();
            d.activePacketName = o["activePacketName"].toString();
            QJsonArray packetArray = o["packets"].toArray();
            for (const QJsonValue &val : std::as_const(packetArray))
            {
                if (val.isObject())
                {
                    d.packetJsons.append(val.toObject());
                }
            }
            return d;
        }
    };

    AnimManagerData getData() const
    {

        AnimManagerData d;
        d.size = m_size;
        d.progressTime = progressTime;
        d.activePacketName = m_activePacket ? m_activePacket->name() : "";
        AnimPacketNode *node = m_head;
        while (node)
        {
            if (node->packet)
            {
                d.packetJsons.append(node->packet->getData().toJSON());
            }
            node = node->next;
        }
        return d;
    }

    void setFromJSON(const QJsonObject &o)
    {
        AnimManagerData d = AnimManagerData::fromJSON(o);
        clearList();
        progressTime = d.progressTime;

        AnimPacket *targetActivePacket = nullptr;

        for (const QJsonObject &packetObj : std::as_const(d.packetJsons))
        {
            AnimPacket *packet = new AnimPacket(this);
            packet->setFromJSON(packetObj);
            insertSorted(packet);

            qInfo() << packetObj << &packetObj << "PACKET";

            // Remember which packet should be active, but don't set it yet
            if (packet->name() == d.activePacketName)
            {
                targetActivePacket = packet;
            }
        }

        // Now set the active packet without calling add()
        if (targetActivePacket)
        {
            setActivePacket(targetActivePacket);
        }

        // Emit signals to notify of changes
        emit packetsChanged();
        emit activePacketChanged();
    }

signals:
    void activePacketChanged();
    void packetToAddChanged();
    void packetsChanged();

private:
    // Doubly linked list pointers
    AnimPacketNode *m_head;
    AnimPacketNode *m_tail;
    AnimPacketNode *m_activeNode;
    int m_size;

    // Original members
    AnimPacket *m_activePacket;
    AnimPacket *m_packetToAdd;
    qreal progressTime = 0;

    // Linked list operations
    void insertSorted(AnimPacket *packet);

    void removeNode(AnimPacketNode *node);

    AnimPacketNode *findNode(AnimPacket *packet);

    AnimPacketNode *getNodeAt(int index);

    void clearList();

    void setActivePacket(AnimPacket *packet);

    void setActiveNode(AnimPacketNode *node);
};

#endif // ANIMATIONMANAGER_H
