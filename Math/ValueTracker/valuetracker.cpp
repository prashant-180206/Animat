#include "valuetracker.h"






ValueTracker::ValueTracker()
    : m_updateTimer(new QTimer(this))
{
    connect(m_updateTimer, &QTimer::timeout, this, [this]{
        if (m_currentStep >= 100) {
            m_updateTimer->stop();
            setValue(m_target);
            return;
        }

        // Calculate normalized progress [0..1]
        double progress = double(m_currentStep) / 100;

        // Apply easing curve
        double easedProgress = m_easingCurve.valueForProgress(progress);

        // Interpolate value with easing
        double newVal = m_value + (m_target - m_value) * easedProgress;

        setValue(newVal);
        m_currentStep++;
    });
}

qreal ValueTracker::value(){return m_value;}

void ValueTracker::setValue(qreal v){
    if(v!=m_value){
        m_value = v;
        emit valueChanged();
    };
}

void ValueTracker::updateVal(qreal target)
{
    m_target = target;
    m_stepDelta = (m_target - m_value) / 100.0;
    m_currentStep = 0;
    if (!m_updateTimer->isActive()) {
        m_updateTimer->start(20); // 20 ms interval, for 2 seconds total (100 steps)
    }
}
