#ifndef PTVALUETRACKER_H
#define PTVALUETRACKER_H

#include <QObject>
#include <QQmlEngine>
#include <QTimer>
#include <QQuickItem>
#include <QEasingCurve>

class PtValueTracker : public QQuickItem
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QPointF value READ value WRITE setValue NOTIFY valueChanged FINAL)

public:
    PtValueTracker();

    QPointF value() const;
    void setValue(QPointF v);

signals:
    void valueChanged(QPointF v);

private:
    QPointF m_value{0, 0};
};

#endif // PTVALUETRACKER_H
