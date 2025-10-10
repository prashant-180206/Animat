#ifndef LINE_H
#define LINE_H

#include "Math/Mobjects/Base/ClickableMobject.h"
#include <QPointF>
#include <QSGGeometryNode>
#include <QSGGeometry>
#include <QSGFlatColorMaterial>
#include <QColor>

class Line : public ClickableMobject // Inherit from Mobject instead of QQuickItem
{
    Q_OBJECT
    QML_ELEMENT

public:
    explicit Line(Scene *canvas, QQuickItem *parent = nullptr);

    virtual void setCenter(qreal x, qreal y) override;

protected:
    QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *data) override;
    bool contains(const QPointF &point) const override;
    QRectF boundingRect() const override;
    // void mouseReleaseEvent(QMouseEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

private:
    QPointF start_pos;
    QPointF m_p1;
    QPointF m_p2;
    QPointF m_shift;
signals:
    void ThicknessChanged();
};

#endif // LINE_H
