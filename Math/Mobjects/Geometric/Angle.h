#ifndef ANGLE_H
#define ANGLE_H

#include "Math/Mobjects/Groups/group.h"
#include "Math/Mobjects/Simple/SimpleVector.h"
#include "Math/Helper/Properties/Geometric/angleproperties.h"
#include <QSGGeometryNode>
#include <QSGGeometry>
#include <QSGFlatColorMaterial>
#include <QtMath>

class Angle : public Group
{
    Q_OBJECT
    QML_ELEMENT

public:
    explicit Angle(Scene *canvas, QQuickItem *parent = nullptr);

protected:
    QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *data) override;
    QRectF boundingRect() const override;
    bool contains(const QPointF &point) const override;

private slots:
    void updateVectors();
    void updateArc();

private:
    SimpleVector *m_startVector;
    SimpleVector *m_endVector;

    void createArcGeometry(QSGGeometry *geometry, const QPointF &center, qreal radius,
                           qreal startAngle, qreal endAngle, qreal thickness, int segments = 32);
    QPointF m_center;
    void updateGeometry();
};

#endif // ANGLE_H