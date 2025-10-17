#ifndef SIMPLE_VECTOR_H
#define SIMPLE_VECTOR_H

#include "SimpleLine.h"
#include <QtMath>

class SimpleVector : public SimpleLine
{
    Q_OBJECT
    QML_ELEMENT

public:
    explicit SimpleVector(Scene *canvas, QQuickItem *parent = nullptr);

protected:
    QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *data) override;

private:
    void createArrowGeometry(QSGGeometry *geometry, const QVector2D &p1, const QVector2D &p2, float thickness);
};

#endif // SIMPLE_VECTOR_H