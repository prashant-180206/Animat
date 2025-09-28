#include "playbackslider.h"
#include "../Animations/animationmanager.h"
#include <QDebug>

void PlaybackSlider::setAnimationManager(AnimationManager *manager)
{
    if (m_animationManager == manager)
        return;

    // Disconnect from old manager
    if (m_animationManager)
    {
        disconnect(m_animationManager, &AnimationManager::packetsChanged, this, &PlaybackSlider::onAnimationsChanged);
    }

    m_animationManager = manager;

    // Connect to new manager
    if (m_animationManager)
    {
        connect(m_animationManager, &AnimationManager::packetsChanged, this, &PlaybackSlider::onAnimationsChanged);
    }

    emit animationManagerChanged();
    emit maxDurationChanged();
}

qreal PlaybackSlider::calculateMaxDuration() const
{
    if (!m_animationManager)
    {
        return m_minDuration;
    }

    // Find the latest end time among all animation packets
    qreal maxEndTime = 0;
    QVector<AnimPacket *> packets = m_animationManager->packets();

    for (AnimPacket *packet : packets)
    {
        if (packet)
        {
            qreal endTime = packet->startTime() + packet->duration();
            maxEndTime = qMax(maxEndTime, endTime);
        }
    }

    // Convert seconds to milliseconds and ensure minimum duration
    qreal maxDurationMs = maxEndTime * 1000; // Convert to milliseconds
    return qMax(maxDurationMs, m_minDuration);
}

void PlaybackSlider::onAnimationsChanged()
{
    emit maxDurationChanged();
    qDebug() << "PlaybackSlider: maxDuration updated to" << maxDuration() << "ms";
}
