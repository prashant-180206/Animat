#include "Math/Mobjects/Polygon.h"
#include "Math/Mobjects/SimpleLine.h"
#include <QSGFlatColorMaterial>
#include <QSGGeometry>
#include <QSGGeometryNode>

Polygon::Polygon(Scene *canvas, QQuickItem *parent)
    : Group(canvas, parent),m_fillNode(nullptr)
{
    setFlag(ItemHasContents, true);
    properties->setEndPoints({
        QPointF(0,0),
        QPointF(2,0),
        QPointF(0,3),
    });

    if(properties->endPoints().size() <10){
        QList<QPointF> variantList;
        auto m_points = properties->endPoints();
        for (const QPointF &point : std::as_const(m_points)) {
            variantList.append(point);
        }
        properties->setEndPoints(variantList);

    }

    qDebug()<<"End Points in constructor polygon "<<properties->endPoints();

    buildPolygon();

    properties->setBorderColor(Qt::yellow);
    properties->setName("Polygon");
    properties->setThickness(4);
    properties->setColor(Qt::blue);


}


void Polygon::buildPolygon()
{
    // Remove old lines
    auto children =childItems();
    for (auto child : std::as_const(children)) {
        if (SimpleLine *line = qobject_cast<SimpleLine *>(child)) {
            line->setParentItem(nullptr);
            line->deleteLater();
        }
    }

    auto m_points = properties->endPoints();

    qDebug()<<"Polygon with points "<<m_points;
    if (m_points.size() < 3)
        return;

    qreal minX = m_points[0].x();
    qreal maxX = minX;
    qreal minY = m_points[0].y();
    qreal maxY = minY;

    int n = m_points.size();
    for (int i = 0; i < n; ++i) {
        const QPointF& p = m_points[i];
        if (p.x() < minX) minX = p.x();
        if (p.x() > maxX) maxX = p.x();
        if (p.y() < minY) minY = p.y();
        if (p.y() > maxY) maxY = p.y();

        m_points[i]=getcanvas()->p2c(m_points[i]);
    }

    // qDebug()<<m_points<<"Calling with Points";

    // Add polygon edges as SimpleLine children
    for (int i = 0; i < n; ++i) {
        auto *line = new SimpleLine(getcanvas(), this);
        line->setP1(m_points[i]);
        line->setP2(m_points[(i + 1) % n]);
        line->setColor(properties->borderColor());
        addMember(line);
    }

    arrange();

    qreal w = maxX - minX;
    qreal h = maxY - minY;

    setSize(h,w);
    update();
}


QSGNode *Polygon::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{

    auto m_points = properties->endPoints();

    for (int i =0;i<m_points.size();i++){
        m_points[i]=getcanvas()->p2c(m_points[i]);
    }

    if (m_points.size() < 3) {
        delete m_fillNode;
        m_fillNode = nullptr;
        return nullptr;
    }

    if (!m_fillNode) {
        m_fillNode = new QSGGeometryNode();
        QSGGeometry *geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(),(m_points.size() - 2) * 3);

        geometry->setDrawingMode(QSGGeometry::DrawTriangles);
        m_fillNode->setGeometry(geometry);
        m_fillNode->setFlag(QSGNode::OwnsGeometry);

        auto *material = new QSGFlatColorMaterial();
        m_fillNode->setMaterial(material);
        m_fillNode->setFlag(QSGNode::OwnsMaterial);
    }

    // Reallocate geometry for current polygon
    QSGGeometry *geometry = m_fillNode->geometry();
    geometry->allocate((m_points.size() - 2) * 3);
    QSGGeometry::Point2D *vertices = geometry->vertexDataAsPoint2D();

    int idx = 0;
    for (int i = 1; i < m_points.size() - 1; ++i) {
        // Triangle: (0, i, i+1)
        vertices[idx++].set(m_points[0].x(), m_points[0].y());
        vertices[idx++].set(m_points[i].x(), m_points[i].y());
        vertices[idx++].set(m_points[i + 1].x(), m_points[i + 1].y());
    }

    auto material = static_cast<QSGFlatColorMaterial *>(m_fillNode->material());
    material->setColor(properties->color());

    m_fillNode->markDirty(QSGNode::DirtyGeometry | QSGNode::DirtyMaterial);
    return m_fillNode;
}

bool Polygon::contains(const QPointF &point) const
{

    return boundingRect().contains(point);
}

QRectF Polygon::boundingRect() const
{
    auto m_points = properties->endPoints();

    if (m_points.isEmpty())
        return QRectF();
    qreal minX = m_points.first().x();
    qreal maxX = minX;
    qreal minY = m_points.first().y();
    qreal maxY = minY;

    for (const QPointF& p : std::as_const(m_points)) {
        auto pt = getcanvas()->p2c(p);
        minX = qMin(minX, pt.x());
        maxX = qMax(maxX, pt.x());
        minY = qMin(minY, pt.y());
        maxY = qMax(maxY, pt.y());
    }



    return QRectF(minX, minY, maxX - minX, maxY - minY);
}
