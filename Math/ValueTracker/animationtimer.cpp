#include "AnimationTimer.h"

AnimationTimer::AnimationTimer(QObject *parent)
    : ValueTracker(0.0, parent), accumulated(0), running(false), tickCount(0) {
    secondTimer = new QTimer(this);
    secondTimer->setInterval(1000); // 1 second

    connect(secondTimer, &QTimer::timeout, this, &AnimationTimer::onSecondTick);
}

void AnimationTimer::play() {
    if (!running) {
        timer.start();
        running = true;
        tickCount = 0;
        secondTimer->start();
    }
}

void AnimationTimer::pause() {
    if (running) {
        accumulated += timer.elapsed();
        running = false;
        secondTimer->stop();
    }
}

void AnimationTimer::cont() {
    if (!running) {
        timer.start();
        running = true;
        secondTimer->start();
    }
}

void AnimationTimer::reset() {
    accumulated = 0;
    setValue(0);
    running = false;
    tickCount = 0;
    secondTimer->stop();
}

double AnimationTimer::getTime() const {
    return running ? accumulated + timer.elapsed() : accumulated;
}

void AnimationTimer::onSecondTick() {
    ++tickCount;
    double currentTimeSec = getTime(); // Convert ms to s
    setValue(currentTimeSec);
    emit secondPassed(tickCount);

    if (tickCount >= 20) {
        secondTimer->stop();
        pause();
    }
}
