#ifndef CIRCLEPROPERTIES_H
#define CIRCLEPROPERTIES_H

#include <QObject>
#include <QQmlEngine>

class CircleProperties : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(qreal radius READ radius WRITE setRadius NOTIFY radiusChanged)
    Q_PROPERTY(int segments READ segments WRITE setSegments NOTIFY segmentsChanged FINAL)

public:
    explicit CircleProperties(QObject *parent = nullptr);
    qreal radius() const;
    int segments(){return m_segments;};
public slots:
    void setRadius(qreal radius);
    void setSegments(int segments);
signals:
    void radiusChanged();
    void segmentsChanged();
private:
    qreal m_radius = 0;
    int m_segments=30;
};

#endif // CIRCLEPROPERTIES_H
