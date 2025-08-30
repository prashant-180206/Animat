#ifndef LINE_H
#define LINE_H

#include <Math/Helper/ClickableMobject.h>  // Include the Mobject base class header
#include <QPointF>
#include <QSGGeometryNode>
#include <QSGGeometry>
#include <QSGFlatColorMaterial>
#include <QColor>


class Line : public ClickableMobject  // Inherit from Mobject instead of QQuickItem
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(qreal Thickness READ Thickness WRITE setThickness NOTIFY ThicknessChanged FINAL)

public:
    explicit Line(Scene* canvas,QQuickItem *parent = nullptr);

    virtual void setCenter(qreal x,qreal y) override;
    QPointF p1() const;
    void setP1(const QPointF &pt);
    QPointF p2() const;
    void setP2(const QPointF &pt);


    qreal Thickness(){
        return m_thickness;
    }
    void setThickness(qreal thickness){
        m_thickness= thickness;
        emit ThicknessChanged();
    }

protected:
    QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *data) override;
    bool contains(const QPointF &point) const override;
    QRectF boundingRect() const override;

    qreal width() const;
    qreal height() const;
private:
    QPointF m_p1, m_p2;
    qreal m_thickness=4;
signals:
    void ThicknessChanged();
};

#endif // LINE_H
