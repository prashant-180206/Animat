#ifndef CURVE_H
#define CURVE_H

#include "Group.h"
#include "Math/scene.h"
#include <functional>
#include <QRectF>
#include <QPointF>
#include <QString>
#include <QVector>
#include "lib/include/muParser.h"

class Curve : public Group
{
    Q_OBJECT
    QML_ELEMENT

public:
    explicit Curve(Scene *canvas, QQuickItem *parent = nullptr);

    using CurveFunc = std::function<QPointF(double)>;

    void setCurveFunction(const CurveFunc &func);
    CurveFunc curveFunction() const;


protected:
    bool contains(const QPointF &point) const override;
    QRectF boundingRect() const override;

    // Curve::CurveFunc curveFunction() const;
private:
    void buildCurveSegments();
    void updateCurveFunction();

    CurveFunc m_curveFunction;
    int m_segmentCount;
    int segperdis = 20;

    // muParser instances for x and y
    mu::Parser m_parserX;
    mu::Parser m_parserY;

    double m_tVal = 0.0;
    QVector<QPointF> m_cachedPoints;
};

#endif // CURVE_H
