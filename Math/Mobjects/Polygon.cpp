#include "Math/Mobjects/Polygon.h"
#include "Math/Mobjects/SimpleLine.h"
#include <QSGFlatColorMaterial>
#include <QSGGeometry>
#include <QSGGeometryNode>

Polygon::Polygon(Scene *canvas, QQuickItem *parent)
    : Group(canvas, parent), m_fillColor(Qt::red), m_fillNode(nullptr)
{
    setFlag(ItemHasContents, true);
    this->canvas=canvas;
    // setFlag(ItemAcceptsInputMethod, true);
    // setAcceptHoverEvents(true);

}

void Polygon::setPoints(const QVector<QPointF> &points)
{

    // qDebug()<<"Setpoints clled";
    m_points = points;
    buildPolygon();
    // setHeight(200);
    // qDebug()<<height()<<width()<<"Outside func";
}

QVector<QPointF> Polygon::points() const
{
    return m_points;
}

QColor Polygon::fillColor() const
{
    return m_fillColor;
}

void Polygon::setFillColor(const QColor &color)
{
    if (m_fillColor != color) {
        m_fillColor = color;
        emit fillColorChanged();
        update();
    }
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

    // Calculate bounding rect min/max
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

    QPointF offset(minX, minY);

    // Create translated points without modifying m_points
    QVector<QPointF> localPoints;
    localPoints.reserve(n);
    for (const auto &pt : m_points) {
        localPoints.append(pt - offset);
    }

    // Add polygon edges as SimpleLine children
    for (int i = 0; i < n; ++i) {
        auto *line = new SimpleLine(canvas, this);
        line->setP1(localPoints[i]);
        line->setP2(localPoints[(i + 1) % n]);
        addMember(line);
    }

    // Set geometry based on bounding rect size
    arrange();
    // Set geometry based on bounding rect size
    setX(offset.x());
    setY(offset.y());

    qreal w = maxX - minX;
    qreal h = maxY - minY;

    // qDebug() << w << h << "Calculated";

    setWidth(w);
    setHeight(h);

    // qDebug() << implicitWidth() << implicitHeight();


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
        QSGGeometry *geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(),
                                                (m_points.size() - 2) * 3); // number of triangles
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
    material->setColor(m_fillColor);

    m_fillNode->markDirty(QSGNode::DirtyGeometry | QSGNode::DirtyMaterial);
    return m_fillNode;
}

bool Polygon::contains(const QPointF &point) const
{
    // Accept events everywhere in bounding box (or improve with point-in-polygon)
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
