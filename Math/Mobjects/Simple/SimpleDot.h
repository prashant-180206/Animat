#ifndef SIMPLE_DOT_H
#define SIMPLE_DOT_H

#include "Math/Mobjects/Base/Mobject.h"
#include <QPointF>
#include <QSGGeometryNode>
#include <QSGGeometry>
#include <QSGFlatColorMaterial>
#include <qqmlintegration.h>

class SimpleDot : public Mobject // Inherit from Mobject instead of QQuickItem
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos NOTIFY posChanged)
    Q_PROPERTY(qreal size READ size WRITE setSize NOTIFY sizeChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged FINAL)

public:
    explicit SimpleDot(Scene *canvas, QQuickItem *parent = nullptr);

    QPointF pos() const { return m_pos; }
    void setPos(const QPointF &pt);

    qreal size() const { return m_size; }
    void setSize(qreal s);

    QColor color() const { return m_color; }
    void setColor(const QColor &c);

signals:
    void posChanged();
    void sizeChanged();
    void colorChanged();

protected:
    QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *data) override;

private:
    Scene *m_canvas;
    QPointF m_pos;
    qreal m_size = 10.0;
    QColor m_color = Qt::blue;
};

#endif // SIMPLE_DOT_H