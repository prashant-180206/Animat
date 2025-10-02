#ifndef CURVEPROPERTIES_H
#define CURVEPROPERTIES_H

#include <QObject>
#include <QQmlEngine>
#include <qpoint.h>
#include <qjsondocument.h>
#include <QJsonObject>

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
    {
    }
    QString curveXFunc() const;
    QString curveYFunc() const;
    QPointF tRange() const;
    qreal thickness() const { return m_thickness; }
    int segments();

    struct CurvePropData
    {
        QString curveXFunc = "";
        QString curveYFunc = "";
        QPointF tRange{0, 0};
        int segments = 30;
        qreal thickness = 0;
        QJsonDocument toJson() const
        {
            QJsonObject o;
            o["curveXFunc"] = curveXFunc;
            o["curveYFunc"] = curveYFunc;
            o["tRange"] = QJsonObject{{"x", tRange.x()}, {"y", tRange.y()}};
            o["segments"] = segments;
            o["thickness"] = thickness;
            return QJsonDocument(o);
        }
        static CurvePropData fromJSON(const QJsonObject &o)
        {
            CurvePropData d;
            d.curveXFunc = o["curveXFunc"].toString();
            d.curveYFunc = o["curveYFunc"].toString();
            auto t = o["tRange"].toObject();
            d.tRange = QPointF(t["x"].toDouble(), t["y"].toDouble());
            d.segments = o["segments"].toInt();
            d.thickness = o["thickness"].toDouble();
            return d;
        }
    };

    CurvePropData getData() const
    {
        CurvePropData d;
        d.curveXFunc = m_Curve_X_func;
        d.curveYFunc = m_Curve_Y_func;
        d.tRange = m_tRange;
        d.segments = m_segments;
        d.thickness = m_thickness;
        return d;
    }

    void setfromJSON(const QJsonObject &o)
    {
        CurvePropData d = CurvePropData::fromJSON(o);
        setCurveXFunc(d.curveXFunc);
        setCurveYFunc(d.curveYFunc);
        setTRange(d.tRange);
        setSegments(d.segments);
        setThickness(d.thickness);
    }

public slots:
    void setCurveXFunc(const QString &func);
    void setCurveYFunc(const QString &func);
    void setTRange(const QPointF &range);
    void setSegments(int segments);
    void setThickness(qreal thickness)
    {
        if (!qFuzzyCompare(m_thickness, thickness))
        {
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
    QString m_Curve_X_func = "";
    QString m_Curve_Y_func = "";
    QPointF m_tRange{0, 0};
    int m_segments = 30;
    qreal m_thickness = 0;
};

#endif // CURVEPROPERTIES_H
