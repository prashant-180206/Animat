#include "line.h"


Line::Line(Scene* canvas,QQuickItem* parent) :Mobject(canvas,parent){
    if (parent) {

    }
    setFlag(ItemHasContents, true);
    connect(this, &QQuickItem::windowChanged, this, [this](QQuickWindow* w){
        if(w) update();
    });
}

QPointF Line::p1() const { return m_p1; }
void Line::setP1(const QPointF &pt) {
    if (m_p1 == pt)
        return;
    m_p1 = pt;
    emit p1Changed();
    update();
}
QPointF Line::p2() const { return m_p2; }
void Line::setP2(const QPointF &pt) {
    if (m_p2 == pt)
        return;
    m_p2 = pt;
    emit p2Changed();
    update();
}

QSGNode *Line::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
    QSGGeometryNode *node = static_cast<QSGGeometryNode *>(oldNode);

    if (!node) {
        node = new QSGGeometryNode;

        QSGGeometry *geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 4);
        geometry->setDrawingMode(QSGGeometry::DrawTriangleStrip);

        node->setGeometry(geometry);
        node->setFlag(QSGNode::OwnsGeometry);

        QSGFlatColorMaterial *material = new QSGFlatColorMaterial;
        material->setColor(Qt::white);
        node->setMaterial(material);
        node->setFlag(QSGNode::OwnsMaterial);
    }

    QSGGeometry *geometry = node->geometry();
    geometry->allocate(4);
    QSGGeometry::Point2D *vertices = geometry->vertexDataAsPoint2D();

    QVector2D p1_vec(m_p1);
    QVector2D p2_vec(m_p2);
    QVector2D dir = p2_vec - p1_vec;

    if (dir.lengthSquared() < 1e-6) {
        // Avoid drawing zero-length line
        return node;
    }

    dir.normalize();

    float thickness = 4.0f;
    QVector2D perp(-dir.y(), dir.x());
    perp *= thickness / 2.0f;

    vertices[0].set(p1_vec.x() + perp.x(), p1_vec.y() + perp.y());
    vertices[1].set(p1_vec.x() - perp.x(), p1_vec.y() - perp.y());
    vertices[2].set(p2_vec.x() + perp.x(), p2_vec.y() + perp.y());
    vertices[3].set(p2_vec.x() - perp.x(), p2_vec.y() - perp.y());

    node->markDirty(QSGNode::DirtyGeometry);

    return node;
}

