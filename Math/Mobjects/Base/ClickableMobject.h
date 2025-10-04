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

    struct MobjData
    {
        QString id;
        MProperties::MPropData properties;

        QJsonDocument toJson() const;

        static MobjData fromJSON(const QJsonObject &o, const ClickableMobject *parent = nullptr);
    };

    MobjData getData() const;

    void setfromJSON(const QJsonObject &o);

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
