#ifndef CIRCLE_H
#define CIRCLE_H

#include "Polygon.h"
#include <QQuickItem>

class Circle : public Polygon
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(qreal radius READ radius WRITE setRadius NOTIFY radiusChanged)
    Q_PROPERTY(int segmentCount READ segmentCount WRITE setSegmentCount NOTIFY segmentCountChanged)

public:
    explicit Circle(Scene *canvas, QQuickItem *parent = nullptr);

    qreal radius() const;
    void setRadius(qreal radius);

    int segmentCount() const;
    void setSegmentCount(int count);

signals:
    void radiusChanged();
    void segmentCountChanged();

protected:
    void updatePoints();

private:
    qreal m_radius = 1;
    int m_segmentCount = 40;
};

#endif // CIRCLE_H
