#ifndef CURVE_H
#define CURVE_H


#include "Group.h"
#include "Math/scene.h"
#include <functional>


class Curve : public Group
{
    Q_OBJECT
    QML_ELEMENT

public:
    explicit Curve(Scene *canvas = nullptr, QQuickItem *parent = nullptr);

    using CurveFunc = std::function<QPointF(double)>;

    void setCurveFunction(const CurveFunc &func);
    CurveFunc curveFunction() const;

    void setParameterRange(double tStart, double tEnd);
    void setSegmentCount(int count);

    Q_INVOKABLE void buildCurveSegments();

signals:
    void curveFunctionChanged();
    void parameterRangeChanged();
    void segmentCountChanged();

private:
    CurveFunc m_curveFunction;
    double m_tStart = 0.0;
    double m_tEnd = 1.0;
    int m_segmentCount = 100;
    Scene*canvas;
};


#endif // CURVE_H
