#include "Circle.h"
#include <QtMath>

Circle::Circle(Scene *canvas, QQuickItem *parent)
    : Polygon(canvas, parent)
{
    setFlag(ItemHasContents, true);

    auto r = properties->radius();
    properties->setSize({2*r,2*r});
    properties->setThickness(4);
    setCenter(0,0);
    properties->setRadius(1);
    properties->setSegments(30);
    properties->setName("Circle");

    connect(properties,&MProperties::radiusChanged,this,[this]{
        updatePoints();
    });
    connect(properties,&MProperties::thicknessChanged,this,[this]{
        updatePoints();
    });
    connect(properties,&MProperties::borderColorChanged,this,[this]{
        updatePoints();
    });
    updatePoints();
}


void Circle::updatePoints()
{
    QVector<QPointF> points;
    for (int i = 0; i < m_segmentCount; ++i) {
        double angle = (2 * M_PI * i) / properties->segments();
        double x = properties->radius()  * qCos(angle);
        double y = properties->radius() *  qSin(angle);
        points.append(QPointF(x, y));
    }
    setPoints(points);      // <--- REMOVE the subtraction!
    buildPolygon();
    update();
}


