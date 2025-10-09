#ifndef POLYGON_H
#define POLYGON_H

#include "Mobjects/Groups/group.h"
#include "Mobjects/Simple/SimpleLine.h"
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
    QRectF boundingRect() const override;

    QList<QPointF> points() const
    {
        return m_points;
    }

    void setPoints(const QList<QPointF> &pts)
    {
        m_points = pts;
    }

    void updateLines();

private:
    QPointF shift;

    QSGGeometryNode *m_fillNode = nullptr;
    QList<QPointF> m_points;
    QList<SimpleLine *> m_lines{};

signals:
    void borderColorChanged();
    void ThicknessChanged();
};

#endif // POLYGON_H
