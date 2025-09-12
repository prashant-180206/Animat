#include "MRectangle.h"

MRectangle::MRectangle(Scene *canvas, QQuickItem *parent)
    : Polygon(canvas, parent)
{

    setFlag(ItemHasContents, true);
    properties->base()->setSize({2,4});

    updatePoints(properties->base()->size().x()/2,properties->base()->size().y()/2);
    properties->base()->setName("Rectangle");
    properties->base()->setType("Rectangle");

    connect(properties->base(),&BaseProperties::sizeChanged,this,[this](auto s){
        auto p = QPointF(0,0);
        updatePoints(
            properties->base()->size().x()/2
            ,properties->base()->size().y()/2
            );
        properties->base()->setSize(s);
        updateLines();
    });

    connect(properties->polygon(),&PolygonProperties::thicknessChanged,this,[this](){
        updateLines();
    });

    connect(properties->polygon(),&PolygonProperties::borderColorChanged,this,[this](){
        updateLines();
    });


    buildPolygon();

}


void MRectangle::updatePoints(qreal height, qreal width)
{
    QList<QPointF> rectPoints = {
        QPointF(QPointF(height,width)),
        QPointF(QPointF(height,-width)),
        QPointF(QPointF(-height,-width)),
        QPointF(QPointF(-height,width)),
    };

    setPoints(rectPoints);
}
