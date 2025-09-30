#include "SimpleDot.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

SimpleDot::SimpleDot(Scene *canvas, QQuickItem *parent)
    : Mobject(parent), m_canvas(canvas)
{
    setFlag(ItemHasContents, true);
}

void SimpleDot::setPos(const QPointF &pt)
{
    if (m_pos != pt)
    {
        m_pos = pt;
        emit posChanged();
        update();
    }
}

void SimpleDot::setSize(qreal s)
{
    if (m_size != s)
    {
        m_size = s;
        emit sizeChanged();
        update();
    }
}

void SimpleDot::setColor(const QColor &c)
{
    if (m_color != c)
    {
        m_color = c;
        emit colorChanged();
        update();
    }
}

QSGNode *SimpleDot::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
    QSGGeometryNode *node = static_cast<QSGGeometryNode *>(oldNode);

    if (!node)
    {
        node = new QSGGeometryNode;

        // Create geometry for circle (24 segments = 72 vertices for triangle fan)
        int segments = 24;
        int vertexCount = segments * 3; // 3 vertices per triangle
        QSGGeometry *geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), vertexCount);
        geometry->setDrawingMode(QSGGeometry::DrawTriangles);

        node->setGeometry(geometry);
        node->setFlag(QSGNode::OwnsGeometry);

        QSGFlatColorMaterial *material = new QSGFlatColorMaterial;
        material->setColor(m_color);
        node->setMaterial(material);
        node->setFlag(QSGNode::OwnsMaterial);
    }

    QSGFlatColorMaterial *material = static_cast<QSGFlatColorMaterial *>(node->material());
    if (material)
    {
        material->setColor(m_color);
        node->markDirty(QSGNode::DirtyMaterial);
    }

    QSGGeometry *geometry = node->geometry();
    int segments = 24;
    geometry->allocate(segments * 3);
    QSGGeometry::Point2D *vertices = geometry->vertexDataAsPoint2D();

    float radius = m_size / 2.0f;
    float centerX = m_pos.x();
    float centerY = m_pos.y();

    int vertexIndex = 0;

    // Draw circle as triangle fan
    for (int i = 0; i < segments; ++i)
    {
        float angle1 = (i * 2.0f * M_PI) / segments;
        float angle2 = ((i + 1) * 2.0f * M_PI) / segments;

        // Triangle: center + two edge points
        vertices[vertexIndex++].set(centerX, centerY); // Center
        vertices[vertexIndex++].set(centerX + radius * cos(angle1), centerY + radius * sin(angle1));
        vertices[vertexIndex++].set(centerX + radius * cos(angle2), centerY + radius * sin(angle2));
    }

    node->markDirty(QSGNode::DirtyGeometry);
    return node;
}