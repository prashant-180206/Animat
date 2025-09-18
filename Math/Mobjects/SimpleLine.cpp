#include "Math/Mobjects/SimpleLine.h"


SimpleLine::SimpleLine(Scene* canvas,QQuickItem* parent) :Mobject(parent){
    if (parent) {
    }
    setFlag(ItemHasContents, true);
}

QPointF SimpleLine::p1() const { return m_p1; }

void SimpleLine::setP1(const QPointF &pt) {
    m_p1 = pt;
    emit p1Changed();
    update();
}

QPointF SimpleLine::p2() const { return m_p2; }

void SimpleLine::setP2(const QPointF &pt) {
    m_p2 = pt;
    emit p2Changed();
    update();
}

QSGNode *SimpleLine::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
    QSGGeometryNode *node = static_cast<QSGGeometryNode *>(oldNode);

    if (!node) {
        node = new QSGGeometryNode;

        QSGGeometry *geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 4);
        geometry->setDrawingMode(QSGGeometry::DrawTriangleStrip);

        node->setGeometry(geometry);
        node->setFlag(QSGNode::OwnsGeometry);

        QSGFlatColorMaterial *material = new QSGFlatColorMaterial;
        material->setColor(color());
        node->setMaterial(material);
        node->setFlag(QSGNode::OwnsMaterial);
    }
    QSGFlatColorMaterial *material = static_cast<QSGFlatColorMaterial *>(node->material());
    if (material) {
        material->setColor(color());
        node->markDirty(QSGNode::DirtyMaterial);
    }


    QSGGeometry *geometry = node->geometry();
    geometry->allocate(4);
    QSGGeometry::Point2D *vertices = geometry->vertexDataAsPoint2D();

    // qDebug()<<"painting with "<<m_p1<<m_p2;

    QVector2D p1_vec(m_p1);
    QVector2D p2_vec(m_p2);
    QVector2D dir = p2_vec - p1_vec;


    // qDebug()<<p1_vec<<p2_vec<<dir;

    if (dir.lengthSquared() < 1e-6) {
        // Avoid drawing zero-length line
        return node;
    }

    dir.normalize();

    float thickness = Thickness();
    QVector2D perp(-dir.y(), dir.x());
    perp *= thickness / 2.0f;

    // Extend ends by thickness/2 along the line direction
    QVector2D extension =dir* (thickness / 2.0f);
    // extension*=0;

    vertices[0].set((p1_vec - extension).x() + perp.x(), (p1_vec - extension).y() + perp.y());
    vertices[1].set((p1_vec - extension).x() - perp.x(), (p1_vec - extension).y() - perp.y());
    vertices[2].set((p2_vec + extension).x() + perp.x(), (p2_vec + extension).y() + perp.y());
    vertices[3].set((p2_vec + extension).x() - perp.x(), (p2_vec + extension).y() - perp.y());

    node->markDirty(QSGNode::DirtyGeometry);

    // qDebug()<<vertices;

    return node;
}



