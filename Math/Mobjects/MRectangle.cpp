#include "MRectangle.h"

MRectangle::MRectangle(Scene *canvas, QQuickItem *parent)
    : Polygon(canvas, parent)
{
    setFlag(ItemHasContents, true);
    updatePoints(m_rectHeight,m_rectWidth,getCenter());
    setSize(m_rectHeight,m_rectWidth);
    setCenter(0,2);
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
        updatePoints(m_rectHeight,m_rectWidth,getCenter());
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
        updatePoints(m_rectHeight,m_rectWidth,getCenter());
        update();
    }
}

void MRectangle::updatePoints(qreal height, qreal width, QPointF center)
{
    QVector<QPointF> rectPoints = {
        QPointF(center + QPointF(height*2,width/2)),
        QPointF(center + QPointF(height*2,-width/2)),
        QPointF(center + QPointF(-height*2,-width/2)),
        QPointF(center + QPointF(-height*2,width/2)),
    };

    qDebug()<<rectPoints;

    setPoints(rectPoints);
}
