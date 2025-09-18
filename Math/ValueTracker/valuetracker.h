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

    qreal value() { return m_value; }
    void setValue(qreal v);

private:
    qreal m_value = 1;

signals:
    void valueChanged(qreal v);
};

#endif // VALUETRACKER_H
