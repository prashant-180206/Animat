#include "Math/Mobjects/line.h"


Line::Line(Scene* canvas,QQuickItem* parent) :ClickableMobject(canvas,parent){


    auto p1 =canvas->p2c(QPointF(-1,-1));
    auto p2 = canvas->p2c(QPointF(1,1));

    qDebug()<<p1<<p2;
    setP1(p1);
    setP2(p2);

    setColor(Qt::yellow);

    setFlag(ItemHasContents, true);
    connect(this, &QQuickItem::windowChanged, this, [this](QQuickWindow* w){
        if(w){
            update();
        }
    });
    properties["Name"]="Line";
    properties.remove("Width");
    properties.remove("Height");
    properties.remove("x");
    properties.remove("y");
    properties["P1"]=p1;
    properties["P2"]=p2;
    properties["Color"]="p";
    properties["Thickness"]=Thickness();

}

QPointF Line::p1() const { return m_p1; }

void Line::setP1(const QPointF &pt) {
    // auto p = getcanvas()->p2c(pt);
    m_p1 = pt;

    update();
}
QPointF Line::p2() const { return m_p2; }

void Line::setP2(const QPointF &pt) {
    // auto p = getcanvas()->p2c(pt);
    m_p2 = pt;
    update();
}

void Line::setCenter(qreal x, qreal y)
{
    auto offset =  getcanvas()->p2c(QPointF(x,y));
    auto c =(m_p1 + m_p2)/2;
    qDebug()<<offset<<"center"<<c <<"offset";
    offset = offset -c;
    qDebug()<<m_p1<<m_p2 <<"mps";
    qDebug()<<offset <<"offset";

    setP2(m_p2 +offset);
    setP1(m_p1 +offset);
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
        material->setColor(color());
        node->setMaterial(material);
        node->setFlag(QSGNode::OwnsMaterial);
    }

    QSGGeometry *geometry = node->geometry();
    geometry->allocate(4);
    QSGGeometry::Point2D *vertices = geometry->vertexDataAsPoint2D();

    QVector2D p1_vec(m_p1);
    QVector2D p2_vec(m_p2);
    QVector2D dir = p2_vec - p1_vec;

    qDebug()<<p1_vec<<p2_vec<<dir;
    qDebug()<<m_p1<<m_p2<<dir;

    if (dir.lengthSquared() < 1e-6) {
        // Avoid drawing zero-length line
        return node;
    }

    dir.normalize();

    float thickness = Thickness();
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


