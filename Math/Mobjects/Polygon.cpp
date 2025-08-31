#include "Math/Mobjects/Polygon.h"
#include "Math/Mobjects/SimpleLine.h"
#include <QSGFlatColorMaterial>
#include <QSGGeometry>
#include <QSGGeometryNode>

Polygon::Polygon(Scene *canvas, QQuickItem *parent)
    : Group(canvas, parent),m_fillNode(nullptr)
{
    setFlag(ItemHasContents, true);
    setPoints({
        QPointF(0,0),
        QPointF(2,0),
        QPointF(0,3),
    });

    if(m_points.size() <10){
        QVariantList variantList;
        for (const QPointF &point : std::as_const(m_points)) {
            variantList.append(QVariant::fromValue(point));
        }
        properties["Points"] = variantList;
    }

    properties["Border Color"]=borderColor();
    properties["Thickness"]=Thickness();
    properties["Name"]="Polygon";


}

void Polygon::setPoints(const QVector<QPointF> &points)
{
    m_points.clear();
    for (auto p:points){
        m_points.append(getcanvas()->p2c(p));
    }
    buildPolygon();
}

QVector<QPointF> Polygon::points() const
{
    return m_points;
}

void Polygon::buildPolygon()
{
    // Remove old lines
    for (auto child : childItems()) {
        if (SimpleLine *line = qobject_cast<SimpleLine *>(child)) {
            line->setParentItem(nullptr);
            line->deleteLater();
        }
    }

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
    }

    // qDebug()<<m_points;

    // Add polygon edges as SimpleLine children
    for (int i = 0; i < n; ++i) {
        auto *line = new SimpleLine(getcanvas(), this);
        line->setP1(m_points[i]);
        line->setP2(m_points[(i + 1) % n]);
        line->setColor(borderColor());
        addMember(line);
    }

    arrange();

    qreal w = maxX - minX;
    qreal h = maxY - minY;

    setSize(h/getcanvas()->scalefactor(),w/getcanvas()->scalefactor());
    update();
}


QSGNode *Polygon::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
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
    material->setColor(color());

    m_fillNode->markDirty(QSGNode::DirtyGeometry | QSGNode::DirtyMaterial);
    return m_fillNode;
}

bool Polygon::contains(const QPointF &point) const
{

    return boundingRect().contains(point);
}

QRectF Polygon::boundingRect() const
{
    if (m_points.isEmpty())
        return QRectF();
    qreal minX = m_points.first().x();
    qreal maxX = minX;
    qreal minY = m_points.first().y();
    qreal maxY = minY;

    for (const QPointF& p : m_points) {
        minX = qMin(minX, p.x());
        maxX = qMax(maxX, p.x());
        minY = qMin(minY, p.y());
        maxY = qMax(maxY, p.y());
    }
    return QRectF(minX, minY, maxX - minX, maxY - minY);
}
