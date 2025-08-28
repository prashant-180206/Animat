#ifndef LINE_H
#define LINE_H

#include <Math/Helper/ClickableMobject.h>  // Include the Mobject base class header
#include <QPointF>
#include <QSGGeometryNode>
#include <QSGGeometry>
#include <QSGFlatColorMaterial>


class Line : public ClickableMobject  // Inherit from Mobject instead of QQuickItem
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QPointF p1 READ p1 WRITE setP1 NOTIFY p1Changed)
    Q_PROPERTY(QPointF p2 READ p2 WRITE setP2 NOTIFY p2Changed)

public:
    explicit Line(Scene* canvas,QQuickItem *parent = nullptr);

    QPointF p1() const;
    void setP1(const QPointF &pt);


    QPointF p2() const;
    void setP2(const QPointF &pt);

signals:
    void p1Changed();
    void p2Changed();

protected:
    QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *data) override;
    bool contains(const QPointF &point) const override;
    QRectF boundingRect() const override;
    void setCenter(float x,float y) override;
    qreal width() const;
    qreal height() const;
private:
    QPointF m_p1, m_p2;
};

#endif // LINE_H
