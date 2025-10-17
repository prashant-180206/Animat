#include "Angle.h"
#include "Math/Scene.h"
#include "Math/Helper/Properties/geometricproperties.h"

Angle::Angle(Scene *canvas, QQuickItem *parent) : Group(canvas, parent)
{
    setFlag(ItemHasContents, true);

    // Initialize angle properties
    properties->setGeometric(new GeometricProperties(this->properties));
    properties->geometric()->setAngle(new AngleProperties(this->properties));

    // Set up basic properties
    properties->base()->setName("Angle");
    properties->base()->setType("Angle");
    properties->base()->setColor(Qt::blue);

    // Create the two vectors
    m_startVector = new SimpleVector(canvas, this);
    m_endVector = new SimpleVector(canvas, this);

    // Add vectors to the group
    addMember(m_startVector);
    addMember(m_endVector);

    // Set initial center point
    m_center = QPointF(0, 0);

    // Set initial vector properties
    m_startVector->setColor(properties->base()->color());
    m_endVector->setColor(properties->base()->color());
    m_startVector->setThickness(4);
    m_endVector->setThickness(4);

    // Connect property changes to update methods
    connect(properties->geometric()->angle(), &AngleProperties::angleChanged, this, &Angle::updateVectors);
    connect(properties->geometric()->angle(), &AngleProperties::startVectorLengthChanged, this, &Angle::updateVectors);
    connect(properties->geometric()->angle(), &AngleProperties::endVectorLengthChanged, this, &Angle::updateVectors);
    connect(properties->geometric()->angle(), &AngleProperties::arcColorChanged, this, &Angle::updateArc);
    connect(properties->geometric()->angle(), &AngleProperties::arcRadiusChanged, this, &Angle::updateArc);
    connect(properties->geometric()->angle(), &AngleProperties::arcThicknessChanged, this, &Angle::updateArc);

    // Connect base property changes
    connect(properties->base(), &BaseProperties::colorChanged, this, [this]()
            {
        m_startVector->setColor(properties->base()->color());
        m_endVector->setColor(properties->base()->color()); });

    // Initial setup
    updateGeometry();
}

void Angle::updateVectors()
{
    updateGeometry();
    update(); // Trigger arc redraw
}

void Angle::updateArc()
{
    update(); // Trigger arc redraw
}

void Angle::updateGeometry()
{
    if (!properties->geometric() || !properties->geometric()->angle())
        return;

    AngleProperties *angleProps = properties->geometric()->angle();

    // Calculate vector endpoints
    qreal startLength = angleProps->startVectorLength();
    qreal endLength = angleProps->endVectorLength();
    qreal angle = qDegreesToRadians(angleProps->angle());

    // Start vector is along positive X-axis
    QPointF startEnd = m_center + QPointF(startLength, 0) *getcanvas()->scalefactor();

    // End vector is rotated by the angle
    QPointF endEnd = m_center + QPointF(endLength * qCos(angle), -endLength * qSin(angle)) *getcanvas()->scalefactor();

    // Update vector positions
    m_startVector->setP1(m_center);
    m_startVector->setP2(startEnd);
    m_endVector->setP1(m_center);
    m_endVector->setP2(endEnd);
}

QSGNode *Angle::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
    if (!properties->geometric() || !properties->geometric()->angle())
        return oldNode;

    QSGGeometryNode *node = static_cast<QSGGeometryNode *>(oldNode);

    if (!node)
    {
        node = new QSGGeometryNode;

        // Create geometry for arc (multiple triangles for smooth curve)
        QSGGeometry *geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 96); // 32 segments * 3 vertices per triangle
        geometry->setDrawingMode(QSGGeometry::DrawTriangles);

        node->setGeometry(geometry);
        node->setFlag(QSGNode::OwnsGeometry);

        QSGFlatColorMaterial *material = new QSGFlatColorMaterial;
        material->setColor(properties->geometric()->angle()->arcColor());
        node->setMaterial(material);
        node->setFlag(QSGNode::OwnsMaterial);
    }

    QSGFlatColorMaterial *material = static_cast<QSGFlatColorMaterial *>(node->material());
    if (material)
    {
        material->setColor(properties->geometric()->angle()->arcColor());
        node->markDirty(QSGNode::DirtyMaterial);
    }

    QSGGeometry *geometry = node->geometry();

    AngleProperties *angleProps = properties->geometric()->angle();
    qreal angle = qDegreesToRadians(angleProps->angle());

    createArcGeometry(geometry, m_center, angleProps->arcRadius() *getcanvas()->scalefactor(), 0, angle, angleProps->arcThickness());

    node->markDirty(QSGNode::DirtyGeometry);
    return node;
}

void Angle::createArcGeometry(QSGGeometry *geometry, const QPointF &center, qreal radius,
                              qreal startAngle, qreal endAngle, qreal thickness, int segments)
{
    int vertexCount = segments * 6; // Each segment needs 2 triangles = 6 vertices
    geometry->allocate(vertexCount);
    QSGGeometry::Point2D *vertices = geometry->vertexDataAsPoint2D();

    qreal innerRadius = radius;
    qreal outerRadius = radius + thickness;
    qreal angleStep = (endAngle - startAngle) / segments;

    int vertexIndex = 0;

    for (int i = 0; i < segments; ++i)
    {
        qreal angle1 = startAngle + i * angleStep;
        qreal angle2 = startAngle + (i + 1) * angleStep;

        // Calculate points for this segment
        QPointF inner1 = center + QPointF(innerRadius * qCos(angle1),- innerRadius * qSin(angle1));
        QPointF outer1 = center + QPointF(outerRadius * qCos(angle1),- outerRadius * qSin(angle1));
        QPointF inner2 = center + QPointF(innerRadius * qCos(angle2),- innerRadius * qSin(angle2));
        QPointF outer2 = center + QPointF(outerRadius * qCos(angle2),- outerRadius * qSin(angle2));

        // First triangle: inner1, outer1, inner2
        vertices[vertexIndex++].set(inner1.x(), inner1.y());
        vertices[vertexIndex++].set(outer1.x(), outer1.y());
        vertices[vertexIndex++].set(inner2.x(), inner2.y());

        // Second triangle: inner2, outer1, outer2
        vertices[vertexIndex++].set(inner2.x(), inner2.y());
        vertices[vertexIndex++].set(outer1.x(), outer1.y());
        vertices[vertexIndex++].set(outer2.x(), outer2.y());
    }
}

QRectF Angle::boundingRect() const
{
    if (!properties->geometric() || !properties->geometric()->angle())
        return QRectF();

    AngleProperties *angleProps = properties->geometric()->angle();

    // Calculate the maximum extent of vectors and arc
    qreal maxLength = qMax(angleProps->startVectorLength(), angleProps->endVectorLength());
    qreal arcExtent = angleProps->arcRadius() + angleProps->arcThickness();
    qreal maxExtent = qMax(maxLength, arcExtent);

    // Add some padding
    qreal padding = 20;
    return QRectF(m_center.x() - maxExtent - padding,
                  m_center.y() - maxExtent - padding,
                  2 * (maxExtent + padding),
                  2 * (maxExtent + padding));
}

bool Angle::contains(const QPointF &point) const
{
    // Check if point is within the bounding rectangle first
    if (!boundingRect().contains(point))
        return false;

    // For simplicity, consider any point within the bounding rect as contained
    // More sophisticated containment checking could check if point is near vectors or arc
    return true;
}
