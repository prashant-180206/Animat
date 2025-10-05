#include "mproperties.h"

MProperties::MProperties(QObject *parent)
    : QObject(parent)
{
    // Initialize all property objects to avoid null pointer access
    // m_base = new BaseProperties(this);
    // m_circle = new CircleProperties(this);
    // m_curve = new CurveProperties(this);
    // m_line = new LineProperties(this);
    // m_polygon = new PolygonProperties(this);
}

void MProperties::setBase(BaseProperties *base)
{
    if (m_base == base)
        return;
    m_base = base;
    emit baseChanged();
}

void MProperties::setCircle(CircleProperties *circle)
{
    if (m_circle == circle)
        return;
    m_circle = circle;
    emit circleChanged();
}

void MProperties::setCurve(CurveProperties *curve)
{
    if (m_curve == curve)
        return;
    m_curve = curve;
    emit curveChanged();
}

void MProperties::setLine(LineProperties *line)
{
    if (m_line == line)
        return;
    m_line = line;
    emit lineChanged();
}

void MProperties::setPolygon(PolygonProperties *polygon)
{
    if (m_polygon == polygon)
        return;
    m_polygon = polygon;
    emit polygonChanged();
}

void MProperties::setText(TextProperties *text)
{
    if (m_text == text)
        return;
    m_text = text;
    emit textChanged();
}

void MProperties::setGeometric(GeometricProperties *geometric)
{
    if (m_geometric == geometric)
        return;
    m_geometric = geometric;
    emit geometricChanged();
}

void MProperties::setfromJSON(const QJsonObject &o)
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

MProperties::MPropData MProperties::getData() const
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

QJsonDocument MProperties::MPropData::toJson() const
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

MProperties::MPropData MProperties::MPropData::fromJSON(const QJsonObject &o, const MProperties *parent)
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
