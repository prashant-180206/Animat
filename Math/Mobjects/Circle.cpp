#include "Circle.h"
#include <QtMath>

Circle::Circle(Scene *canvas, QQuickItem *parent)
    : Polygon(canvas, parent)
{
    setFlag(ItemHasContents, true);
    updatePoints();
}

qreal Circle::radius() const
{
    return m_radius;
}

void Circle::setRadius(qreal radius)
{
    if (!qFuzzyCompare(m_radius, radius)) {
        m_radius = radius;
        emit radiusChanged();
        updatePoints();
        update();
    }
}

int Circle::segmentCount() const
{
    return m_segmentCount;
}

void Circle::setSegmentCount(int count)
{
    if (m_segmentCount != count && count > 2) {
        m_segmentCount = count;
        emit segmentCountChanged();
        updatePoints();
        update();
    }
}

void Circle::updatePoints()
{
    QVector<QPointF> points;
    for (int i = 0; i < m_segmentCount; ++i) {
        double angle = (2 * M_PI * i) / m_segmentCount;
        double x = m_radius * qCos(angle);
        double y = m_radius * qSin(angle);
        points.append(QPointF(x, y));
    }
    setPoints(points);
}
