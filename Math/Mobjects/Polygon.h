#ifndef POLYGON_H
#define POLYGON_H

#include "Group.h"
#include <QColor>
#include <QVector>
#include <QPointF>
#include <QSGGeometryNode>

class Polygon : public Group
{
    Q_OBJECT
    Q_PROPERTY(QColor fillColor READ fillColor WRITE setFillColor NOTIFY fillColorChanged)
public:
    explicit Polygon(Scene *canvas = nullptr, QQuickItem *parent = nullptr);

    void setPoints(const QVector<QPointF> &points);
    QVector<QPointF> points() const;

    QColor fillColor() const;
    void setFillColor(const QColor &color);

    Q_INVOKABLE void buildPolygon();

signals:
    void fillColorChanged();

protected:
    QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *data) override;

private:
    QVector<QPointF> m_points;
    QColor m_fillColor;
    QSGGeometryNode *m_fillNode = nullptr;
    Scene * canvas;
};

#endif // POLYGON_H
