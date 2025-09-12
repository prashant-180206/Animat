#ifndef CURVEPROPERTIES_H
#define CURVEPROPERTIES_H

#include <QObject>
#include <QQmlEngine>
#include <qpoint.h>

class CurveProperties : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString curveXFunc READ curveXFunc WRITE setCurveXFunc NOTIFY curveXFuncChanged)
    Q_PROPERTY(QString curveYFunc READ curveYFunc WRITE setCurveYFunc NOTIFY curveYFuncChanged)
    Q_PROPERTY(QPointF tRange READ tRange WRITE setTRange NOTIFY tRangeChanged)
    Q_PROPERTY(int segments READ segments WRITE setSegments NOTIFY segmentsChanged FINAL)
    Q_PROPERTY(qreal thickness READ thickness WRITE setThickness NOTIFY thicknessChanged)

public:
    explicit CurveProperties(QObject *parent = nullptr)
        : QObject{parent}
    {}
    QString curveXFunc() const;
    QString curveYFunc() const;
    QPointF tRange() const;;
    qreal thickness() const { return m_thickness; }
    int segments();;


public slots:
    void setCurveXFunc(const QString &func);
    void setCurveYFunc(const QString &func);
    void setTRange(const QPointF &range);
    void setSegments(int segments);
    void setThickness(qreal thickness) {
        if (!qFuzzyCompare(m_thickness, thickness)) {
            m_thickness = thickness;
            emit thicknessChanged();
        }
    }

signals:
    void curveXFuncChanged();
    void curveYFuncChanged();
    void tRangeChanged();
    void segmentsChanged();
    void thicknessChanged();

private:
    QString m_Curve_X_func="";
    QString m_Curve_Y_func ="";
    QPointF m_tRange{0,0};
    int m_segments=30;
    qreal m_thickness = 0;
};

#endif // CURVEPROPERTIES_H
