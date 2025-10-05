#ifndef MPROPERTIES_H
#define MPROPERTIES_H

#include "baseproperties.h"
#include "circleproperties.h"
#include "curveproperties.h"
#include "lineproperties.h"
#include "polygonproperties.h"
#include "textproperties.h"
#include "geometricproperties.h"
#include <QObject>
#include <qqmlintegration.h>

class MProperties : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(BaseProperties *base READ base WRITE setBase NOTIFY baseChanged)
    Q_PROPERTY(CircleProperties *circle READ circle WRITE setCircle NOTIFY circleChanged)
    Q_PROPERTY(CurveProperties *curve READ curve WRITE setCurve NOTIFY curveChanged)
    Q_PROPERTY(LineProperties *line READ line WRITE setLine NOTIFY lineChanged)
    Q_PROPERTY(PolygonProperties *polygon READ polygon WRITE setPolygon NOTIFY polygonChanged)
    Q_PROPERTY(TextProperties *text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(GeometricProperties *geometric READ geometric WRITE setGeometric NOTIFY geometricChanged)

public:
    explicit MProperties(QObject *parent = nullptr);

    BaseProperties *base() const { return m_base; }
    void setBase(BaseProperties *base);

    CircleProperties *circle() const { return m_circle; }
    void setCircle(CircleProperties *circle);

    CurveProperties *curve() const { return m_curve; }
    void setCurve(CurveProperties *curve);

    LineProperties *line() const { return m_line; }
    void setLine(LineProperties *line);

    PolygonProperties *polygon() const { return m_polygon; }
    void setPolygon(PolygonProperties *polygon);

    TextProperties *text() const { return m_text; }
    void setText(TextProperties *text);

    GeometricProperties *geometric() const { return m_geometric; }
    void setGeometric(GeometricProperties *geometric);

    struct MPropData
    {
        BaseProperties::basePropData base;
        CircleProperties::CirclePropData circle;
        CurveProperties::CurvePropData curve;
        LineProperties::LinePropData line;
        PolygonProperties::PolygonPropData polygon;
        TextProperties::TextPropData text;
        GeometricProperties::GeometricPropData geometric;

        QJsonDocument toJson() const;

        static MPropData fromJSON(const QJsonObject &o, const MProperties *parent = nullptr);
    };

    MPropData getData() const;

    void setfromJSON(const QJsonObject &o);
signals:
    void baseChanged();
    void circleChanged();
    void curveChanged();
    void lineChanged();
    void polygonChanged();
    void textChanged();
    void geometricChanged();

private:
    BaseProperties *m_base = nullptr;
    CircleProperties *m_circle = nullptr;
    CurveProperties *m_curve = nullptr;
    LineProperties *m_line = nullptr;
    PolygonProperties *m_polygon = nullptr;
    TextProperties *m_text = nullptr;
    GeometricProperties *m_geometric = nullptr;
};

#endif // MPROPERTIES_H
