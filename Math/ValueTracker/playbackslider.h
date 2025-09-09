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
    explicit PlaybackSlider(QQuickItem *parent = nullptr);

    bool isPlaying() const;
    qreal maxDuration() const;

    Q_INVOKABLE void play();

    Q_INVOKABLE void pause();

    Q_INVOKABLE void reset();

signals:
    void playingChanged();

private slots:
    void onTimeout();

private:
    qreal m_maxDuration;
    QTimer *m_updateTimer;
};


#endif // PLAYBACKSLIDER_H
