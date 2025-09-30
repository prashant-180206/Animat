#include "plane.h"
#include "Math/scene.h"

Plane::Plane(Scene *canvas, QQuickItem *parent)
    : Group(canvas, parent)
{
    properties->base()->setName("Plane");
    properties->setGeometric(new GeometricProperties(this));
    setupProperties();
    properties->geometric()->setPlane(m_planeProps);
    createGrid();
    createAxis();
    createLabels();
}

Plane::~Plane()
{
    clearGrid();
}

void Plane::setupProperties()
{
    // Create plane properties and set up in geometric properties
    m_planeProps = new PlaneProperties(this);
    properties->geometric()->setPlane(m_planeProps);

    // Set default values for plane appearance
    m_planeProps->setAxisColor(QColor(255, 255, 255));  // White axis lines
    m_planeProps->setLineColor(QColor(68, 68, 68));     // Dark gray grid lines
    m_planeProps->setLabelColor(QColor(204, 204, 204)); // Light gray labels
    m_planeProps->setStep(1.0);                         // 1 unit grid step
    m_planeProps->setShowGrid(true);                    // Show grid by default
    m_planeProps->setShowAxes(true);                    // Show axes by default
    m_planeProps->setShowLabels(true);                  // Show labels by default
    m_planeProps->setAxisThickness(2.0);                // Thicker axis lines
    m_planeProps->setGridThickness(1.0);                // Thinner grid lines
    m_planeProps->setLabelFontSize(12);                 // 12pt font size

    // Connect to property changes
    connect(m_planeProps, &PlaneProperties::axisColorChanged, this, &Plane::onPlanePropertiesChanged);
    connect(m_planeProps, &PlaneProperties::lineColorChanged, this, &Plane::onPlanePropertiesChanged);
    connect(m_planeProps, &PlaneProperties::stepChanged, this, &Plane::onPlanePropertiesChanged);
    connect(m_planeProps, &PlaneProperties::showGridChanged, this, &Plane::onPlanePropertiesChanged);
    connect(m_planeProps, &PlaneProperties::showAxesChanged, this, &Plane::onPlanePropertiesChanged);
    connect(m_planeProps, &PlaneProperties::showLabelsChanged, this, &Plane::onPlanePropertiesChanged);
    connect(m_planeProps, &PlaneProperties::axisThicknessChanged, this, &Plane::onPlanePropertiesChanged);
    connect(m_planeProps, &PlaneProperties::gridThicknessChanged, this, &Plane::onPlanePropertiesChanged);
    connect(m_planeProps, &PlaneProperties::labelFontSizeChanged, this, &Plane::onPlanePropertiesChanged);
    connect(m_planeProps, &PlaneProperties::labelColorChanged, this, &Plane::onPlanePropertiesChanged);
}

void Plane::setWidth(double width)
{
    if (qFuzzyCompare(m_width, width))
        return;
    m_width = width;
    updateGridLines();
    updateAxisLines();
    updateLabels();
    emit widthChanged();
}

void Plane::setHeight(double height)
{
    if (qFuzzyCompare(m_height, height))
        return;
    m_height = height;
    updateGridLines();
    updateAxisLines();
    updateLabels();
    emit heightChanged();
}

void Plane::setStep(double step)
{
    if (qFuzzyCompare(m_step, step))
        return;
    m_step = step;
    if (m_planeProps)
    {
        m_planeProps->setStep(step);
    }
    updateGridLines();
    updateLabels();
    emit stepChanged();
}

void Plane::clearGrid()
{
    // Clear existing grid lines
    for (SimpleLine *line : m_gridLines)
    {
        if (line)
        {
            line->deleteLater();
        }
    }
    m_gridLines.clear();

    // Clear axis lines
    for (SimpleLine *line : m_axisLines)
    {
        if (line)
        {
            line->deleteLater();
        }
    }
    m_axisLines.clear();

    // Clear labels
    for (SimpleText *label : m_labels)
    {
        if (label)
        {
            label->deleteLater();
        }
    }
    m_labels.clear();
}

void Plane::createGrid()
{
    if (!m_planeProps || !m_planeProps->showGrid())
        return;

    double step = m_planeProps->step();
    if (step <= 0)
        step = 1.0;

    // Vertical grid lines
    for (double x = -m_width / 2; x <= m_width / 2; x += step)
    {
        if (qFuzzyIsNull(x))
            continue; // Skip center line (will be axis)

        SimpleLine *line = new SimpleLine(getcanvas(), this);
        line->setP1(QPointF(x * 50, -m_height / 2 * 50)); // Scale by 50 for screen coordinates
        line->setP2(QPointF(x * 50, m_height / 2 * 50));
        line->setThickness(m_planeProps->gridThickness());
        line->setColor(m_planeProps->lineColor());
        line->setZ(this->z() - 0.1);

        addMember(line);
        m_gridLines.append(line);
    }

    // Horizontal grid lines
    for (double y = -m_height / 2; y <= m_height / 2; y += step)
    {
        if (qFuzzyIsNull(y))
            continue; // Skip center line (will be axis)

        SimpleLine *line = new SimpleLine(this->getcanvas(), this);
        line->setP1(QPointF(-m_width / 2 * 50, y * 50));
        line->setP2(QPointF(m_width / 2 * 50, y * 50));
        line->setThickness(m_planeProps->gridThickness());
        line->setColor(m_planeProps->lineColor());
        line->setZ(this->z() - 0.1);

        addMember(line);
        m_gridLines.append(line);
    }
}

void Plane::createAxis()
{
    if (!m_planeProps || !m_planeProps->showAxes())
        return;

    // X-axis (horizontal)
    SimpleLine *xAxis = new SimpleLine(getcanvas(), this);
    xAxis->setP1(QPointF(-m_width / 2 * 50, 0));
    xAxis->setP2(QPointF(m_width / 2 * 50, 0));
    xAxis->setThickness(m_planeProps->axisThickness());
    xAxis->setColor(m_planeProps->axisColor());
    xAxis->setZ(this->z());

    addMember(xAxis);
    m_axisLines.append(xAxis);

    // Y-axis (vertical)
    SimpleLine *yAxis = new SimpleLine(getcanvas(), this);
    yAxis->setP1(QPointF(0, -m_height / 2 * 50));
    yAxis->setP2(QPointF(0, m_height / 2 * 50));
    yAxis->setThickness(m_planeProps->axisThickness());
    yAxis->setColor(m_planeProps->axisColor());
    yAxis->setZ(this->z());

    addMember(yAxis);
    m_axisLines.append(yAxis);
}

void Plane::createLabels()
{
    if (!m_planeProps || !m_planeProps->showLabels())
        return;

    double step = m_planeProps->step();
    if (step <= 0)
        step = 1.0;

    // X-axis labels
    for (double x = -m_width / 2; x <= m_width / 2; x += step)
    {
        if (qFuzzyIsNull(x))
            continue; // Skip origin

        SimpleText *label = new SimpleText(getcanvas(), this);
        label->setText(QString::number(x));
        label->setFontSize(m_planeProps->labelFontSize());
        label->setColor(m_planeProps->labelColor());
        label->setPosition(QPointF(x * 50, -20)); // Position below x-axis
        label->setZ(this->z() + 0.1);

        addMember(label);
        m_labels.append(label);
    }

    // Y-axis labels
    for (double y = -m_height / 2; y <= m_height / 2; y += step)
    {
        if (qFuzzyIsNull(y))
            continue; // Skip origin

        SimpleText *label = new SimpleText(getcanvas(), this);
        label->setText(QString::number(y));
        label->setFontSize(m_planeProps->labelFontSize());
        label->setColor(m_planeProps->labelColor());
        label->setPosition(QPointF(-30, y * 50)); // Position left of y-axis
        label->setZ(this->z() + 0.1);

        addMember(label);
        m_labels.append(label);
    }
}

void Plane::updateGridLines()
{
    clearGrid();
    createGrid();
}

void Plane::updateAxisLines()
{
    // Clear only axis lines
    for (SimpleLine *line : std::as_const(m_axisLines))
    {
        if (line)
        {
            line->deleteLater();
        }
    }
    m_axisLines.clear();
    createAxis();
}

void Plane::updateLabels()
{
    // Clear only labels
    for (SimpleText *label : std::as_const(m_labels))
    {
        if (label)
        {
            label->deleteLater();
        }
    }
    m_labels.clear();
    createLabels();
}

void Plane::onPlanePropertiesChanged()
{
    // Recreate all elements when properties change
    clearGrid();
    createGrid();
    createAxis();
    createLabels();
}

bool Plane::contains(const QPointF &point) const
{
    // Define clickable area as the plane's bounding rectangle
    return boundingRect().contains(point);
}

QRectF Plane::boundingRect() const
{
    // Return the bounding rectangle based on plane dimensions
    // Convert from plane units to screen coordinates (50px per unit)
    qreal screenWidth = m_width * 50;
    qreal screenHeight = m_height * 50;

    // Center the rectangle around the plane's origin
    return QRectF(-screenWidth / 2, -screenHeight / 2, screenWidth, screenHeight);
}
