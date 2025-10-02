#ifndef LINEPROPERTIES_H
#define LINEPROPERTIES_H

#include <QObject>
#include <QQmlEngine>
#include <qjsondocument.h>
#include <QJsonObject>
#include <qpoint.h>

class LineProperties : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QPointF lineStart READ lineStart WRITE setLineStart NOTIFY lineStartChanged)
    Q_PROPERTY(QPointF lineEnd READ lineEnd WRITE setLineEnd NOTIFY lineEndChanged)
    Q_PROPERTY(qreal thickness READ thickness WRITE setThickness NOTIFY thicknessChanged)

public:
    explicit LineProperties(QObject *parent = nullptr);
    QPointF lineStart() const;
    QPointF lineEnd() const;
    qreal thickness() const;

    struct LinePropData
    {
        QPointF lineStart{0, 0};
        QPointF lineEnd{0, 0};
        qreal thickness = 0;
        QJsonDocument toJson() const
        {
            QJsonObject o;
            o["lineStart"] = QJsonObject{{"x", lineStart.x()}, {"y", lineStart.y()}};
            o["lineEnd"] = QJsonObject{{"x", lineEnd.x()}, {"y", lineEnd.y()}};
            o["thickness"] = thickness;
            return QJsonDocument(o);
        }
        static LinePropData fromJSON(const QJsonObject &o)
        {
            LinePropData d;
            auto s = o["lineStart"].toObject();
            d.lineStart = QPointF(s["x"].toDouble(), s["y"].toDouble());
            auto e = o["lineEnd"].toObject();
            d.lineEnd = QPointF(e["x"].toDouble(), e["y"].toDouble());
            d.thickness = o["thickness"].toDouble();
            return d;
        }
    };

    LinePropData getData() const
    {
        LinePropData d;
        d.lineStart = m_lineStart;
        d.lineEnd = m_lineEnd;
        d.thickness = m_thickness;
        return d;
    }

    void setfromJSON(const QJsonObject &o)
    {
        LinePropData d = LinePropData::fromJSON(o);
        setLineStart(d.lineStart);
        setLineEnd(d.lineEnd);
        setThickness(d.thickness);
    }

public slots:
    void setLineStart(const QPointF &p);
    void setLineEnd(const QPointF &p);
    void setThickness(qreal thickness);
signals:
    void lineStartChanged(const QPointF &p);
    void lineEndChanged(const QPointF &p);
    void thicknessChanged();

private:
    QPointF m_lineStart{0, 0};
    QPointF m_lineEnd{0, 0};
    qreal m_thickness = 0;
};

#endif // LINEPROPERTIES_H
