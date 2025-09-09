#include "valuetracker.h"

ValueTracker::ValueTracker()
    : m_updateTimer(new QTimer(this))
{
    connect(m_updateTimer, &QTimer::timeout, this, [this]{
        if (m_currentStep >= 300) {
            m_updateTimer->stop();
            setValue(m_target);
            return;
        }

        double progress = double(m_currentStep) / 300;
        double easedProgress;

        if (func() == "linear") {
            easedProgress = double(m_currentStep) / 300;
        } else {
            easedProgress = m_easingCurve.valueForProgress(double(m_currentStep) / 300);
        }


        double newVal = m_value + (m_target - m_value) * easedProgress;
        setValue(newVal);
        m_currentStep++;
    });
}

QString ValueTracker::func(){return m_func;}

void ValueTracker::setFunc(QString f){
    m_func = f;
}

void ValueTracker::setValue(qreal v) {
    if (v != m_value) {
        m_value = v;
        emit valueChanged(v);
    }
}



void ValueTracker::updateVal(qreal target, qreal sec, QString func) {
    m_target = target;
    func = func.toLower();
    setFunc(func);

    if (func == "linear") {
        m_easingCurve = QEasingCurve(QEasingCurve::Linear);
    } else {  // default to InOutQuad
        m_easingCurve = QEasingCurve(QEasingCurve::InOutQuad);
    }

    m_stepDelta = (m_target - m_value) / 100.0;
    m_currentStep = 0;

    if (!m_updateTimer->isActive()) {
        m_updateTimer->start(sec*10); // 50 ms interval (~5 seconds total)
    }
}


