#include "playbackslider.h"



PlaybackSlider::PlaybackSlider(QQuickItem *parent)
    : ValueTracker()
    , m_maxDuration(60000) // 1 minute max
{
    m_updateTimer = new QTimer(this);
    m_updateTimer->setInterval(30); // ~33 FPS update
    connect(m_updateTimer, &QTimer::timeout, this, &PlaybackSlider::onTimeout);
    setValue(0);
}

bool PlaybackSlider::isPlaying() const { return m_updateTimer->isActive(); }

qreal PlaybackSlider::maxDuration() const { return m_maxDuration; }

void PlaybackSlider::play() {
    if (value() >= m_maxDuration)
        setValue(0);
    m_updateTimer->start();
    emit playingChanged();
}

void PlaybackSlider::pause() {
    m_updateTimer->stop();
    emit playingChanged();
}

void PlaybackSlider::reset() {
    m_updateTimer->stop();
    setValue(0);
    emit playingChanged();
}

void PlaybackSlider::onTimeout() {
    if (value() < m_maxDuration) {
        // Increment value by timer interval
        setValue(qMin(value() + m_updateTimer->interval(), m_maxDuration));
    } else {
        pause();
    }
}
