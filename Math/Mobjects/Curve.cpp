#include "Curve.h"
#include "Math/Mobjects/line.h"


Curve::Curve(Scene *canvas, QQuickItem *parent)
    : Group(canvas, parent)
{
    // Default parabola: y = x^2 parametric
    m_curveFunction = [](double t) {
        return QPointF(t, t * t);
    };

    m_tStart = -1.0;
    m_tEnd = 1.0;
    m_segmentCount = 100;

    setFlag(ItemHasContents, true);

    buildCurveSegments();
    this->canvas = canvas;
}

void Curve::setCurveFunction(const CurveFunc &func)
{
    // You may want a better comparison but this works for demos
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
        if (Line *line = qobject_cast<Line *>(childObj)) {
            line->setParentItem(nullptr);
            line->deleteLater();
        }
    }

    // QList<Mobject *> lines;

    QVector<QPointF> points;
    for (int i = 0; i <= m_segmentCount; ++i) {
        double t = m_tStart + (m_tEnd - m_tStart) * i / m_segmentCount;
        QPointF pt =m_curveFunction(t);
      /*  if (canvas)*//* QPointF*/ pt =mapToItem(canvas,pt);
        points.append(pt);
    }

    for (int i = 0; i < points.size() - 1; ++i) {
        auto *segment = new Line(canvas, this);
        segment->setP1(points[i]);
        segment->setP2(points[i + 1]);
        addMember(segment);
    }

    arrange();
}
