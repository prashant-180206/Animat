#include "Mobject.h"

Mobject::Mobject(Scene *canvas, QQuickItem *parent)
    : QQuickItem(parent)
{
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

