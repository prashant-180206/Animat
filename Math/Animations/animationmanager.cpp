#include "animationmanager.h"

AnimationManager::AnimationManager(QObject *parent)
    : QObject(parent), m_activePacket(nullptr), m_head(nullptr), m_tail(nullptr), m_activeNode(nullptr), m_size(0)
{
    m_packetToAdd = new AnimPacket(this);
}

AnimationManager::~AnimationManager()
{
    clearList();
}

void AnimationManager::add()
{
    if (!m_packetToAdd)
    {
        m_packetToAdd = new AnimPacket(this);
    }

    // Ensure the packet has this manager as parent before adding to list
    m_packetToAdd->setParent(this);
    m_packetToAdd->setStartTime(progressTime);
    progressTime += m_packetToAdd->duration();

    // Insert in sorted order by start time
    insertSorted(m_packetToAdd);

    setActivePacket(m_packetToAdd);

    // Create new packet for modification/editing
    m_packetToAdd = new AnimPacket(this);

    qInfo() << "Added packet to doubly linked list, size:" << m_size;
    emit packetToAddChanged();
    emit packetsChanged();
}

bool AnimationManager::playNext()
{
    if (!m_activeNode || !m_activeNode->next)
        return false;

    setActiveNode(m_activeNode->next);
    return true;
}

bool AnimationManager::playPrevious()
{
    if (!m_activeNode || !m_activeNode->prev)
        return false;

    setActiveNode(m_activeNode->prev);
    return true;
}

void AnimationManager::jumpToFirst()
{
    if (m_head)
        setActiveNode(m_head);
}

void AnimationManager::jumpToLast()
{
    if (m_tail)
        setActiveNode(m_tail);
}

void AnimationManager::jumpToIndex(int index)
{
    AnimPacketNode *node = getNodeAt(index);
    if (node)
        setActiveNode(node);
}

int AnimationManager::getCurrentIndex() const
{
    if (!m_activeNode)
        return -1;

    int index = 0;
    AnimPacketNode *current = m_head;
    while (current && current != m_activeNode)
    {
        current = current->next;
        index++;
    }
    return current ? index : -1;
}

bool AnimationManager::validateList() const
{
    int count = 0;
    AnimPacketNode *node = m_head;

    while (node)
    {
        count++;
        // Check parent ownership
        if (node->packet && node->packet->parent() != this)
        {
            qWarning() << "AnimPacket with incorrect parent found in list!";
            return false;
        }

        // Check forward/backward links consistency
        if (node->next && node->next->prev != node)
        {
            qWarning() << "Inconsistent forward/backward links detected!";
            return false;
        }

        node = node->next;
    }

    if (count != m_size)
    {
        qWarning() << "List size mismatch: counted" << count << "but size is" << m_size;
        return false;
    }

    return true;
}

void AnimationManager::debugPrintList() const
{
    qInfo() << "=== AnimPacket List Debug ===";
    qInfo() << "Size:" << m_size;
    qInfo() << "Head:" << (m_head ? "exists" : "null");
    qInfo() << "Tail:" << (m_tail ? "exists" : "null");
    qInfo() << "Active:" << (m_activeNode ? "exists" : "null");

    AnimPacketNode *node = m_head;
    int index = 0;
    while (node && index < 10) // Limit to 10 for debugging
    {
        qInfo() << "Node" << index << ":"
                << "packet=" << node->packet
                << "parent=" << (node->packet ? node->packet->parent() : nullptr)
                << "startTime=" << (node->packet ? node->packet->startTime() : -1);
        node = node->next;
        index++;
    }

    if (node)
    {
        qInfo() << "... (more nodes exist)";
    }
}

AnimPacket *AnimationManager::activePacket() const
{
    return m_activePacket;
}

void AnimationManager::setTime(qreal v)
{
    AnimPacketNode *current = nullptr;

    // Search through linked list for the active packet at time v
    AnimPacketNode *node = m_head;
    while (node)
    {
        qreal start = node->packet->startTime();
        qreal end = start + node->packet->duration();

        if (v >= start && v <= end)
        {
            current = node;
            break; // found the active one
        }
        node = node->next;
    }

    // Update the active packet
    if (current)
    {
        if (m_activeNode != current)
        {
            setActiveNode(current); // triggers signal
        }
        current->packet->update(v);
    }
}

AnimPacket *AnimationManager::packetToAdd() const
{
    return m_packetToAdd;
}

void AnimationManager::setPacketToAdd(AnimPacket *packet)
{
    if (m_packetToAdd == packet)
        return;
    m_packetToAdd = packet;
    emit packetToAddChanged();
}

QVector<AnimPacket *> AnimationManager::packets() const
{
    QVector<AnimPacket *> result;
    AnimPacketNode *node = m_head;
    while (node)
    {
        result.append(node->packet);
        node = node->next;
    }
    return result;
}

void AnimationManager::removePacket(AnimPacket *packet)
{
    AnimPacketNode *node = findNode(packet);
    if (node)
    {
        removeNode(node);
        emit packetsChanged();
    }
}

void AnimationManager::setFromJSON(const QJsonObject &o, Scene *c)
{
    AnimManagerData d = AnimManagerData::fromJSON(o);
    clearList();
    progressTime = d.progressTime;

    AnimPacket *targetPacket = nullptr;

    for (const QJsonObject &packetObj : std::as_const(d.packetJsons))
    {
        AnimPacket *packet = new AnimPacket(this);
        packet->setFromJSON(packetObj,c);

        qInfo() << packetObj << &packetObj << "PACKET";
        targetPacket = packet;
        setPacketToAdd(targetPacket);
        add();
    }

    // Now set the active packet without calling add()
    if (targetPacket)
        emit packetsChanged();
    emit activePacketChanged();
}

AnimationManager::AnimManagerData AnimationManager::getData() const
{

    AnimManagerData d;
    d.size = m_size;
    d.progressTime = 0;
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




// AnimationManager::AnimManagerData AnimationManager::getData() const
// {
//     AnimManagerData d;
//     d.size = m_size;
//     d.progressTime = progressTime;
//     d.activePacketName = m_activePacket ? m_activePacket->name() : "";
//     AnimPacketNode *node = m_head;
//     while (node)
//     {
//         if (node->packet)
//         {
//             d.packetJsons.append(node->packet->getData().toJSON());
//         }
//         node = node->next;
//     }
//     return d;
// }

void AnimationManager::insertSorted(AnimPacket *packet)
{
    // Ensure packet has this manager as parent for proper memory management
    if (packet->parent() != this)
    {
        packet->setParent(this);
    }

    AnimPacketNode *newNode = new AnimPacketNode(packet);

    // If list is empty
    if (!m_head)
    {
        m_head = m_tail = newNode;
        m_size++;
        return;
    } // Find insertion point (sorted by start time)
    AnimPacketNode *current = m_head;
    while (current && current->packet->startTime() < packet->startTime())
    {
        current = current->next;
    }

    // Insert at the beginning
    if (current == m_head)
    {
        newNode->next = m_head;
        m_head->prev = newNode;
        m_head = newNode;
    }
    // Insert at the end
    else if (!current)
    {
        newNode->prev = m_tail;
        m_tail->next = newNode;
        m_tail = newNode;
    }
    // Insert in the middle
    else
    {
        newNode->next = current;
        newNode->prev = current->prev;
        current->prev->next = newNode;
        current->prev = newNode;
    }

    m_size++;
}

void AnimationManager::removeNode(AnimPacketNode *node)
{
    if (!node)
        return;

    // Update links
    if (node->prev)
        node->prev->next = node->next;
    else
        m_head = node->next;

    if (node->next)
        node->next->prev = node->prev;
    else
        m_tail = node->prev;

    // If we're removing the active node, clear it
    if (m_activeNode == node)
    {
        setActiveNode(nullptr);
    }

    // Safe packet deletion: only delete if we own it
    if (node->packet && node->packet->parent() == this)
    {
        node->packet->deleteLater();
    }

    delete node;
    m_size--;
}

AnimPacketNode *AnimationManager::findNode(AnimPacket *packet)
{
    AnimPacketNode *current = m_head;
    while (current)
    {
        if (current->packet == packet)
            return current;
        current = current->next;
    }
    return nullptr;
}

AnimPacketNode *AnimationManager::getNodeAt(int index)
{
    if (index < 0 || index >= m_size)
        return nullptr;

    AnimPacketNode *current = m_head;
    for (int i = 0; i < index && current; i++)
    {
        current = current->next;
    }
    return current;
}

void AnimationManager::clearList()
{
    // Clear active references first
    setActiveNode(nullptr);

    while (m_head)
    {
        AnimPacketNode *next = m_head->next;

        // Safe packet deletion: only delete if we own it
        if (m_head->packet && m_head->packet->parent() == this)
        {
            m_head->packet->deleteLater();
        }

        delete m_head;
        m_head = next;
    }
    m_tail = m_activeNode = nullptr;
    m_size = 0;
}

void AnimationManager::setActivePacket(AnimPacket *packet)
{
    if (m_activePacket == packet)
        return;
    m_activePacket = packet;
    emit activePacketChanged();
}

void AnimationManager::setActiveNode(AnimPacketNode *node)
{
    if (m_activeNode == node)
        return;

    m_activeNode = node;
    AnimPacket *packet = node ? node->packet : nullptr;
    setActivePacket(packet);
}

// QJsonDocument AnimationManager::AnimManagerData::toJson() const
// {
//     QJsonObject o;
//     o["size"] = size;
//     o["progressTime"] = progressTime;
//     o["activePacketName"] = activePacketName;
//     QJsonArray packetArray;
//     for (const QJsonObject &packetObj : packetJsons)
//     {
//         packetArray.append(packetObj);
//     }
//     o["packets"] = packetArray;
//     return QJsonDocument(o);
// }

// AnimationManager::AnimManagerData AnimationManager::AnimManagerData::fromJSON(const QJsonObject &o)
// {
//     AnimManagerData d;
//     d.size = o["size"].toInt();
//     d.progressTime = o["progressTime"].toDouble();
//     d.activePacketName = o["activePacketName"].toString();
//     if (o.contains("packets") && o["packets"].isArray())
//     {
//         QJsonArray arr = o["packets"].toArray();
//         for (const QJsonValue &val : arr)
//         {
//             if (val.isObject())
//                 d.packetJsons.append(val.toObject());
//         }
//     }
//     return d;
// }

QJsonDocument AnimationManager::AnimManagerData::toJson() const
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

AnimationManager::AnimManagerData AnimationManager::AnimManagerData::fromJSON(const QJsonObject &o)
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
