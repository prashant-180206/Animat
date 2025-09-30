#include "Dot.h"
#include "Math/Scene.h"
#include <QSGSimpleRectNode>
#include <QSGGeometry>
#include <QSGFlatColorMaterial>
#include <QSGGeometryNode>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <cmath>

Dot::Dot(Scene *canvas, QQuickItem *parent)
    : ClickableMobject(canvas, parent), m_position(0, 0)
{
    setFlag(ItemHasContents, true);

    // Initialize properties specific to Dot
    if (properties && properties->base())
    {
        properties->base()->setName("Dot");
        properties->base()->setType("Dot");
        properties->base()->setColor(Qt::yellow);
        // properties->base()->setSize({10,10});
    }

    // Connect to property changes
    if (properties && properties->base())
    {
        connect(properties->base(), &BaseProperties::colorChanged, this, &Dot::dotColorChanged);
    }

    updateDotSize();
}

void Dot::setDotColor(const QColor &color)
{
    if (properties && properties->base())
    {
        properties->base()->setColor(color);
        update();
        emit dotColorChanged();
    }
}

void Dot::setRadius(qreal radius)
{
    if (m_radius != radius)
    {
        m_radius = radius;
        updateDotSize();
        update();
        emit radiusChanged();
    }
}

void Dot::setCenter(qreal x, qreal y)
{
    m_position = QPointF(x, y);
    ClickableMobject::setCenter(x, y);
}

void Dot::updateDotSize()
{
    // Use a simple fixed size without any scaling
    qreal diameter = m_radius * 2; // Simple: radius of 5 = diameter of 10

    setWidth(diameter);
    setHeight(diameter);
}

QSGNode *Dot::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
    QSGGeometryNode *node = static_cast<QSGGeometryNode *>(oldNode);

    if (!node)
    {
        node = new QSGGeometryNode;

        // Create geometry for a circle using triangles
        QSGGeometry *geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 0);
        node->setGeometry(geometry);
        node->setFlag(QSGNode::OwnsGeometry);

        // Create material
        QSGFlatColorMaterial *material = new QSGFlatColorMaterial;
        node->setMaterial(material);
        node->setFlag(QSGNode::OwnsMaterial);
    }

    // Update geometry for circle
    QSGGeometry *geometry = node->geometry();
    QSGFlatColorMaterial *material = static_cast<QSGFlatColorMaterial *>(node->material());

    // Create circle using triangles (more compatible than triangle fan)
    const int segments = 48; // Increased for smoother circle
    const int triangleCount = segments;
    const int vertexCount = triangleCount * 3; // 3 vertices per triangle

    geometry->allocate(vertexCount);
    QSGGeometry::Point2D *vertices = geometry->vertexDataAsPoint2D();

    qreal scaledRadius = m_radius; // Use simple radius without canvas scaling
    qreal centerX = width() / 2.0-m_radius;
    qreal centerY = height() / 2.0-m_radius;

    // Create triangles from center to perimeter
    int vertexIndex = 0;
    for (int i = 0; i < segments; ++i)
    {
        qreal angle1 = 2.0 * M_PI * i / segments;
        qreal angle2 = 2.0 * M_PI * (i + 1) / segments;

        qreal x1 = centerX + scaledRadius * cos(angle1);
        qreal y1 = centerY + scaledRadius * sin(angle1);
        qreal x2 = centerX + scaledRadius * cos(angle2);
        qreal y2 = centerY + scaledRadius * sin(angle2);

        // Triangle: center, point1, point2
        vertices[vertexIndex++].set(centerX, centerY);
        vertices[vertexIndex++].set(x1, y1);
        vertices[vertexIndex++].set(x2, y2);
    }

    geometry->setDrawingMode(QSGGeometry::DrawTriangles);
    geometry->markVertexDataDirty();

    // Update color
    QColor color = dotColor();
    if (properties && properties->base())
    {
        color.setAlphaF(properties->base()->opacity());
    }
    material->setColor(color);

    node->markDirty(QSGNode::DirtyGeometry | QSGNode::DirtyMaterial);

    return node;
}

bool Dot::contains(const QPointF &point) const
{
    qreal scaledRadius = m_radius; // Use simple radius without canvas scaling
    QPointF center(width() / 2.0-m_radius, height() / 2.0-m_radius);
    QPointF diff = point - center;
    qreal distance = sqrt(diff.x() * diff.x() + diff.y() * diff.y());
    return distance <= scaledRadius;
}

QRectF Dot::boundingRect() const
{
    qreal scaledRadius = m_radius; // Use simple radius without canvas scaling
    qreal diameter = scaledRadius * 2;
    return QRectF(0, 0, diameter, diameter);
}
