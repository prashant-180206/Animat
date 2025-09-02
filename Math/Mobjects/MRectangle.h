#ifndef MRECTANGLE_H
#define MRECTANGLE_H

#include "Polygon.h"
#include <QQuickItem>

class MRectangle : public Polygon
{
    Q_OBJECT
    QML_ELEMENT

public:
    explicit MRectangle(Scene *canvas, QQuickItem *parent = nullptr);

protected:
    void updatePoints(qreal height,qreal width , QPointF &center);

private:
};

#endif // MRECTANGLE_H
