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

    QString func() const;
    void setFunc(const QString &f) { m_func = f; }

    QPointF value() const { return m_value; }
    void setValue(QPointF v);

    Q_INVOKABLE void updateVal(QPointF target, qreal sec = 1.0, QString func = "default");

signals:
    void valueChanged(QPointF v);

private slots:
    void onTimeout();

private:
    QPointF m_value{0, 0};
    QPointF m_startValue{0, 0};
    QPointF m_target{1, 1};
    qreal m_duration = 1000; // Animation duration in milliseconds
    int m_elapsed = 0;       // Time elapsed in milliseconds

    QTimer *m_updateTimer = nullptr;
    QEasingCurve m_easingCurve = QEasingCurve(QEasingCurve::InOutQuad);
    QString m_func = "default";

    static const int m_updateInterval = 16; // ~60 FPS timer interval (16 ms)
};

#endif // PTVALUETRACKER_H
