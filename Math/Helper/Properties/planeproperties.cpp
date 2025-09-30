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