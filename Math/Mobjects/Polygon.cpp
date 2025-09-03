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

    buildPolygon();

    properties->setBorderColor(Qt::yellow);
    properties->setName("Polygon");
    properties->setThickness(4);
    properties->setColor(Qt::blue);
}


void Polygon::buildPolygon()
{
    // Remove old lines
    for (auto line :std::as_const(m_lines)){
        line->setParentItem(nullptr);
        line->deleteLater();
    }
    m_lines.clear();
    removeAllMembers();

    auto pts = points();

    qDebug()<<"Polygon with points "<<pts;
    if (pts.size() < 3)
        return;

    qreal minX = pts[0].x();
    qreal maxX = minX;
    qreal minY = pts[0].y();
    qreal maxY = minY;

    int n = pts.size();
    for (int i = 0; i < n; ++i) {
        const QPointF& p = pts[i];
        if (p.x() < minX) minX = p.x();
        if (p.x() > maxX) maxX = p.x();
        if (p.y() < minY) minY = p.y();
        if (p.y() > maxY) maxY = p.y();

        pts[i]=getcanvas()->p2c(pts[i]);
    }

    // qDebug()<<m_points<<"Calling with Points";

    // Add polygon edges as SimpleLine children
    for (int i = 0; i < n; ++i) {
        auto *line = new SimpleLine(getcanvas(), this);
        line->setP1(pts[i]);
        line->setP2(pts[(i + 1) % n]);
        line->setColor(Qt::yellow);
        m_lines.append(line);
        addMember(line);
    }

    qreal w = maxX - minX;
    qreal h = maxY - minY;

    properties->setSize({h,w});
    update();
}

void Polygon::updateLines()
{
    // qInfo() << "UPDATELINES CALLED";

    auto pts = points();
    QVector<QPointF> convertedPts;
    for (const QPointF &p : std::as_const(pts)) {
        convertedPts.append(getcanvas()->p2c(p));  // convert to canvas coords
    }

    // qInfo() << "m_lines size:" << m_lines.size() << "convertedPts size:" << convertedPts.size();


    // qInfo() << "points converted";

    // Safety check - skip update if mismatch in points and lines count
    if (m_lines.size() != convertedPts.size()) {
        // qWarning() << "updateLines: m_lines size and points size mismatch!"
        //            << "m_lines size:" << m_lines.size()
        //            << "convertedPts size:" << convertedPts.size();
        return;  // Avoid out-of-bounds crash
    }

    for (int i = 0; i < m_lines.size(); ++i) {
        m_lines[i]->setP1(convertedPts[i]);
        m_lines[i]->setP2(convertedPts[(i + 1) % convertedPts.size()]);
    }
    // qInfo() << "UPDATELINES End";
}





QSGNode *Polygon::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{

    auto pts = points();

    for (int i =0;i<pts.size();i++){
        pts[i]=getcanvas()->p2c(pts[i]);
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
        vertices[idx++].set(pts[0].x(), pts[0].y());
        vertices[idx++].set(pts[i].x(), pts[i].y());
        vertices[idx++].set(pts[i + 1].x(), pts[i + 1].y());
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
    auto pts = points();

    if (pts.isEmpty())
        return QRectF();
    qreal minX = pts.first().x();
    qreal maxX = minX;
    qreal minY = pts.first().y();
    qreal maxY = minY;

    for (const QPointF& p : std::as_const(pts)) {
        auto pt = getcanvas()->p2c(p);
        minX = qMin(minX, pt.x());
        maxX = qMax(maxX, pt.x());
        minY = qMin(minY, pt.y());
        maxY = qMax(maxY, pt.y());
    }



    return QRectF(minX, minY, maxX - minX, maxY - minY);
}


