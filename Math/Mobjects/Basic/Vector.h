#ifndef VECTOR_H
#define VECTOR_H

#include "line.h"
#include <QtMath>

class Vector : public Line
{
    Q_OBJECT
    QML_ELEMENT

public:
    explicit Vector(Scene *canvas, QQuickItem *parent = nullptr);

protected:
    QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *data) override;

private:
    void createArrowGeometry(QSGGeometry *geometry, const QVector2D &p1, const QVector2D &p2, float thickness);
};

#endif // VECTOR_H