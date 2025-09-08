#ifndef ANIMATIONTIMER_H
#define ANIMATIONTIMER_H

#include <QElapsedTimer>
#include <QTimer>
#include "Math/ValueTracker/valuetacker.h" // Your previous ValueTracker header

class AnimationTimer : public ValueTracker {
    Q_OBJECT

public:
    explicit AnimationTimer(QObject *parent = nullptr);

    void play();
    void pause();
    void cont();
    void reset();
    double getTime() const;

signals:
    void secondPassed(int currentSecond);

private slots:
    void onSecondTick();

private:
    QElapsedTimer timer;
    QTimer *secondTimer;
    double accumulated;
    bool running;
    int tickCount;
};

#endif // ANIMATIONTIMER_H
