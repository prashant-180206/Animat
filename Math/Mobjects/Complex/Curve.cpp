#include "Curve.h"
#include "Math/Mobjects/Simple/SimpleLine.h"
#include <QVector2D>
#include <QDebug>
#include <QtGlobal>

Curve::Curve(Scene *canvas, QQuickItem *parent)
    : Group(canvas, parent)
{

    properties->setCurve(new CurveProperties(this->properties));

    properties->curve()->setCurveXFunc("t");
    properties->curve()->setCurveYFunc("-0.2*t^2");
    properties->base()->setName("Function");
    properties->curve()->setTRange({-3, 3});
    properties->curve()->setSegments(30);
    properties->curve()->setThickness(4);
    properties->base()->setType("Curve");

    m_parserX.DefineVar(L"t", &m_tVal);
    m_parserY.DefineVar(L"t", &m_tVal);

    m_parserX.SetExpr(properties->curve()->curveXFunc().toStdWString());
    m_parserY.SetExpr(properties->curve()->curveYFunc().toStdWString());

    updateCurveFunction();

    m_segmentCount = 100;
    setFlag(ItemHasContents, true);
    setAcceptedMouseButtons(Qt::AllButtons);

    connect(properties->curve(), &CurveProperties::curveXFuncChanged, this, [this]
            {
                m_parserX.SetExpr(properties->curve()->curveXFunc().toStdWString());
                updateCurveFunction();
                buildCurveSegments(); });

    connect(properties->curve(), &CurveProperties::curveYFuncChanged, this, [this]
            {
                m_parserY.SetExpr(properties->curve()->curveYFunc().toStdWString());
                updateCurveFunction();
                buildCurveSegments(); });

    connect(properties->curve(), &CurveProperties::tRangeChanged, this, [this]
            { buildCurveSegments(); });

    connect(properties->base(), &BaseProperties::colorChanged, this, [this]
            { buildCurveSegments(); });
    connect(properties->curve(), &CurveProperties::thicknessChanged, this, [this]
            { buildCurveSegments(); });

    buildCurveSegments();

    qDebug() << "[Curve::Ctor] done";
}

void Curve::updateCurveFunction()
{
    m_curveFunction = [this](double t) -> QPointF
    {
        m_tVal = t;
        double x = 0.0;
        double y = 0.0;
        try
        {
            x = m_parserX.Eval();
        }
        catch (mu::Parser::exception_type &e)
        {
            qWarning() << "[Curve] muParser Eval X error:" << e.GetMsg().c_str();
            x = t;
        }
        try
        {
            y = m_parserY.Eval();
        }
        catch (mu::Parser::exception_type &e)
        {
            qWarning() << "[Curve] muParser Eval Y error:" << e.GetMsg().c_str();
            y = t;
        }
        return QPointF(x, y);
    };
}

void Curve::setCurveFunction(const CurveFunc &func)
{
    m_curveFunction = func;
    buildCurveSegments();
}

Curve::CurveFunc Curve::curveFunction() const
{
    return m_curveFunction;
}

void Curve::buildCurveSegments()
{

    const auto childrenCopy = childItems();
    int removed = 0;
    for (auto *childObj : childrenCopy)
    {
        if (auto *line = qobject_cast<SimpleLine *>(childObj))
        {
            line->setParentItem(nullptr);
            line->deleteLater();
            ++removed;
        }
    }

    QVector<QPointF> points;
    auto tr = properties->curve()->tRange();
    const double t0 = tr.x();
    const double t1 = tr.y();
    int invalidPts = 0;
    points.reserve(m_segmentCount + 1);
    for (int i = 0; i <= m_segmentCount; ++i)
    {
        const double t = t0 + ((t1 - t0) / static_cast<double>(m_segmentCount)) * i;
        QPointF pt = m_curveFunction(t);
        if (!std::isfinite(pt.x()) || !std::isfinite(pt.y()))
        {
            ++invalidPts;
            continue;
        }
        QPointF cpt = getcanvas()->p2c(pt);
        if (std::isfinite(cpt.x()) && std::isfinite(cpt.y()))
        {
            points.append(cpt);
        }
        else
        {
            ++invalidPts;
        }
    }
    if (points.size() < 2)
    {
        m_cachedPoints.clear();
        update();
        return;
    }
    m_cachedPoints = points;
    int created = 0;
    for (int i = 0; i < m_cachedPoints.size() - 1; ++i)
    {
        auto *segment = new SimpleLine(getcanvas(), this);
        segment->setP1(m_cachedPoints[i]);
        segment->setP2(m_cachedPoints[i + 1]);
        segment->setColor(properties->base()->color());
        segment->setThickness(properties->curve()->thickness());
        addMember(segment);
        ++created;
    }
    update();
}

QRectF Curve::boundingRect() const
{
    if (m_cachedPoints.isEmpty())
    {
        return QRectF();
    }
    constexpr qreal penWidth = 8.0;
    qreal minX = m_cachedPoints[0].x();
    qreal maxX = minX;
    qreal minY = m_cachedPoints[0].y();
    qreal maxY = minY;
    for (const QPointF &pt : m_cachedPoints)
    {
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
    if (m_cachedPoints.size() < 2)
        return false;
    for (int i = 0; i < m_cachedPoints.size() - 1; ++i)
    {
        const QVector2D p1(m_cachedPoints[i]);
        const QVector2D p2(m_cachedPoints[i + 1]);
        const QVector2D v = p2 - p1;
        const QVector2D w = pt - p1;
        const float c1 = QVector2D::dotProduct(w, v);
        if (c1 <= 0)
        {
            if ((pt - p1).length() <= tolerance)
                return true;
            continue;
        }
        const float c2 = QVector2D::dotProduct(v, v);
        if (c2 <= c1)
        {
            if ((pt - p2).length() <= tolerance)
                return true;
            continue;
        }
        const float b = c1 / c2;
        const QVector2D pb = p1 + b * v;
        if ((pt - pb).length() <= tolerance)
            return true;
    }
    return false;
}
