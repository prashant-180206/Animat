#include "MRectangle.h"

MRectangle::MRectangle(Scene *canvas, QQuickItem *parent)
    : Polygon(canvas, parent)
{

    setFlag(ItemHasContents, true);
    properties->setSize({2,4});

    updatePoints(properties->size().x()/2,properties->size().y()/2);
    properties->setName("Rectangle");

    connect(properties,&MProperties::sizeChanged,this,[this](auto s){
        // auto p = properties->pos();
        auto p = QPointF(0,0);
        updatePoints(
            properties->size().x()/2
            ,properties->size().y()/2
            );
        update();
        properties->setSize(s);
        // removeAllMembers();
        updateLines();
    });


    buildPolygon();

}


void MRectangle::updatePoints(qreal height, qreal width)
{
    // height= height * getcanvas()->scalefactor();
    // width= width * getcanvas()->scalefactor();
    QList<QPointF> rectPoints = {
        QPointF(QPointF(height,width)),
        QPointF(QPointF(height,-width)),
        QPointF(QPointF(-height,-width)),
        QPointF(QPointF(-height,width)),
    };

    qDebug()<<rectPoints<<"Setting End Points";

    setPoints(rectPoints);
}
