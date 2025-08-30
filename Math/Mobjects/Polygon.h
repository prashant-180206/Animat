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
    Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor NOTIFY borderColorChanged)
    Q_PROPERTY(qreal Thickness READ Thickness WRITE setThickness NOTIFY ThicknessChanged FINAL)
public:
    explicit Polygon(Scene *canvas, QQuickItem *parent = nullptr);

    void setPoints(const QVector<QPointF> &points);
    QVector<QPointF> points() const;
    QColor borderColor(){return m_borderColor;};
    void setBorderColor(QColor c){m_borderColor=c;
        emit borderColorChanged();
    };

    qreal Thickness(){
        return m_thickness;
    }
    void setThickness(qreal thickness){
        m_thickness = thickness;
        emit ThicknessChanged();
    }

    void buildPolygon();

protected:
    QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *data) override;
    bool contains(const QPointF &) const override;
    QRectF boundingRect() const override;

    void updateGeometry();
private:
    qreal m_thickness=6;
    QColor m_borderColor= Qt::yellow;
    QVector<QPointF> m_points;
    QSGGeometryNode *m_fillNode = nullptr;

signals:
    void borderColorChanged();
    void ThicknessChanged();
};

#endif // POLYGON_H
