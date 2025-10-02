#ifndef POLYGONPROPERTIES_H
#define POLYGONPROPERTIES_H

#include <QObject>
#include <QQmlEngine>
#include <qcolor.h>
#include <qjsondocument.h>
#include <QJsonObject>

class PolygonProperties : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor NOTIFY borderColorChanged FINAL)
    Q_PROPERTY(qreal thickness READ thickness WRITE setThickness NOTIFY thicknessChanged)

public:
    explicit PolygonProperties(QObject *parent = nullptr);

    QColor borderColor() const { return m_bordercolor; }
    qreal thickness() const { return m_thickness; }

    struct PolygonPropData
    {
        QColor borderColor = Qt::white;
        qreal thickness = 0;
        QJsonDocument toJson() const
        {
            QJsonObject o;
            o["borderColor"] = QJsonObject{{"r", borderColor.red()}, {"g", borderColor.green()}, {"b", borderColor.blue()}, {"a", borderColor.alpha()}};
            o["thickness"] = thickness;
            return QJsonDocument(o);
        }
        static PolygonPropData fromJSON(const QJsonObject &o)
        {
            PolygonPropData d;
            auto c = o["borderColor"].toObject();
            d.borderColor = QColor(c["r"].toInt(), c["g"].toInt(), c["b"].toInt(), c["a"].toInt());
            d.thickness = o["thickness"].toDouble();
            return d;
        }
    };

    PolygonPropData getData() const
    {
        PolygonPropData d;
        d.borderColor = m_bordercolor;
        d.thickness = m_thickness;
        return d;
    }

    void setfromJSON(const QJsonObject &o)
    {
        PolygonPropData d = PolygonPropData::fromJSON(o);
        setBorderColor(d.borderColor);
        setThickness(d.thickness);
    }

public slots:
    void setBorderColor(const QColor &color)
    {
        if (m_bordercolor != color)
        {
            m_bordercolor = color;
            emit borderColorChanged();
        }
    }
    void setThickness(qreal thickness)
    {
        if (!qFuzzyCompare(m_thickness, thickness))
        {
            m_thickness = thickness;
            emit thicknessChanged();
        }
    }
signals:
    void borderColorChanged();
    void thicknessChanged();

private:
    QColor m_bordercolor = Qt::white;
    qreal m_thickness = 0;
};

#endif // POLYGONPROPERTIES_H
