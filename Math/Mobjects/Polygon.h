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

public:
    explicit Polygon(Scene *canvas, QQuickItem *parent = nullptr);

    void buildPolygon();

protected:
    QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *data) override;
    bool contains(const QPointF &) const override;
    void extracted(QList<QPointF> &m_points, qreal &minX, qreal &maxX,
                   qreal &minY, qreal &maxY) const;
    QRectF boundingRect() const override;

    void updateGeometry();
private:

    QSGGeometryNode *m_fillNode = nullptr;

signals:
    void borderColorChanged();
    void ThicknessChanged();
};

#endif // POLYGON_H
