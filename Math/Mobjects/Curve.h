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
    Q_PROPERTY(QString Xfunc READ Xfunc WRITE setXfunc NOTIFY XfuncChanged FINAL)
    Q_PROPERTY(QString Yfunc READ Yfunc WRITE setYfunc NOTIFY YfuncChanged FINAL)
    Q_PROPERTY(QPointF TRange READ TRange WRITE setTRange NOTIFY TRangeChanged FINAL)
    Q_PROPERTY(int Segments READ Segments WRITE setSegments NOTIFY SegmentsChanged FINAL)
public:
    explicit Curve(Scene *canvas, QQuickItem *parent = nullptr);

    using CurveFunc = std::function<QPointF(double)>;

    void setCurveFunction(const CurveFunc &func);
    CurveFunc curveFunction() const;

    QString Xfunc() const;
    void setXfunc(const QString &func);

    QString Yfunc() const;
    void setYfunc(const QString &func);

    QPointF TRange() const;
    void setTRange(const QPointF &range);

    int Segments();;
    void setSegments(int seg);

signals:
    void XfuncChanged();
    void YfuncChanged();
    void TRangeChanged();
    void SegmentsChanged();

protected:
    bool contains(const QPointF &point) const override;
    QRectF boundingRect() const override;

private:
    void buildCurveSegments();
    void updateCurveFunction();

    CurveFunc m_curveFunction;

    QString m_xfunc = "t";
    QString m_yfunc = "-t^2";
    QPointF m_tRange = QPointF(-2, 2);

    int m_segmentCount = 100;
    int segperdis = 20;

    Scene *canvas = nullptr;
    QVector<QPointF> m_points;

    // muParser instances for x and y
    mu::Parser m_parserX;
    mu::Parser m_parserY;

    double m_tVal = 0.0;
};

#endif // CURVE_H
