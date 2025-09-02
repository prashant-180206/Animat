#include "Circle.h"
#include <QtMath>

Circle::Circle(Scene *canvas, QQuickItem *parent)
    : Polygon(canvas, parent)
{
    setFlag(ItemHasContents, true);

    auto r = properties->radius();
    setSize(2*r,2*r);
    setCenter(0,0);
    properties->setRadius(1);
    properties->setSegments(30);
    properties->setName("Circle");
    updatePoints();
}


void Circle::updatePoints()
{
    QVector<QPointF> points;
    for (int i = 0; i < m_segmentCount; ++i) {
        double angle = (2 * M_PI * i) / m_segmentCount;
        double x = properties->radius()  * qCos(angle);
        double y = properties->radius() *  qSin(angle);
        points.append((QPointF(x, y)));
    }
    properties->setEndPoints(points);

    // Adjust position and size of the Circle item based on boundingRect

    // Translate points to local coords (subtract topLeft)
    QVector<QPointF> localPoints;
    localPoints.reserve(points.size());
    for (const QPointF& p : std::as_const(points))
        localPoints.append(p - (position()));
    properties->setEndPoints(localPoints);

    buildPolygon();

    update();
}

