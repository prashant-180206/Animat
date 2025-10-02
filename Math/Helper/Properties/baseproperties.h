#ifndef BASEPROPERTIES_H
#define BASEPROPERTIES_H

#include <QObject>
#include <QQmlEngine>
#include <qcolor.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qpoint.h>

class BaseProperties : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    // Basic Mobject
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QPointF pos READ pos WRITE setPos NOTIFY posChanged)
    Q_PROPERTY(QPointF size READ size WRITE setSize NOTIFY sizeChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(QString type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity NOTIFY opacityChanged FINAL)
    Q_PROPERTY(qreal scale READ scale WRITE setScale NOTIFY scaleChanged FINAL)
    Q_PROPERTY(qreal rotation READ rotation WRITE setRotation NOTIFY rotationChanged FINAL)
    Q_PROPERTY(qreal zindex READ zindex WRITE setZindex NOTIFY zindexChanged FINAL)
public:
    explicit BaseProperties(QObject *parent = nullptr);
    QString name() const;
    QPointF pos() const;
    QPointF size() const;
    QColor color() const;
    qreal opacity() const;
    QString type();
    qreal scale() const;
    qreal rotation() const;
    qreal zindex() const;

    struct basePropData
    {
        QString name = "Mobject";
        QPointF pos{0, 0};
        QPointF size{0, 0};
        QColor color = Qt::transparent;
        qreal opacity = 0.5;
        QString type = "Mobject";
        qreal scale = 1.0;
        qreal rotation = 0.0;
        qreal zindex = 0.0;
        QJsonDocument toJson() const
        {
            QJsonObject o;
            o["name"] = name;
            o["pos"] = QJsonObject{{"x", pos.x()}, {"y", pos.y()}};
            o["size"] = QJsonObject{{"width", size.x()}, {"height", size.y()}};
            o["color"] = QJsonObject{
                {"r", color.red()},
                {"g", color.green()},
                {"b", color.blue()},
                {"a", color.alpha()}};
            o["opacity"] = opacity;
            o["type"] = type;
            o["scale"] = scale;
            o["rotation"] = rotation;
            o["zindex"] = zindex;
            return QJsonDocument(o);
        }
        static basePropData fromJSON(const QJsonObject &o)
        {
            basePropData d;
            d.name = o["name"].toString();
            auto p = o["pos"].toObject();
            d.pos = QPointF(p["x"].toDouble(), p["y"].toDouble());
            auto s = o["size"].toObject();
            d.size = QPointF(s["width"].toDouble(), s["height"].toDouble());
            auto c = o["color"].toObject();
            d.color = QColor(c["r"].toInt(), c["g"].toInt(), c["b"].toInt(), c["a"].toInt());
            d.opacity = o["opacity"].toDouble();
            d.type = o["type"].toString();
            d.scale = o["scale"].toDouble();
            d.rotation = o["rotation"].toDouble();
            d.zindex = o["zindex"].toDouble();
            return d;
        }
    };

    basePropData getData() const
    {
        basePropData d;
        d.name = m_Name;
        d.pos = m_pos;
        d.size = m_size;
        d.color = m_color;
        d.opacity = m_opacity;
        d.type = m_type;
        d.scale = m_scale;
        d.rotation = m_rotation;
        d.zindex = m_zindex;
        return d;
    }

    void setfromJSON(const QJsonObject &o)
    {
        basePropData d = basePropData::fromJSON(o);
        setName(d.name);
        setPos(d.pos);
        setSize(d.size);
        setColor(d.color);
        setOpacity(d.opacity);
        setType(d.type);
        setScale(d.scale);
        setRotation(d.rotation);
        setZindex(d.zindex);
    }

public slots:
    void setName(const QString &name);
    void setPos(const QPointF &pos);
    void setSize(const QPointF &size);
    void setColor(const QColor &color);
    void setOpacity(qreal op);
    void setType(const QString &q);
    ;
    void setScale(qreal scale);
    void setRotation(qreal rotation);
    void setZindex(qreal zindex);

signals:

    void nameChanged();
    void posChanged(const QPointF &p);
    void sizeChanged(const QPointF &p);
    void colorChanged();
    void opacityChanged(qreal op);
    void typeChanged();
    void scaleChanged(qreal scale);
    void rotationChanged(qreal rotation);
    void zindexChanged(qreal zindex);

private:
    QString m_Name = "Mobject";
    QPointF m_pos{0, 0};
    QPointF m_size{0, 0};
    QColor m_color = Qt::transparent;
    qreal m_opacity = 0.5;
    QString m_type = "Mobject";
    qreal m_scale = 1.0;
    qreal m_rotation = 0.0;
    qreal m_zindex = 0.0;
};

#endif // BASEPROPERTIES_H
