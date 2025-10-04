#ifndef PLANE_H
#define PLANE_H

#include "Math/Mobjects/Groups/group.h"
#include "Math/Mobjects/Simple/SimpleLine.h"
#include "Math/Mobjects/Simple/SimpleText.h"
#include "Math/Helper/Properties/planeproperties.h"
#include <qqmlintegration.h>

class Plane : public Group
{
    Q_OBJECT
    QML_ELEMENT

public:
    explicit Plane(Scene *canvas, QQuickItem *parent = nullptr);
    ~Plane();

    // Override for clickable area
    bool contains(const QPointF &point) const override;
    QRectF boundingRect() const override;

    // Configure plane appearance
    void updateGridLines();
    void updateAxisLines();
    void updateLabels();

private slots:
    void onPlanePropertiesChanged();

private:
    void setupProperties();
    void createGrid();
    void createAxis();
    void createLabels();
    void clearGrid();

    QList<SimpleLine *> m_gridLines;
    QList<SimpleLine *> m_axisLines;
    QList<SimpleText *> m_labels;
};

#endif // PLANE_H
