#include "SimpleVector.h"

SimpleVector::SimpleVector(Scene *canvas, QQuickItem *parent) : SimpleLine(canvas, parent)
{
}

QSGNode *SimpleVector::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
    QSGGeometryNode *node = static_cast<QSGGeometryNode *>(oldNode);

    if (!node)
    {
        node = new QSGGeometryNode;

        // Create geometry for line + arrowhead (6 vertices for line + 3 for arrow = 9 total)
        QSGGeometry *geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 9);
        geometry->setDrawingMode(QSGGeometry::DrawTriangles);

        node->setGeometry(geometry);
        node->setFlag(QSGNode::OwnsGeometry);

        QSGFlatColorMaterial *material = new QSGFlatColorMaterial;
        material->setColor(color());
        node->setMaterial(material);
        node->setFlag(QSGNode::OwnsMaterial);
    }

    QSGFlatColorMaterial *material = static_cast<QSGFlatColorMaterial *>(node->material());
    if (material)
    {
        material->setColor(color());
        node->markDirty(QSGNode::DirtyMaterial);
    }

    QSGGeometry *geometry = node->geometry();

    QVector2D p1_vec(p1());
    QVector2D p2_vec(p2());
    QVector2D dir = p2_vec - p1_vec;

    if (dir.lengthSquared() < 1e-6)
    {
        // Avoid drawing zero-length line
        return node;
    }

    createArrowGeometry(geometry, p1_vec, p2_vec, Thickness());

    node->markDirty(QSGNode::DirtyGeometry);
    return node;
}

void SimpleVector::createArrowGeometry(QSGGeometry *geometry, const QVector2D &p1, const QVector2D &p2, float thickness)
{
    geometry->allocate(9); // 6 vertices for line rectangle + 3 for arrowhead triangle
    QSGGeometry::Point2D *vertices = geometry->vertexDataAsPoint2D();

    QVector2D dir = p2 - p1;
    dir.normalize();

    QVector2D perp(-dir.y(), dir.x());
    perp *= thickness / 2.0f;

    // Calculate arrowhead dimensions
    float arrowLength = thickness * 2.5f; // Arrow length is 2.5x the line thickness
    float arrowWidth = thickness * 1.5f;  // Arrow width is 1.5x the line thickness

    // Adjust line end to account for arrow
    QVector2D lineEnd = p2 - dir * arrowLength * 0.7f; // Shorten line so arrow connects properly

    // Line rectangle (2 triangles = 6 vertices)
    vertices[0].set(p1.x() + perp.x(), p1.y() + perp.y());
    vertices[1].set(p1.x() - perp.x(), p1.y() - perp.y());
    vertices[2].set(lineEnd.x() + perp.x(), lineEnd.y() + perp.y());

    vertices[3].set(p1.x() - perp.x(), p1.y() - perp.y());
    vertices[4].set(lineEnd.x() - perp.x(), lineEnd.y() - perp.y());
    vertices[5].set(lineEnd.x() + perp.x(), lineEnd.y() + perp.y());

    // Arrowhead triangle (1 triangle = 3 vertices)
    QVector2D arrowPerp = perp * (arrowWidth / thickness); // Scale perpendicular for arrow width
    QVector2D arrowBase = p2 - dir * arrowLength;

    vertices[6].set(p2.x(), p2.y());                                               // Arrow tip
    vertices[7].set(arrowBase.x() + arrowPerp.x(), arrowBase.y() + arrowPerp.y()); // Arrow base left
    vertices[8].set(arrowBase.x() - arrowPerp.x(), arrowBase.y() - arrowPerp.y()); // Arrow base right
}