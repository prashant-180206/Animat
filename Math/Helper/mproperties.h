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

        QJsonDocument toJson() const
        {
            QJsonObject o;
            o["base"] = base.toJson().object();
            o["circle"] = circle.toJson().object();
            o["curve"] = curve.toJson().object();
            o["line"] = line.toJson().object();
            o["polygon"] = polygon.toJson().object();
            o["text"] = text.toJson().object();
            o["geometric"] = geometric.toJson().object();
            return QJsonDocument(o);
        }

        static MPropData fromJSON(const QJsonObject &o, const MProperties *parent = nullptr)
        {
            MPropData d;
            if (parent && parent->base() && o.contains("base"))
                d.base = BaseProperties::basePropData::fromJSON(o["base"].toObject());
            if (parent && parent->circle() && o.contains("circle"))
                d.circle = CircleProperties::CirclePropData::fromJSON(o["circle"].toObject());
            if (parent && parent->curve() && o.contains("curve"))
                d.curve = CurveProperties::CurvePropData::fromJSON(o["curve"].toObject());
            if (parent && parent->line() && o.contains("line"))
                d.line = LineProperties::LinePropData::fromJSON(o["line"].toObject());
            if (parent && parent->polygon() && o.contains("polygon"))
                d.polygon = PolygonProperties::PolygonPropData::fromJSON(o["polygon"].toObject());
            if (parent && parent->text() && o.contains("text"))
                d.text = TextProperties::TextPropData::fromJSON(o["text"].toObject());
            if (parent && parent->geometric() && o.contains("geometric"))
                d.geometric = GeometricProperties::GeometricPropData::fromJSON(o["geometric"].toObject());
            return d;
        }
    };

    MPropData getData() const
    {
        MPropData d;
        if (m_base)
            d.base = m_base->getData();
        if (m_circle)
            d.circle = m_circle->getData();
        if (m_curve)
            d.curve = m_curve->getData();
        if (m_line)
            d.line = m_line->getData();
        if (m_polygon)
            d.polygon = m_polygon->getData();
        if (m_text)
            d.text = m_text->getData();
        if (m_geometric)
            d.geometric = m_geometric->getData();
        return d;
    }

    void setfromJSON(const QJsonObject &o)
    {
        MPropData d = MPropData::fromJSON(o, this);
        if (m_base)
            m_base->setfromJSON(o["base"].toObject());
        if (m_circle)
            m_circle->setfromJSON(o["circle"].toObject());
        if (m_curve)
            m_curve->setfromJSON(o["curve"].toObject());
        if (m_line)
            m_line->setfromJSON(o["line"].toObject());
        if (m_polygon)
            m_polygon->setfromJSON(o["polygon"].toObject());
        if (m_text)
            m_text->setfromJSON(o["text"].toObject());
        if (m_geometric)
            m_geometric->setfromJSON(o["geometric"].toObject());
    }
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
