#include "MRectangle.h"

MRectangle::MRectangle(Scene *canvas, QQuickItem *parent)
    : Polygon(canvas, parent)
{

    setFlag(ItemHasContents, true);
    auto position = properties->pos();
    properties->setSize({2,4});


    updatePoints(properties->size().first/2,properties->size().second/2,position);
    properties->setName("Rectangle");

    // connect(properties, &MProperties::sizeChanged,
    //         this, [this](const QPair<qreal, qreal> &sz) {
    //             auto p =properties->pos();
    //             m_rectHeight = sz.first;
    //             m_rectWidth  = sz.second;
    //             updatePoints(m_rectHeight/2,m_rectWidth/2,p);
    //         });


    buildPolygon();

}


void MRectangle::updatePoints(qreal height, qreal width, QPointF &center)
{
    // height= height * getcanvas()->scalefactor();
    // width= width * getcanvas()->scalefactor();
    QList<QPointF> rectPoints = {
        QPointF(center + QPointF(height*2,width/2)),
        QPointF(center + QPointF(height*2,-width/2)),
        QPointF(center + QPointF(-height*2,-width/2)),
        QPointF(center + QPointF(-height*2,width/2)),
    };

    qDebug()<<rectPoints<<"Setting End Points";

    properties->setEndPoints(rectPoints);
}
