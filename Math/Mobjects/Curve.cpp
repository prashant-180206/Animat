// Curve.cpp
#include "Curve.h"
#include "Math/Mobjects/SimpleLine.h"
#include <QVector2D>
#include <QMouseEvent>
#include <QDebug>

Curve::Curve(Scene *canvas, QQuickItem *parent)
    : Group(canvas, parent), canvas(canvas)
{
    // Default parabola: y = x^2 parametric
    m_curveFunction = [](double t) {
        return QPointF(t, 0.2*t * t);
    };
    setParameterRange(-2,1);
    m_segmentCount = segperdis*6;
    setFlag(ItemHasContents, true);
    setAcceptedMouseButtons(Qt::AllButtons);
    buildCurveSegments();
}

void Curve::setCurveFunction(const CurveFunc &func)
{
    m_curveFunction = func;
    emit curveFunctionChanged();
    buildCurveSegments();
}

Curve::CurveFunc Curve::curveFunction() const
{
    return m_curveFunction;
}

void Curve::setParameterRange(double tStart, double tEnd)
{
    if (m_tStart != tStart || m_tEnd != tEnd) {
        m_tStart = tStart;
        m_tEnd = tEnd;
        emit parameterRangeChanged();
        buildCurveSegments();
    }

    double d =abs(tEnd-tStart);
    setSegmentCount(floor(segperdis*d));

}

void Curve::setSegmentCount(int count)
{
    if (m_segmentCount != count && count > 1) {
        m_segmentCount = count;
        emit segmentCountChanged();
        buildCurveSegments();
    }
}

void Curve::buildCurveSegments()
{
    // Remove old line segments
    for (auto childObj : childItems()) {
        if (SimpleLine *line = qobject_cast<SimpleLine *>(childObj)) {
            line->setParentItem(nullptr);
            line->deleteLater();
        }
    }

    m_points.clear();
    for (int i = 0; i <= m_segmentCount; ++i) {
        double t = m_tStart + (m_tEnd - m_tStart) * i / m_segmentCount;
        QPointF pt = m_curveFunction(t);
        pt = getcanvas()->p2c(pt);
        m_points.append(pt);
    }

    for (int i = 0; i < m_points.size() - 1; ++i) {
        auto *segment = new SimpleLine(canvas, this);
        segment->setP1(m_points[i]);
        segment->setP2(m_points[i + 1]);
        // segment->set(5);
        addMember(segment);
    }

    arrange();
    update();
}

QRectF Curve::boundingRect() const
{
    if (m_points.isEmpty())
        return QRectF();

    qreal penWidth = 8.0; // tolerance for clicking
    qreal minX = m_points[0].x();
    qreal maxX = minX;
    qreal minY = m_points[0].y();
    qreal maxY = minY;

    for (const QPointF &pt : m_points) {
        minX = qMin(minX, pt.x());
        maxX = qMax(maxX, pt.x());
        minY = qMin(minY, pt.y());
        maxY = qMax(maxY, pt.y());
    }

    return QRectF(minX - penWidth, minY - penWidth, maxX - minX + 2 * penWidth, maxY - minY + 2 * penWidth);
}

bool Curve::contains(const QPointF &point) const
{
    constexpr qreal tolerance = 8.0;
    QVector2D pt(point);

    for (int i = 0; i < m_points.size() - 1; ++i) {
        QVector2D p1(m_points[i]);
        QVector2D p2(m_points[i + 1]);
        QVector2D v = p2 - p1;
        QVector2D w = pt - p1;

        float c1 = QVector2D::dotProduct(w, v);
        if (c1 <= 0) {
            if ((pt - p1).length() <= tolerance) return true;
            continue;
        }
        float c2 = QVector2D::dotProduct(v, v);
        if (c2 <= c1) {
            if ((pt - p2).length() <= tolerance) return true;
            continue;
        }
        float b = c1 / c2;
        QVector2D pb = p1 + b * v;
        if ((pt - pb).length() <= tolerance) return true;
    }
    return false;
}

void Curve::mousePressEvent(QMouseEvent *event)
{
    if (contains(event->pos())) {
        qDebug() << "Curve clicked at" << event->pos();
        emit curveClicked();
        event->accept();
    } else {
        event->ignore();
    }
    getcanvas()->setActiveMobjectId(m_id);
}
