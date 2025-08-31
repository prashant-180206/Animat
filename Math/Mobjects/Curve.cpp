#include "Curve.h"
#include "Math/Mobjects/SimpleLine.h"
#include <QVector2D>
#include <QDebug>

Curve::Curve(Scene *canvas, QQuickItem *parent)
    : Group(canvas, parent), canvas(canvas)
{
    // Initialize muParser variables
    m_parserX.DefineVar(L"t", &m_tVal);
    m_parserY.DefineVar(L"t", &m_tVal);

    m_parserX.SetExpr(Xfunc().toStdWString());
    m_parserY.SetExpr(Yfunc().toStdWString());

    // Set default formulas
    setXfunc("t");
    setYfunc("0.2*t^2");
    properties.remove("Height");
    properties.remove("Width");
    properties.remove("x");
    properties.remove("y");
    properties["Name"]= "Function";
    properties["X Func"] = Xfunc();
    properties["Y Func"]= Yfunc();
    properties["T Range"]= TRange();

    m_segmentCount = segperdis * (m_tRange.y() - m_tRange.x());
    setFlag(ItemHasContents, true);
    setAcceptedMouseButtons(Qt::AllButtons);

    buildCurveSegments();
}

void Curve::setXfunc(const QString &func)
{
    if (m_xfunc != func) {
        m_xfunc = func;
        try {
            m_parserX.SetExpr(func.toStdWString());
            updateCurveFunction();
            buildCurveSegments();
            emit XfuncChanged();
        } catch (mu::Parser::exception_type &e) {
            qWarning() << "muParser Xfunc error:" << e.GetMsg().c_str();
        }
    }
}

QString Curve::Yfunc() const { return m_yfunc; }

void Curve::setYfunc(const QString &func)
{
    if (m_yfunc != func) {
        m_yfunc = func;
        try {
            m_parserY.SetExpr(func.toStdWString());
            updateCurveFunction();
            buildCurveSegments();
            emit YfuncChanged();
        } catch (mu::Parser::exception_type &e) {
            qWarning() << "muParser Yfunc error:" << e.GetMsg().c_str();
        }
    }
}

QPointF Curve::TRange() const { return m_tRange; }


void Curve::setTRange(const QPointF &range)
{
    if (m_tRange != range) {
        m_tRange = range;
        m_segmentCount = segperdis * std::abs(m_tRange.y() - m_tRange.x());
        updateCurveFunction();
        buildCurveSegments();
        emit TRangeChanged();
    }
}

int Curve::Segments(){return segperdis;}

void Curve::setSegments(int seg){
    segperdis = seg;
    emit SegmentsChanged();
}

void Curve::updateCurveFunction()
{
    m_curveFunction = [this](double t) -> QPointF {
        m_tVal = t;
        double x = 0.0;
        double y = 0.0;

        try {
            x = m_parserX.Eval();
        } catch (mu::Parser::exception_type &e) {
            qWarning() << "muParser Eval X error:" << e.GetMsg().c_str();
        }

        try {
            y = m_parserY.Eval();
        } catch (mu::Parser::exception_type &e) {
            qWarning() << "muParser Eval Y error:" << e.GetMsg().c_str();
        }

        return QPointF(x, y);
    };
}

void Curve::setCurveFunction(const CurveFunc &func)
{
    m_curveFunction = func;
    buildCurveSegments();
}

QString Curve::Xfunc() const { return m_xfunc; }

Curve::CurveFunc Curve::curveFunction() const
{
    return m_curveFunction;
}

void Curve::buildCurveSegments()
{
    for (auto childObj : childItems()) {
        if (SimpleLine *line = qobject_cast<SimpleLine *>(childObj)) {
            line->setParentItem(nullptr);
            line->deleteLater();
        }
    }

    m_points.clear();

    for (int i = 0; i <= m_segmentCount; ++i) {
        double t = m_tRange.x() + (m_tRange.y() - m_tRange.x()) * i / m_segmentCount;
        QPointF pt = m_curveFunction(t);
        pt = getcanvas()->p2c(pt);
        m_points.append(pt);
    }

    for (int i = 0; i < m_points.size() - 1; ++i) {
        auto *segment = new SimpleLine(canvas, this);
        segment->setP1(m_points[i]);
        segment->setP2(m_points[i + 1]);
        addMember(segment);
    }

    arrange();
    update();
}

QRectF Curve::boundingRect() const
{
    if (m_points.isEmpty())
        return QRectF();

    qreal penWidth = 8.0;
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
