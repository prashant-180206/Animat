#ifndef MPOLYGON_H
#define MPOLYGON_H

#include "Mobjects/Basic/Polygon.h"
#include <QQmlEngine>

class Scene;

class MPolygon : public Polygon
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QList<QPointF> polygonPoints READ polygonPoints WRITE setPolygonPoints NOTIFY polygonPointsChanged FINAL)

public:
    explicit MPolygon(Scene *canvas = nullptr, QQuickItem *parent = nullptr);

    // Property getter/setter
    QList<QPointF> polygonPoints() const { return points(); }
    void setPolygonPoints(const QList<QPointF> &points);

    // Q_INVOKABLE functions for dynamic point management
    Q_INVOKABLE void addPoint(const QPointF &point);
    Q_INVOKABLE void addPoint(qreal x, qreal y);
    Q_INVOKABLE void removePoint(int index);
    Q_INVOKABLE void insertPoint(int index, const QPointF &point);
    Q_INVOKABLE void insertPoint(int index, qreal x, qreal y);
    Q_INVOKABLE void movePoint(int index, const QPointF &point);
    Q_INVOKABLE void movePoint(int index, qreal x, qreal y);
    Q_INVOKABLE int pointCount() const;
    Q_INVOKABLE QPointF getPoint(int index) const;
    Q_INVOKABLE void clearPoints();

signals:
    void polygonPointsChanged();

private:
    void updatePolygon();
    void connectToPropertyChanges();
};

#endif // MPOLYGON_H
