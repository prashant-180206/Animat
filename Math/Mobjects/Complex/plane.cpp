#include "plane.h"
#include "Math/scene.h"

Plane::Plane(Scene *canvas, QQuickItem *parent)
    : Group(canvas, parent)
{
    properties->base()->setName("Plane");
    properties->base()->setType("Plane");

    // Initialize PlaneProperties first

    // Set up geometric properties
    properties->setGeometric(new GeometricProperties(this));
    properties->geometric()->setPlane(new PlaneProperties(this));

    setupProperties();
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
    // Set default values for plane appearance
    properties->geometric()->plane()->setAxisColor(QColor(255, 255, 255));  // White axis lines
    properties->geometric()->plane()->setLineColor(QColor(68, 68, 68));     // Dark gray grid lines
    properties->geometric()->plane()->setLabelColor(QColor(204, 204, 204)); // Light gray labels
    properties->geometric()->plane()->setStep(1.0);                         // 1 unit grid step
    properties->geometric()->plane()->setWidth(10.0);                       // Default width
    properties->geometric()->plane()->setHeight(10.0);                      // Default height
    properties->geometric()->plane()->setShowGrid(true);                    // Show grid by default
    properties->geometric()->plane()->setShowAxes(true);                    // Show axes by default
    properties->geometric()->plane()->setShowLabels(true);                  // Show labels by default
    properties->geometric()->plane()->setAxisThickness(2.0);                // Thicker axis lines
    properties->geometric()->plane()->setGridThickness(1.0);                // Thinner grid lines
    properties->geometric()->plane()->setLabelFontSize(12);                 // 12pt font size

    // Connect to property changes
    connect(properties->geometric()->plane(), &PlaneProperties::axisColorChanged, this, &Plane::onPlanePropertiesChanged);
    connect(properties->geometric()->plane(), &PlaneProperties::lineColorChanged, this, &Plane::onPlanePropertiesChanged);
    connect(properties->geometric()->plane(), &PlaneProperties::stepChanged, this, &Plane::onPlanePropertiesChanged);
    connect(properties->geometric()->plane(), &PlaneProperties::widthChanged, this, &Plane::onPlanePropertiesChanged);
    connect(properties->geometric()->plane(), &PlaneProperties::heightChanged, this, &Plane::onPlanePropertiesChanged);
    connect(properties->geometric()->plane(), &PlaneProperties::showGridChanged, this, &Plane::onPlanePropertiesChanged);
    connect(properties->geometric()->plane(), &PlaneProperties::showAxesChanged, this, &Plane::onPlanePropertiesChanged);
    connect(properties->geometric()->plane(), &PlaneProperties::showLabelsChanged, this, &Plane::onPlanePropertiesChanged);
    connect(properties->geometric()->plane(), &PlaneProperties::axisThicknessChanged, this, &Plane::onPlanePropertiesChanged);
    connect(properties->geometric()->plane(), &PlaneProperties::gridThicknessChanged, this, &Plane::onPlanePropertiesChanged);
    connect(properties->geometric()->plane(), &PlaneProperties::labelFontSizeChanged, this, &Plane::onPlanePropertiesChanged);
    connect(properties->geometric()->plane(), &PlaneProperties::labelColorChanged, this, &Plane::onPlanePropertiesChanged);
}

void Plane::clearGrid()
{
    // Clear existing grid lines
    for (SimpleLine *line : std::as_const(m_gridLines))
    {
        if (line)
        {
            line->deleteLater();
        }
    }
    m_gridLines.clear();

    // Clear axis lines
    for (SimpleLine *line : std::as_const(m_axisLines))
    {
        if (line)
        {
            line->deleteLater();
        }
    }
    m_axisLines.clear();

    // Clear labels
    for (SimpleText *label : std::as_const(m_labels))
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
    if (!properties->geometric()->plane() || !properties->geometric()->plane()->showGrid())
        return;

    double step = properties->geometric()->plane()->step();
    if (step <= 0)
        step = 1.0;

    qreal m_height = properties->geometric()->plane()->height();
    qreal m_width = properties->geometric()->plane()->width();

    // Vertical grid lines
    for (qreal x = -m_width / 2; x <= m_width / 2; x += step)
    {
        if (qFuzzyIsNull(x))
            continue; // Skip center line (will be axis)

        SimpleLine *line = new SimpleLine(getcanvas(), this);
        line->setP1(QPointF(x * getcanvas()->scalefactor(), -m_height / 2 * getcanvas()->scalefactor())); // Scale by getcanvas()->scalefactor() for screen coordinates
        line->setP2(QPointF(x * getcanvas()->scalefactor(), m_height / 2 * getcanvas()->scalefactor()));
        line->setThickness(properties->geometric()->plane()->gridThickness());
        line->setColor(properties->geometric()->plane()->lineColor());
        line->setZ(this->z() - 0.1);

        addMember(line);
        m_gridLines.append(line);
    }

    // Horizontal grid lines
    for (qreal y = -m_height / 2; y <= m_height / 2; y += step)
    {
        if (qFuzzyIsNull(y))
            continue; // Skip center line (will be axis)

        SimpleLine *line = new SimpleLine(this->getcanvas(), this);
        line->setP1(QPointF(-m_width / 2 * getcanvas()->scalefactor(), y * getcanvas()->scalefactor()));
        line->setP2(QPointF(m_width / 2 * getcanvas()->scalefactor(), y * getcanvas()->scalefactor()));
        line->setThickness(properties->geometric()->plane()->gridThickness());
        line->setColor(properties->geometric()->plane()->lineColor());
        line->setZ(this->z() - 0.1);

        addMember(line);
        m_gridLines.append(line);
    }
}

void Plane::createAxis()
{
    if (!properties->geometric()->plane() || !properties->geometric()->plane()->showAxes())
        return;

    qreal m_height = properties->geometric()->plane()->height();
    qreal m_width = properties->geometric()->plane()->width();

    // X-axis (horizontal)
    SimpleLine *xAxis = new SimpleLine(getcanvas(), this);
    xAxis->setP1(QPointF(-m_width / 2 * getcanvas()->scalefactor(), 0));
    xAxis->setP2(QPointF(m_width / 2 * getcanvas()->scalefactor(), 0));
    xAxis->setThickness(properties->geometric()->plane()->axisThickness());
    xAxis->setColor(properties->geometric()->plane()->axisColor());
    xAxis->setZ(this->z());

    addMember(xAxis);
    m_axisLines.append(xAxis);

    // Y-axis (vertical)
    SimpleLine *yAxis = new SimpleLine(getcanvas(), this);

    yAxis->setP1(QPointF(0, -m_height / 2 * getcanvas()->scalefactor()));
    yAxis->setP2(QPointF(0, m_height / 2 * getcanvas()->scalefactor()));
    yAxis->setThickness(properties->geometric()->plane()->axisThickness());
    yAxis->setColor(properties->geometric()->plane()->axisColor());
    yAxis->setZ(this->z());

    addMember(yAxis);
    m_axisLines.append(yAxis);
}

void Plane::createLabels()
{
    if (!properties->geometric()->plane() || !properties->geometric()->plane()->showLabels())
        return;

    double step = properties->geometric()->plane()->step();
    if (step <= 0)
        step = 1.0;

    // X-axis labels
    qreal m_height = properties->geometric()->plane()->height();
    qreal m_width = properties->geometric()->plane()->width();
    for (qreal x = -m_width / 2; x <= m_width / 2; x += step)
    {
        if (qFuzzyIsNull(x))
            continue; // Skip origin

        SimpleText *label = new SimpleText(getcanvas(), this);
        label->setText(QString::number(x));
        label->setFontSize(properties->geometric()->plane()->labelFontSize());
        label->setColor(properties->geometric()->plane()->labelColor());
        label->setPosition(QPointF(x * getcanvas()->scalefactor(), -20)); // Position below x-axis
        label->setZ(this->z() + 0.1);

        addMember(label);
        m_labels.append(label);
    }

    // Y-axis labels
    for (qreal y = -m_height / 2; y <= m_height / 2; y += step)
    {
        if (qFuzzyIsNull(y))
            continue; // Skip origin

        SimpleText *label = new SimpleText(getcanvas(), this);
        label->setText(QString::number(y));
        label->setFontSize(properties->geometric()->plane()->labelFontSize());
        label->setColor(properties->geometric()->plane()->labelColor());
        label->setPosition(QPointF(-30, y * getcanvas()->scalefactor())); // Position left of y-axis
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
    qInfo() << "PROP CHANGES";
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
    // Convert from plane units to screen coordinates (getcanvas()->scalefactor()px per unit)
    qreal screenWidth = properties->geometric()->plane()->width() * getcanvas()->scalefactor();
    qreal screenHeight = properties->geometric()->plane()->height() * getcanvas()->scalefactor();

    // Center the rectangle around the plane's origin with the shift applied
    return QRectF(-screenWidth / 2, -screenHeight / 2, screenWidth, screenHeight);
}
