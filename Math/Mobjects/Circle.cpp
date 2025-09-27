#include "Circle.h"
#include <QtMath>

Circle::Circle(Scene *canvas, QQuickItem *parent)
    : Polygon(canvas, parent)
{
    setFlag(ItemHasContents, true);

    properties->setCircle(new CircleProperties(this->properties));

    auto r = properties->circle()->radius();
    properties->base()->setSize({2*r,2*r});
    properties->polygon()->setThickness(4);
    setCenter(0,0);
    properties->circle()->setRadius(1);
    properties->circle()->setSegments(30);
    properties->base()->setName("Circle");
    properties->base()->setType("Circle");

    connect(properties->circle(),&CircleProperties::radiusChanged,this,[this]{
        updatePoints();
    });
    connect(properties->polygon(),&PolygonProperties::thicknessChanged,this,[this]{
        updatePoints();
    });
    connect(properties->polygon(),&PolygonProperties::borderColorChanged,this,[this]{
        updatePoints();
    });
    updatePoints();
}


void Circle::updatePoints()
{
    QVector<QPointF> points;
    for (int i = 0; i < properties->circle()->segments(); ++i) {
        double angle = (2 * M_PI * i) / properties->circle()->segments();
        double x = properties->circle()->radius()  * qCos(angle);
        double y = properties->circle()->radius() *  qSin(angle);
        points.append(QPointF(x, y));
    }
    setPoints(points);      // <--- REMOVE the subtraction!
    buildPolygon();
}


