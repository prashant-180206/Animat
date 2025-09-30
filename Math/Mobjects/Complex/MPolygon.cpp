#include "MPolygon.h"
#include "Math/Scene.h"
#include <QDebug>

MPolygon::MPolygon(Scene *canvas, QQuickItem *parent)
    : Polygon(canvas, parent)
{
    // Set default name for MPolygon
    if (properties && properties->base())
    {
        properties->base()->setName("MPolygon");
        properties->base()->setType("MPolygon");
    }

    // Set initial default triangle
    setPoints({QPointF(0, 0),
               QPointF(1, 0),
               QPointF(0, 2)});

    connectToPropertyChanges();
    updatePolygon();
}

void MPolygon::setPolygonPoints(const QList<QPointF> &points)
{
    if (polygonPoints() != points)
    {
        setPoints(points);
        updatePolygon();
        emit polygonPointsChanged();
    }
}

void MPolygon::addPoint(const QPointF &point)
{
    auto currentPoints = points();
    currentPoints.append(point);
    setPolygonPoints(currentPoints);
    qDebug() << "MPolygon: Added point" << point << "Total points:" << currentPoints.size();
}

void MPolygon::addPoint(qreal x, qreal y)
{
    addPoint(QPointF(x, y));
}

void MPolygon::removePoint(int index)
{
    auto currentPoints = points();
    if (index >= 0 && index < currentPoints.size())
    {
        if (currentPoints.size() <= 3)
        {
            qDebug() << "MPolygon: Cannot remove point - minimum 3 points required";
            return;
        }

        QPointF removedPoint = currentPoints.takeAt(index);
        setPolygonPoints(currentPoints);
        qDebug() << "MPolygon: Removed point" << removedPoint << "at index" << index;
    }
    else
    {
        qDebug() << "MPolygon: Invalid index for removePoint:" << index;
    }
}

void MPolygon::insertPoint(int index, const QPointF &point)
{
    auto currentPoints = points();
    if (index >= 0 && index <= currentPoints.size())
    {
        currentPoints.insert(index, point);
        setPolygonPoints(currentPoints);
        qDebug() << "MPolygon: Inserted point" << point << "at index" << index;
    }
    else
    {
        qDebug() << "MPolygon: Invalid index for insertPoint:" << index;
    }
}

void MPolygon::insertPoint(int index, qreal x, qreal y)
{
    insertPoint(index, QPointF(x, y));
}

void MPolygon::movePoint(int index, const QPointF &point)
{
    auto currentPoints = points();
    if (index >= 0 && index < currentPoints.size())
    {
        currentPoints[index] = point;
        setPolygonPoints(currentPoints);
        qDebug() << "MPolygon: Moved point at index" << index << "to" << point;
    }
    else
    {
        qDebug() << "MPolygon: Invalid index for movePoint:" << index;
    }
}

void MPolygon::movePoint(int index, qreal x, qreal y)
{
    movePoint(index, QPointF(x, y));
}

int MPolygon::pointCount() const
{
    return points().size();
}

QPointF MPolygon::getPoint(int index) const
{
    auto currentPoints = points();
    if (index >= 0 && index < currentPoints.size())
    {
        return currentPoints[index];
    }
    qDebug() << "MPolygon: Invalid index for getPoint:" << index;
    return QPointF();
}

void MPolygon::clearPoints()
{
    // Set to minimum triangle
    setPolygonPoints({QPointF(0, 0),
                      QPointF(1, 0),
                      QPointF(1, 0)});
    qDebug() << "MPolygon: Cleared all points, reset to default triangle";
}

void MPolygon::updatePolygon()
{
    buildPolygon();
    update();
}

void MPolygon::connectToPropertyChanges()
{
    // Connect to property changes if needed
    if (properties && properties->polygon())
    {
        connect(properties->polygon(), &PolygonProperties::borderColorChanged,
                this, &MPolygon::updatePolygon);
        connect(properties->polygon(), &PolygonProperties::thicknessChanged,
                this, &MPolygon::updatePolygon);
    }
}
