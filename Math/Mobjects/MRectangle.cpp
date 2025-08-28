#include "MRectangle.h"

MRectangle::MRectangle(Scene *canvas, QQuickItem *parent)
    : Polygon(canvas, parent)
{
    setFlag(ItemHasContents, true);
    // qDebug()<<height()<<width();
    updatePoints();
}

qreal MRectangle::rectWidth() const
{
    return m_rectWidth;
}

void MRectangle::setRectWidth(qreal width)
{
    if (!qFuzzyCompare(m_rectWidth, width)) {
        m_rectWidth = width;
        emit rectWidthChanged();
        updatePoints();
        update();
    }
}

qreal MRectangle::rectHeight() const
{
    return m_rectHeight;
}

void MRectangle::setRectHeight(qreal height)
{
    if (!qFuzzyCompare(m_rectHeight, height)) {
        m_rectHeight = height;
        emit rectHeightChanged();
        updatePoints();
        update();
    }
}

void MRectangle::updatePoints()
{
    QVector<QPointF> rectPoints = {
        QPointF(0, 0),
        QPointF(m_rectWidth, 0),
        QPointF(m_rectWidth, m_rectHeight),
        QPointF(0, m_rectHeight)
    };

    setPoints(rectPoints);
}
