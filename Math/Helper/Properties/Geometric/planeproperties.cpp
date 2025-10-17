#include "planeproperties.h"

PlaneProperties::PlaneProperties(QObject *parent)
    : QObject(parent),
      m_axisColor(Qt::white),
      m_lineColor(Qt::gray),
      m_labelColor(Qt::lightGray),
      m_step(50.0),
      m_showLabels(true),
      m_showGrid(true),
      m_showAxes(true),
      m_axisThickness(2.0),
      m_gridThickness(1.0),
      m_labelFontSize(12)
{
}

qreal PlaneProperties::height() const { return m_height; }

qreal PlaneProperties::width() const { return m_width; }

void PlaneProperties::setfromJSON(const QJsonObject &o)
{
    PlanePropData d = PlanePropData::fromJSON(o);
    setAxisColor(d.axisColor);
    setLineColor(d.lineColor);
    setLabelColor(d.labelColor);
    setStep(d.step);
    setShowLabels(d.showLabels);
    setShowGrid(d.showGrid);
    setShowAxes(d.showAxes);
    setAxisThickness(d.axisThickness);
    setGridThickness(d.gridThickness);
    setLabelFontSize(d.labelFontSize);
    setHeight(d.height);
    setWidth(d.width);
}

PlaneProperties::PlanePropData PlaneProperties::getData() const
{
    PlanePropData d;
    d.axisColor = m_axisColor;
    d.lineColor = m_lineColor;
    d.labelColor = m_labelColor;
    d.step = m_step;
    d.showLabels = m_showLabels;
    d.showGrid = m_showGrid;
    d.showAxes = m_showAxes;
    d.axisThickness = m_axisThickness;
    d.gridThickness = m_gridThickness;
    d.labelFontSize = m_labelFontSize;
    d.height = m_height;
    d.width = m_width;
    return d;
}

void PlaneProperties::setAxisColor(const QColor &color)
{
    if (m_axisColor != color)
    {
        m_axisColor = color;
        emit axisColorChanged();
    }
}

void PlaneProperties::setLineColor(const QColor &color)
{
    if (m_lineColor != color)
    {
        m_lineColor = color;
        emit lineColorChanged();
    }
}

void PlaneProperties::setLabelColor(const QColor &color)
{
    if (m_labelColor != color)
    {
        m_labelColor = color;
        emit labelColorChanged();
    }
}

void PlaneProperties::setStep(qreal step)
{
    if (!qFuzzyCompare(m_step, step) && step > 0)
    {
        m_step = step;
        emit stepChanged();
    }
}

void PlaneProperties::setShowLabels(bool show)
{
    if (m_showLabels != show)
    {
        m_showLabels = show;
        emit showLabelsChanged();
    }
}

void PlaneProperties::setShowGrid(bool show)
{
    if (m_showGrid != show)
    {
        m_showGrid = show;
        emit showGridChanged();
    }
}

void PlaneProperties::setShowAxes(bool show)
{
    if (m_showAxes != show)
    {
        m_showAxes = show;
        emit showAxesChanged();
    }
}

void PlaneProperties::setAxisThickness(qreal thickness)
{
    if (!qFuzzyCompare(m_axisThickness, thickness) && thickness > 0)
    {
        m_axisThickness = thickness;
        emit axisThicknessChanged();
    }
}

void PlaneProperties::setGridThickness(qreal thickness)
{
    if (!qFuzzyCompare(m_gridThickness, thickness) && thickness > 0)
    {
        m_gridThickness = thickness;
        emit gridThicknessChanged();
    }
}

void PlaneProperties::setLabelFontSize(int size)
{
    if (m_labelFontSize != size && size > 0)
    {
        m_labelFontSize = size;
        emit labelFontSizeChanged();
    }
}

void PlaneProperties::setHeight(qreal height)
{
    m_height = height;
    emit heightChanged();
}

void PlaneProperties::setWidth(qreal width)
{
    m_width = width;
    emit widthChanged();
}

QJsonDocument PlaneProperties::PlanePropData::toJson() const
{
    QJsonObject o;
    o["axisColor"] = QJsonObject{{"r", axisColor.red()}, {"g", axisColor.green()}, {"b", axisColor.blue()}, {"a", axisColor.alpha()}};
    o["lineColor"] = QJsonObject{{"r", lineColor.red()}, {"g", lineColor.green()}, {"b", lineColor.blue()}, {"a", lineColor.alpha()}};
    o["labelColor"] = QJsonObject{{"r", labelColor.red()}, {"g", labelColor.green()}, {"b", labelColor.blue()}, {"a", labelColor.alpha()}};
    o["step"] = step;
    o["showLabels"] = showLabels;
    o["showGrid"] = showGrid;
    o["showAxes"] = showAxes;
    o["axisThickness"] = axisThickness;
    o["gridThickness"] = gridThickness;
    o["labelFontSize"] = labelFontSize;
    o["height"] = height;
    o["width"] = width;
    return QJsonDocument(o);
}

PlaneProperties::PlanePropData PlaneProperties::PlanePropData::fromJSON(const QJsonObject &o)
{
    PlanePropData d;
    auto ac = o["axisColor"].toObject();
    d.axisColor = QColor(ac["r"].toInt(), ac["g"].toInt(), ac["b"].toInt(), ac["a"].toInt());
    auto lc = o["lineColor"].toObject();
    d.lineColor = QColor(lc["r"].toInt(), lc["g"].toInt(), lc["b"].toInt(), lc["a"].toInt());
    auto labc = o["labelColor"].toObject();
    d.labelColor = QColor(labc["r"].toInt(), labc["g"].toInt(), labc["b"].toInt(), labc["a"].toInt());
    d.step = o["step"].toDouble();
    d.showLabels = o["showLabels"].toBool();
    d.showGrid = o["showGrid"].toBool();
    d.showAxes = o["showAxes"].toBool();
    d.axisThickness = o["axisThickness"].toDouble();
    d.gridThickness = o["gridThickness"].toDouble();
    d.labelFontSize = o["labelFontSize"].toInt();
    d.height = o["height"].toDouble();
    d.width = o["width"].toDouble();
    return d;
}
