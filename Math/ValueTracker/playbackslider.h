#ifndef PLAYBACKSLIDER_H
#define PLAYBACKSLIDER_H

#include <QTimer>
#include "ValueTracker.h"

class PlaybackSlider : public ValueTracker
{
    Q_OBJECT
    Q_PROPERTY(bool playing READ isPlaying NOTIFY playingChanged FINAL)
    Q_PROPERTY(qreal maxDuration READ maxDuration CONSTANT FINAL)

public:
    explicit PlaybackSlider(QQuickItem *parent = nullptr)
        : ValueTracker()
        , m_maxDuration(60000) // 1 minute max
    {
        m_updateTimer = new QTimer(this);
        m_updateTimer->setInterval(10); // ~33 FPS update
        connect(m_updateTimer, &QTimer::timeout, this, &PlaybackSlider::onTimeout);
        this->setParentItem(parent);
        setValue(0);
    }

    ~PlaybackSlider() {
        qDebug() << "PlaybackSlider destroyed";
    }

    bool isPlaying() const { return m_updateTimer->isActive(); }
    qreal maxDuration() const { return m_maxDuration; }

    Q_INVOKABLE void play() {
        if (value() >= m_maxDuration)
            setValue(0);
        m_updateTimer->start();
        emit playingChanged();
    }

    Q_INVOKABLE void pause() {
        m_updateTimer->stop();
        emit playingChanged();
    }

    Q_INVOKABLE void reset() {
        m_updateTimer->stop();
        setValue(0);
        emit playingChanged();
    }

signals:
    void playingChanged();

private slots:
    void onTimeout() {
        if (value() < m_maxDuration) {
            // Increment value by timer interval
            setValue(qMin(value() + m_updateTimer->interval(), m_maxDuration));

        } else {
            pause();
        }
    }

private:
    qreal m_maxDuration;
    QTimer *m_updateTimer;
};


#endif // PLAYBACKSLIDER_H
