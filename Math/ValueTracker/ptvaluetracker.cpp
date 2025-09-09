#include "ptvaluetracker.h"



PtValueTracker::PtValueTracker()
    : m_updateTimer(new QTimer(this))
{
    connect(m_updateTimer, &QTimer::timeout, this, &PtValueTracker::onTimeout);
}

QString PtValueTracker::func() const { return m_func; }

void PtValueTracker::setValue(QPointF v) {
    if (v != m_value) {
        m_value = v;
        emit valueChanged(v);
    }
}



void PtValueTracker::updateVal(QPointF target, qreal sec, QString func) {
    if (sec <= 0)
        sec = 1.0; // Safety

    m_startValue = m_value;  // Fix interpolation using start value
    m_target = target;
    m_duration = sec * 1000; // milliseconds
    m_elapsed = 0;
    setFunc(func.toLower());

    if (m_func == "linear") {
        m_easingCurve = QEasingCurve(QEasingCurve::Linear);
    } else {
        m_easingCurve = QEasingCurve(QEasingCurve::InOutQuad);
    }

    if (!m_updateTimer->isActive()) {
        m_updateTimer->start(m_updateInterval);
    }
}

void PtValueTracker::onTimeout() {
    m_elapsed += m_updateInterval;
    if (m_elapsed >= m_duration) {
        m_updateTimer->stop();
        setValue(m_target);
        return;
    }

    double progress = double(m_elapsed) / m_duration;
    double easedProgress = m_easingCurve.valueForProgress(progress);
    QPointF newVal = m_startValue + (m_target - m_startValue) * easedProgress;

    setValue(newVal);
}
