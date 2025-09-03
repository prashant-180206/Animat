#include "Curve.h"
#include "Math/Mobjects/SimpleLine.h"
#include <QVector2D>
#include <QDebug>
#include <QtGlobal>



Curve::Curve(Scene *canvas, QQuickItem *parent)
    : Group(canvas, parent)
{
    // qDebug() << "[Curve::Ctor] start";

    // 1) Set defaults on properties **before** wiring expressions.
    properties->setCurveXFunc("t");
    properties->setCurveYFunc("-0.2*t^2");
    properties->setName("Function");
    properties->setTRange({-3, 3});
    properties->setSegments(30);

    // 2) Initialize muParser and bind 't'
    m_parserX.DefineVar(L"t", &m_tVal);
    m_parserY.DefineVar(L"t", &m_tVal);

    // Set the expressions from properties (now guaranteed to be non-empty)
    m_parserX.SetExpr(properties->curveXFunc().toStdWString());
    m_parserY.SetExpr(properties->curveYFunc().toStdWString());

    // 3) Initialize curve function BEFORE generating segments
    updateCurveFunction();

    // Basic visual/QQuickItem setup
    m_segmentCount = 100;
    setFlag(ItemHasContents, true);
    setAcceptedMouseButtons(Qt::AllButtons);

    // 4) Listen for changes to re-build safely
    connect(properties, &MProperties::curveXFuncChanged, this, [this]{
        qDebug() << "[Curve] curveXFuncChanged -> rebuild";
        m_parserX.SetExpr(properties->curveXFunc().toStdWString());
        updateCurveFunction();
        buildCurveSegments();
    });
    connect(properties, &MProperties::curveYFuncChanged, this, [this]{
        qDebug() << "[Curve] curveYFuncChanged -> rebuild";
        m_parserY.SetExpr(properties->curveYFunc().toStdWString());
        updateCurveFunction();
        buildCurveSegments();
    });
    connect(properties, &MProperties::tRangeChanged, this, [this]{
        qDebug() << "[Curve] tRangeChanged -> rebuild";
        buildCurveSegments();
    });

    // 5) Build
    buildCurveSegments();

    qDebug() << "[Curve::Ctor] done";
}

void Curve::updateCurveFunction()
{
    // qDebug() << "[Curve::updateCurveFunction] updating functor";
    m_curveFunction = [this](double t) -> QPointF {
        m_tVal = t;
        double x = 0.0;
        double y = 0.0;
        try {
            x = m_parserX.Eval();
        } catch (mu::Parser::exception_type &e) {
            qWarning() << "[Curve] muParser Eval X error:" << e.GetMsg().c_str();
            x = std::numeric_limits<double>::quiet_NaN();
        }
        try {
            y = m_parserY.Eval();
        } catch (mu::Parser::exception_type &e) {
            qWarning() << "[Curve] muParser Eval Y error:" << e.GetMsg().c_str();
            y = std::numeric_limits<double>::quiet_NaN();
        }
        return QPointF(x, y);
    };
}

void Curve::setCurveFunction(const CurveFunc &func)
{
    qDebug() << "[Curve::setCurveFunction] external functor set";
    m_curveFunction = func;
    buildCurveSegments();
}

Curve::CurveFunc Curve::curveFunction() const
{
    return m_curveFunction;
}

void Curve::buildCurveSegments()
{
    {
        const auto childrenCopy = childItems();
        int removed = 0;
        for (auto *childObj : childrenCopy) {
            if (auto *line = qobject_cast<SimpleLine *>(childObj)) {
                line->setParentItem(nullptr);
                line->deleteLater();
                ++removed;
            }
        }
        qDebug() << "[Curve] removed old SimpleLine segments:" << removed;
    }

    QVector<QPointF> points;
    auto tr = properties->tRange();
    const double t0 = tr.x();
    const double t1 = tr.y();

    qDebug().nospace() << "[Curve] T-range = [" << t0 << ", " << t1
                       << "], segments=" << m_segmentCount;

    int invalidPts = 0;
    points.reserve(m_segmentCount + 1);
    for (int i = 0; i <= m_segmentCount; ++i) {
        const double t = t0 + ((t1 - t0) / static_cast<double>(m_segmentCount)) * i;

        QPointF pt = m_curveFunction(t);

        // Detect NaN/Inf before projection
        if (!std::isfinite(pt.x()) || !std::isfinite(pt.y())) {
            ++invalidPts;
            continue;
        }

        // Project to canvas coords
        QPointF cpt = getcanvas()->p2c(pt);

        if (std::isfinite(cpt.x()) && std::isfinite(cpt.y())) {
            points.append(cpt);
        } else {
            ++invalidPts;
        }
    }

    // Avoid storing an empty list that would break downstream code
    if (points.size() < 2) {
        properties->setEndPoints(points); // still store for transparency
        update(); // ensure a repaint clears old
        return;
    }

    // Store points into properties
    properties->setEndPoints(points);
    // logPointList(points);

    // Build segments
    int created = 0;
    const auto m_points = properties->endPoints();
    for (int i = 0; i < m_points.size() - 1; ++i) {
        auto *segment = new SimpleLine(getcanvas(), this);
        segment->setP1(m_points[i]);
        segment->setP2(m_points[i + 1]);
        addMember(segment);
        ++created;
    }
    update();
}

QRectF Curve::boundingRect() const
{
    const auto m_points = properties->endPoints();
    if (m_points.isEmpty()) {
        return QRectF();
    }

    constexpr qreal penWidth = 8.0;
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

    const QRectF r(minX - penWidth, minY - penWidth,
                   (maxX - minX) + 2 * penWidth,
                   (maxY - minY) + 2 * penWidth);

    return r;
}

bool Curve::contains(const QPointF &point) const
{
    constexpr qreal tolerance = 8.0;
    const QVector2D pt(point);
    const auto m_points = properties->endPoints();

    if (m_points.size() < 2)
        return false;

    for (int i = 0; i < m_points.size() - 1; ++i) {
        const QVector2D p1(m_points[i]);
        const QVector2D p2(m_points[i + 1]);
        const QVector2D v = p2 - p1;
        const QVector2D w = pt - p1;

        const float c1 = QVector2D::dotProduct(w, v);
        if (c1 <= 0) {
            if ((pt - p1).length() <= tolerance) return true;
            continue;
        }

        const float c2 = QVector2D::dotProduct(v, v);
        if (c2 <= c1) {
            if ((pt - p2).length() <= tolerance) return true;
            continue;
        }

        const float b = c1 / c2;
        const QVector2D pb = p1 + b * v;

        if ((pt - pb).length() <= tolerance) return true;
    }

    return false;
}
