// Curve.h
#ifndef CURVE_H
#define CURVE_H

#include "Group.h"
#include "Math/scene.h"
#include <functional>
#include <QRectF>

class Curve : public Group
{
    Q_OBJECT
    QML_ELEMENT

public:
    explicit Curve(Scene *canvas, QQuickItem *parent = nullptr);

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
    void curveClicked();

protected:
    bool contains(const QPointF &point) const override;
    QRectF boundingRect() const override;
    void mousePressEvent(QMouseEvent *event) override;

    // QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *) override;
private:
    CurveFunc m_curveFunction;
    double m_tStart = 0.0;
    double m_tEnd = 1.0;
    int m_segmentCount;
    int segperdis=20;
    Scene *canvas = nullptr;

    QVector<QPointF> m_points;
};

#endif // CURVE_H
