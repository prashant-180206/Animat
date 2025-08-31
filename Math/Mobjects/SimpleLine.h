#ifndef SIMPLE_LINE_H
#define SIMPLE_LINE_H

#include "Math/Mobjects/Mobject.h"
#include <QPointF>
#include <QSGGeometryNode>
#include <QSGGeometry>
#include <QSGFlatColorMaterial>
#include <qqmlintegration.h>


class SimpleLine : public Mobject  // Inherit from Mobject instead of QQuickItem
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QPointF p1 READ p1 WRITE setP1 NOTIFY p1Changed)
    Q_PROPERTY(QPointF p2 READ p2 WRITE setP2 NOTIFY p2Changed)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged FINAL)
    Q_PROPERTY(qreal Thickness READ Thickness WRITE setThickness NOTIFY ThicknessChanged FINAL)

public:
    explicit SimpleLine(Scene* canvas,QQuickItem *parent = nullptr);

    QPointF p1() const;
    void setP1(const QPointF &pt);

    QPointF p2() const;
    void setP2(const QPointF &pt);

    QColor color(){return m_color;};
    void setColor(QColor c){m_color=c;
        emit colorChanged();

    }

    qreal Thickness(){
        return m_thickness;
    };

    void setThickness(qreal thickness)
    {
        m_thickness= thickness;
        emit ThicknessChanged();
    }
signals:
    void p1Changed();
    void p2Changed();
    void colorChanged();
    void ThicknessChanged();

protected:
    QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *data) override;
    QColor m_color=Qt::yellow;
private:
    QPointF m_p1, m_p2;
    qreal m_thickness=6;
};

#endif // LINE_H
