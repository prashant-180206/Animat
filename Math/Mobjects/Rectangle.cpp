#include "Rectangle.h"

Rectangle::Rectangle(Scene *canvas, QQuickItem *parent)
    : Polygon(canvas, parent)
{
    setFlag(ItemHasContents, true);
    updatePoints();
}

qreal Rectangle::rectWidth() const
{
    return m_rectWidth;
}

void Rectangle::setRectWidth(qreal width)
{
    if (!qFuzzyCompare(m_rectWidth, width)) {
        m_rectWidth = width;
        emit rectWidthChanged();
        updatePoints();
        update();
    }
}

qreal Rectangle::rectHeight() const
{
    return m_rectHeight;
}

void Rectangle::setRectHeight(qreal height)
{
    if (!qFuzzyCompare(m_rectHeight, height)) {
        m_rectHeight = height;
        emit rectHeightChanged();
        updatePoints();
        update();
    }
}

void Rectangle::updatePoints()
{
    QVector<QPointF> rectPoints = {
        QPointF(0, 0),
        QPointF(m_rectWidth, 0),
        QPointF(m_rectWidth, m_rectHeight),
        QPointF(0, m_rectHeight)
    };

    setPoints(rectPoints);
}
