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

    QString func();;
    void setFunc(QString f);

    qreal value() { return m_value; }
    void setValue(qreal v);

    Q_INVOKABLE void updateVal(qreal target,qreal sec=1, QString func = "default");


private:
    qreal m_value = 1;
    qreal m_target = 0;
    qreal m_stepDelta = 0;
    int m_currentStep = 0;
    QTimer *m_updateTimer = nullptr;
    QEasingCurve m_easingCurve = QEasingCurve::InOutQuad;
    QString m_func = "default";

signals:
    void valueChanged(qreal v);
};

#endif // VALUETRACKER_H
