#include "Mobject.h"
#include "Math/scene.h"
#include <QDebug>
#include <QQmlComponent>
#include <QQmlEngine>
#include <QQuickItem>

Mobject::Mobject(Scene *canvas, QQuickItem *parent)
    : QQuickItem(parent)
{
    m_canvas = canvas;
}

void Mobject::setId(int newid) {
    if (m_id != newid) {
        m_id = newid;
        emit idChanged();
    }
}

Scene *Mobject::getcanvas(){return m_canvas;}

void Mobject::setCenter(float x, float y)
{
    QPointF pt = QPointF(x,y);
    pt = m_canvas->c2p(pt);
    pt = mapToItem(m_canvas,pt);
    setX(pt.x()-width()/2);
    setY(pt.y()-height()/2);

    pt = m_canvas->p2c(pt);
    center=pt;
}




QPointF Mobject::getCenter() const
{
    return QPointF(x() + width() / 2, y() + height() / 2);
}

QPointF Mobject::top() const
{
    // Middle of the top edge (x center, y top)
    return QPointF(x() + width() / 2, y());
}

QPointF Mobject::bottom() const
{
    // Middle of the bottom edge (x center, y bottom)
    return QPointF(x() + width() / 2, y() + height());
}

QPointF Mobject::left() const
{
    // Middle of the left edge (x left, y center)
    return QPointF(x(), y() + height() / 2);
}

QPointF Mobject::right() const
{
    // Middle of the right edge (x right, y center)
    return QPointF(x() + width(), y() + height() / 2);
}



