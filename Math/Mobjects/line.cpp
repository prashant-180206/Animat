#include "Math/Mobjects/line.h"


Line::Line(Scene* canvas,QQuickItem* parent) :ClickableMobject(canvas,parent){



    setP1(canvas->c2p(QPointF(-1,1)));
    setP2(canvas->c2p(QPointF(1,1)));

    setFlag(ItemHasContents, true);
    connect(this, &QQuickItem::windowChanged, this, [this](QQuickWindow* w){
        if(w){
            update();
        }
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

QRectF Line::boundingRect() const
{
    qreal penWidth = 4.0;  // your thickness
    QRectF rect(m_p1, m_p2);
    rect = rect.normalized();
    rect.adjust(-penWidth, -penWidth, penWidth, penWidth); // Add padding for thickness
    return rect;
}

void Line::setCenter(float x, float y)
{
    QPointF currentCenter = (m_p1 + m_p2) * 0.5;
    QPointF newCenter(x, y);
    QPointF offset = newCenter - currentCenter;

    setP1(getcanvas()->c2p((m_p1 + offset)/getcanvas()->scalefactor()));
    setP2(getcanvas()->c2p((m_p2 + offset)/getcanvas()->scalefactor()));
}

qreal Line::width() const
{
    return boundingRect().width();
}

qreal Line::height() const
{
    return boundingRect().height();
}


bool Line::contains(const QPointF &point) const
{
    // Compute distance from 'point' to line segment (m_p1, m_p2)
    QVector2D p(point);
    QVector2D p1(m_p1);
    QVector2D p2(m_p2);
    QVector2D v = p2 - p1;
    QVector2D w = p - p1;

    float c1 = QVector2D::dotProduct(w, v);
    if (c1 <= 0)
        return (p - p1).length() <= 4.0; // 4 = thickness / 2

    float c2 = QVector2D::dotProduct(v, v);
    if (c2 <= c1)
        return (p - p2).length() <= 4.0;

    float b = c1 / c2;
    QVector2D pb = p1 + b * v;
    return (p - pb).length() <= 4.0;
}


