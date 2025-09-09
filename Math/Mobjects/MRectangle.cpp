#include "MRectangle.h"

MRectangle::MRectangle(Scene *canvas, QQuickItem *parent)
    : Polygon(canvas, parent)
{

    setFlag(ItemHasContents, true);
    properties->setSize({2,4});

    updatePoints(properties->size().x()/2,properties->size().y()/2);
    properties->setName("Rectangle");
    properties->setType("Rectangle");

    connect(properties,&MProperties::sizeChanged,this,[this](auto s){
        auto p = QPointF(0,0);
        updatePoints(
            properties->size().x()/2
            ,properties->size().y()/2
            );
        properties->setSize(s);
        updateLines();
    });

    connect(properties,&MProperties::thicknessChanged,this,[this](){
        updateLines();
    });

    connect(properties,&MProperties::borderColorChanged,this,[this](){
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
