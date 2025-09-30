#ifndef CIRCLE_H
#define CIRCLE_H

#include "Polygon.h"
#include <QQuickItem>

class Circle : public Polygon
{
    Q_OBJECT
    QML_ELEMENT

public:
    explicit Circle(Scene *canvas, QQuickItem *parent = nullptr);

protected:
    void updatePoints();

};

#endif // CIRCLE_H
