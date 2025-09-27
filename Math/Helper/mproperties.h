#ifndef MPROPERTIES_H
#define MPROPERTIES_H

#include "baseproperties.h"
#include "circleproperties.h"
#include "curveproperties.h"
#include "lineproperties.h"
#include "polygonproperties.h"
#include <QObject>
#include <qqmlintegration.h>

class MProperties : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(BaseProperties* base READ base WRITE setBase NOTIFY baseChanged)
    Q_PROPERTY(CircleProperties* circle READ circle WRITE setCircle NOTIFY circleChanged)
    Q_PROPERTY(CurveProperties* curve READ curve WRITE setCurve NOTIFY curveChanged)
    Q_PROPERTY(LineProperties* line READ line WRITE setLine NOTIFY lineChanged)
    Q_PROPERTY(PolygonProperties* polygon READ polygon WRITE setPolygon NOTIFY polygonChanged)

public:
    explicit MProperties(QObject *parent = nullptr);

    BaseProperties* base() const { return m_base; }
    void setBase(BaseProperties* base);

    CircleProperties* circle() const { return m_circle; }
    void setCircle(CircleProperties* circle);

    CurveProperties* curve() const { return m_curve; }
    void setCurve(CurveProperties* curve);

    LineProperties* line() const { return m_line; }
    void setLine(LineProperties* line);

    PolygonProperties* polygon() const { return m_polygon; }
    void setPolygon(PolygonProperties* polygon);

signals:
    void baseChanged();
    void circleChanged();
    void curveChanged();
    void lineChanged();
    void polygonChanged();

private:
    BaseProperties* m_base = nullptr;
    CircleProperties* m_circle = nullptr;
    CurveProperties* m_curve = nullptr;
    LineProperties* m_line = nullptr;
    PolygonProperties* m_polygon = nullptr;
};

#endif // MPROPERTIES_H
