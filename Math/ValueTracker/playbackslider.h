#ifndef PLAYBACKSLIDER_H
#define PLAYBACKSLIDER_H

#include <QTimer>
#include "ValueTracker.h"

class AnimationManager;

class PlaybackSlider : public ValueTracker
{
    Q_OBJECT
    Q_PROPERTY(bool playing READ isPlaying NOTIFY playingChanged FINAL)
    Q_PROPERTY(qreal maxDuration READ maxDuration NOTIFY maxDurationChanged FINAL)
    Q_PROPERTY(AnimationManager *animationManager READ animationManager WRITE setAnimationManager NOTIFY animationManagerChanged FINAL)

public:
    explicit PlaybackSlider(QQuickItem *parent = nullptr)
        : ValueTracker(), m_animationManager(nullptr), m_minDuration(5000) // 5 second minimum
    {
        m_updateTimer = new QTimer(this);
        m_updateTimer->setInterval(10); // ~33 FPS update
        connect(m_updateTimer, &QTimer::timeout, this, &PlaybackSlider::onTimeout);
        this->setParentItem(parent);
        setValue(0);
    }

    ~PlaybackSlider()
    {
        qDebug() << "PlaybackSlider destroyed";
    }

    bool isPlaying() const { return m_updateTimer->isActive(); }
    qreal maxDuration() const { return calculateMaxDuration(); }

    AnimationManager *animationManager() const { return m_animationManager; }
    void setAnimationManager(AnimationManager *manager);

    Q_INVOKABLE void play()
    {
        qreal maxDur = maxDuration();
        if (value() >= maxDur)
            setValue(0);
        m_updateTimer->start();
        emit playingChanged();
    }

    Q_INVOKABLE void pause()
    {
        m_updateTimer->stop();
        emit playingChanged();
    }

    Q_INVOKABLE void reset()
    {
        m_updateTimer->stop();
        setValue(0);
        emit playingChanged();
    }

signals:
    void playingChanged();
    void maxDurationChanged();
    void animationManagerChanged();

private slots:
    void onTimeout()
    {
        qreal maxDur = maxDuration();
        if (value() < maxDur)
        {
            // Increment value by timer interval
            setValue(qMin(value() + m_updateTimer->interval(), maxDur));
        }
        else
        {
            pause();
        }
    }

    void onAnimationsChanged();

private:
    qreal calculateMaxDuration() const;

    AnimationManager *m_animationManager;
    qreal m_minDuration;
    QTimer *m_updateTimer;
};

#endif // PLAYBACKSLIDER_H
