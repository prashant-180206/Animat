#include "ClickableMobject.h"


ClickableMobject::ClickableMobject(Scene *canvas, QQuickItem *parent)
    : Mobject(parent)
{
    setFlag(Mobject::ItemHasContents, true);
    setAcceptedMouseButtons(Qt::AllButtons);
    m_canvas=canvas;
    setSize(0,0);
    properties["Name"]="Mobject";
    properties["x"]=0;
    properties["y"]=0;
    properties["Color"]=color();
}

int ClickableMobject::getId() const { return m_id; }

void ClickableMobject::setId(int newid)
{
    m_id=newid;
}

Scene *ClickableMobject::getcanvas()
{
    return m_canvas;
}

void ClickableMobject::setCenter(qreal xval=0, qreal yval=0)
{
    properties["x"]=xval;
    properties["y"]=yval;
    QPointF pt = QPointF(xval,yval);
    pt = getcanvas()->p2c(pt);

    setX(pt.x());
    setZ(50);
    setY(pt.y());

    center=pt;
}




void ClickableMobject::setSize(qreal height, qreal width)
{
    auto h = height *getcanvas()->scalefactor();
    auto w = width *getcanvas()->scalefactor();
    setHeight(h);
    setWidth(w);
    properties["Height"]=h;
    properties["Width"]=w;
}

QVariantMap ClickableMobject::getProperties(){return properties;}

QPointF ClickableMobject::getCenter() const
{
    return center;
}

QPointF ClickableMobject::top() const
{
    // Middle of the top edge (x center, y top)
    return QPointF(x() + width() / 2, y());
}

QPointF ClickableMobject::bottom() const
{
    // Middle of the bottom edge (x center, y bottom)
    return QPointF(x() + width() / 2, y() + height());
}

QPointF ClickableMobject::left() const
{
    // Middle of the left edge (x left, y center)
    return QPointF(x(), y() + height() / 2);
}

QPointF ClickableMobject::right() const
{
    // Middle of the right edge (x right, y center)
    return QPointF(x() + width(), y() + height() / 2);
}

