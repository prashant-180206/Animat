#pragma once
#include "Math/Helper/mproperties.h"
#include "Mobject.h"
#include <QQuickItem>
#include <QSGSimpleRectNode>
#include <QMouseEvent>
#include <QDebug>
#include <QVariantMap>

class ClickableMobject : public Mobject
{

    Q_OBJECT

public:
    explicit ClickableMobject(Scene *canvas, QQuickItem *parent = nullptr);

    QString getId() const { return m_id; }
    void setId(QString newid);
    Scene *getcanvas() const;

    virtual void setCenter(qreal xval, qreal yval);
    void setSize(qreal height, qreal width);

    Q_INVOKABLE MProperties *getProperties() const;

    QPointF getCenter() const;
    QPointF top() const;
    QPointF bottom() const;
    QPointF left() const;
    QPointF right() const;

    struct MobjData
    {
        QString id;
        MProperties::MPropData properties;

        QJsonDocument toJson() const
        {
            QJsonObject o;
            o["id"] = id;
            o["properties"]= properties.toJson().object();

            return QJsonDocument(o);
        }

        static MobjData fromJSON(const QJsonObject &o, const ClickableMobject *parent = nullptr)
        {
            MobjData d;
            d.id = o["id"].toString();
            if (parent && parent->getProperties() && o.contains("properties"))
                d.properties = MProperties::MPropData::fromJSON(o["properties"].toObject(), parent->getProperties());
            return d;
        }
    };

    MobjData getData() const
    {
        MobjData d;
        d.id = getId();
        if (this->getProperties()){
            qInfo()<<"PROP AVAIL"<<getId();
            d.properties = getProperties()->getData();
        }
        return d;
    }

    void setfromJSON(const QJsonObject &o)
    {
        MobjData d = MobjData::fromJSON(o, this);
        setId(d.id);
        if (properties)
            properties->setfromJSON(d.properties.toJson().object());
    }

    virtual ~ClickableMobject();

private:
    QColor m_color = Qt::blue;

    Scene *m_canvas = nullptr;
    QPointF center = QPointF(0, 0);

    bool m_dragging = false;
    QPointF m_dragItemOffset;
    QPointF m_itemStartPos;
    QPointF m_localpos;

protected:
    QString m_id = "";
    MProperties *properties = new MProperties(this);

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
};
