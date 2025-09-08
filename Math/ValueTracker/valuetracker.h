#ifndef VALUETRACKER_H
#define VALUETRACKER_H

#include <QTimer>
#include <QQuickItem>
#include <QEasingCurve>

class ValueTracker : public QQuickItem
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(qreal value READ value WRITE setValue NOTIFY valueChanged FINAL)
public:
    ValueTracker();
    qreal value();;
    void setValue(qreal v);;

    Q_INVOKABLE void updateVal(qreal target);

private:
    qreal m_value = 1;
    qreal m_target = 0;
    qreal m_stepDelta = 0;
    int m_currentStep = 0;
    QTimer *m_updateTimer = nullptr;

    QEasingCurve m_easingCurve = QEasingCurve::InOutQuad;
signals:
    void valueChanged();
};


#endif // VALUETRACKER_H
