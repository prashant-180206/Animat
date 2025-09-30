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
    Q_PROPERTY(double width READ width WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(double height READ height WRITE setHeight NOTIFY heightChanged)
    Q_PROPERTY(double step READ step WRITE setStep NOTIFY stepChanged)

public:
    explicit Plane(Scene *canvas, QQuickItem *parent = nullptr);
    ~Plane();

    void setWidth(double width);
    void setHeight(double height);
    void setStep(double step);

    double width() const { return m_width; }
    double height() const { return m_height; }
    double step() const { return m_step; }

    // Override for clickable area
    bool contains(const QPointF &point) const override;
    QRectF boundingRect() const override;

    // Configure plane appearance
    void updateGridLines();
    void updateAxisLines();
    void updateLabels();

private slots:
    void onPlanePropertiesChanged();

signals:
    void widthChanged();
    void heightChanged();
    void stepChanged();

private:
    void setupProperties();
    void createGrid();
    void createAxis();
    void createLabels();
    void clearGrid();

    double m_width = 10.0;
    double m_height = 10.0;
    double m_step = 1.0;

    QList<SimpleLine *> m_gridLines;
    QList<SimpleLine *> m_axisLines;
    QList<SimpleText *> m_labels;

    PlaneProperties *m_planeProps = nullptr;
};

#endif // PLANE_H
