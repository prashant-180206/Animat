#ifndef ANGLEPROPERTIES_H
#define ANGLEPROPERTIES_H

#include <QObject>
#include <QQmlEngine>
#include <QColor>
#include <qjsondocument.h>
#include <QJsonObject>

class AngleProperties : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(qreal angle READ angle WRITE setAngle NOTIFY angleChanged)
    Q_PROPERTY(qreal startVectorLength READ startVectorLength WRITE setStartVectorLength NOTIFY startVectorLengthChanged)
    Q_PROPERTY(qreal endVectorLength READ endVectorLength WRITE setEndVectorLength NOTIFY endVectorLengthChanged)
    Q_PROPERTY(QColor arcColor READ arcColor WRITE setArcColor NOTIFY arcColorChanged)
    Q_PROPERTY(qreal arcRadius READ arcRadius WRITE setArcRadius NOTIFY arcRadiusChanged)
    Q_PROPERTY(qreal arcThickness READ arcThickness WRITE setArcThickness NOTIFY arcThicknessChanged)

public:
    explicit AngleProperties(QObject *parent = nullptr);

    qreal angle() const { return m_angle; }
    qreal startVectorLength() const { return m_startVectorLength; }
    qreal endVectorLength() const { return m_endVectorLength; }
    QColor arcColor() const { return m_arcColor; }
    qreal arcRadius() const { return m_arcRadius; }
    qreal arcThickness() const { return m_arcThickness; }

    struct AnglePropData
    {
        qreal angle = 45.0;
        qreal startVectorLength = 2.0;
        qreal endVectorLength = 2.0;
        QColor arcColor = Qt::green;
        qreal arcRadius = 0.5;
        qreal arcThickness = 3.0;

        QJsonDocument toJson() const
        {
            QJsonObject o;
            o["angle"] = angle;
            o["startVectorLength"] = startVectorLength;
            o["endVectorLength"] = endVectorLength;
            o["arcColor"] = QJsonObject{
                {"r", arcColor.red()},
                {"g", arcColor.green()},
                {"b", arcColor.blue()},
                {"a", arcColor.alpha()}};
            o["arcRadius"] = arcRadius;
            o["arcThickness"] = arcThickness;
            return QJsonDocument(o);
        }

        static AnglePropData fromJSON(const QJsonObject &o)
        {
            AnglePropData d;
            d.angle = o["angle"].toDouble(45.0);
            d.startVectorLength = o["startVectorLength"].toDouble(2.0);
            d.endVectorLength = o["endVectorLength"].toDouble(2.0);
            d.arcRadius = o["arcRadius"].toDouble(0.5);
            d.arcThickness = o["arcThickness"].toDouble(3.0);

            if (o.contains("arcColor"))
            {
                QJsonObject colorObj = o["arcColor"].toObject();
                d.arcColor = QColor(
                    colorObj["r"].toInt(0),
                    colorObj["g"].toInt(255),
                    colorObj["b"].toInt(0),
                    colorObj["a"].toInt(255));
            }
            return d;
        }
    };

    AnglePropData getData() const
    {
        AnglePropData d;
        d.angle = m_angle;
        d.startVectorLength = m_startVectorLength;
        d.endVectorLength = m_endVectorLength;
        d.arcColor = m_arcColor;
        d.arcRadius = m_arcRadius;
        d.arcThickness = m_arcThickness;
        return d;
    }

    void setfromJSON(const QJsonObject &o)
    {
        AnglePropData d = AnglePropData::fromJSON(o);
        setAngle(d.angle);
        setStartVectorLength(d.startVectorLength);
        setEndVectorLength(d.endVectorLength);
        setArcColor(d.arcColor);
        setArcRadius(d.arcRadius);
        setArcThickness(d.arcThickness);
    }

public slots:
    void setAngle(qreal angle);
    void setStartVectorLength(qreal length);
    void setEndVectorLength(qreal length);
    void setArcColor(const QColor &color);
    void setArcRadius(qreal radius);
    void setArcThickness(qreal thickness);

signals:
    void angleChanged();
    void startVectorLengthChanged();
    void endVectorLengthChanged();
    void arcColorChanged();
    void arcRadiusChanged();
    void arcThicknessChanged();

private:
    qreal m_angle = 45.0;
    qreal m_startVectorLength = 2.0;
    qreal m_endVectorLength = 2.0;
    QColor m_arcColor = Qt::green;
    qreal m_arcRadius = 0.5;
    qreal m_arcThickness = 3.0;
};

#endif // ANGLEPROPERTIES_H