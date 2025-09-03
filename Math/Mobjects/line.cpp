#include "Math/Mobjects/line.h"


Line::Line(Scene* canvas,QQuickItem* parent) :ClickableMobject(canvas,parent){


    auto p1 =(QPointF(-1,-1));
    auto p2 = (QPointF(1,1));

    setFlag(ItemHasContents, true);
    connect(this, &QQuickItem::windowChanged, this, [this](QQuickWindow* w){
        if(w){
            update();
        }
    });
    properties->setName("Line");
    properties->setLineStart(p1);
    properties->setColor(Qt::yellow);
    properties->setThickness(4);
    properties->setPos(canvas->c2p((p1+p2)/2));

    connect(properties, &MProperties::lineStartChanged ,this, [this]{
        // qDebug() << "[Line] linePointsChanged -> update";
        update();
    });
    connect(properties, &MProperties::lineEndChanged, this, [this]{
        // qDebug() << "[Line] linePointsChanged -> update";
        update();
    });
    connect(properties, &MProperties::thicknessChanged, this, [this]{
        qDebug() << "[Line] thicknessChanged -> update";
        update();
    });

    start_pos=properties->pos();
    m_p1=properties->lineStart();
    m_p2=properties->lineEnd();

}

void Line::setCenter(qreal x, qreal y)
{
    QPointF newCenter(x, y);
    QPointF shift = newCenter - start_pos;
    m_p1 += shift;
    m_p2 += shift;
    start_pos = newCenter;
    QPointF canvasCenter = getcanvas()->p2c(start_pos);
    setX(canvasCenter.x());
    setY(canvasCenter.y());
    setZ(50);

}



void Line::mousePressEvent(QMouseEvent *event)
{
    start_pos = properties->pos();
    // properties->setLineStart(m_p1);
    // properties->setLineEnd(m_p2);
    ClickableMobject::mousePressEvent(event);
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
        material->setColor(properties->color());
        node->setMaterial(material);
        node->setFlag(QSGNode::OwnsMaterial);
    }

    QSGGeometry *geometry = node->geometry();
    geometry->allocate(4);
    QSGGeometry::Point2D *vertices = geometry->vertexDataAsPoint2D();

    QVector2D p1_vec(getcanvas()->p2c(properties->lineStart()));
    QVector2D p2_vec(getcanvas()->p2c(properties->lineEnd()));
    QVector2D dir = p2_vec - p1_vec;

    // qDebug()<<p1_vec<<p2_vec<<dir;

    if (dir.lengthSquared() < 1e-6) {
        // Avoid drawing zero-length line
        return node;
    }

    dir.normalize();

    float thickness = properties->thickness();
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
    // We already stored points in canvas coords, so use them directly.
    QPointF p1 = getcanvas()->c2p(properties->lineStart());
    QPointF p2 = getcanvas()->c2p(properties->lineEnd());

    QRectF rect(p1, p2);
    rect = rect.normalized();

    qreal pad = properties->thickness() / 2.0;
    rect.adjust(-pad, -pad, pad, pad);

    return rect;
}

bool Line::contains(const QPointF &point) const
{
    // point is in the same local coordinate system as boundingRect
    QVector2D p(point);
    QVector2D p1(getcanvas()->p2c(properties->lineStart()));
    QVector2D p2(getcanvas()->p2c(properties->lineEnd()));
    QVector2D v = p2 - p1;
    QVector2D w = p - p1;

    float c1 = QVector2D::dotProduct(w, v);
    if (c1 <= 0)
        return (p - p1).length() <= properties->thickness() / 2.0f;

    float c2 = QVector2D::dotProduct(v, v);
    if (c2 <= c1)
        return (p - p2).length() <= properties->thickness() / 2.0f;

    float b = c1 / c2;
    QVector2D pb = p1 + b * v;
    return (p - pb).length() <= properties->thickness() / 2.0f;
}

