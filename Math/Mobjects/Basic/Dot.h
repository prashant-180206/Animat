#ifndef DOT_H
#define DOT_H

#include "Math/Mobjects/Base/ClickableMobject.h"
#include <QColor>
#include <QSGNode>
#include <QSGSimpleRectNode>

class Dot : public ClickableMobject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QColor dotColor READ dotColor WRITE setDotColor NOTIFY dotColorChanged)
    Q_PROPERTY(qreal radius READ radius WRITE setRadius NOTIFY radiusChanged)

public:
    explicit Dot(Scene *canvas = nullptr, QQuickItem *parent = nullptr);

    QColor dotColor() const { return properties->base() ? properties->base()->color() : Qt::white; }
    qreal radius() const { return m_radius; }

    void setDotColor(const QColor &color);
    void setRadius(qreal radius);

    // Override essential methods for proper mobject behavior
    virtual void setCenter(qreal x, qreal y) override;

signals:
    void dotColorChanged();
    void radiusChanged();

protected:
    QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *) override;
    bool contains(const QPointF &point) const override;
    QRectF boundingRect() const override;

private:
    qreal m_radius = 5.0; // Default radius of 5 pixels
    QPointF m_position;   // Store logical position

    void updateDotSize(); // Helper to update size based on radius and scale
};

#endif // DOT_H
