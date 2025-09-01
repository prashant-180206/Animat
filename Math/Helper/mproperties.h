#ifndef MPROPERTIES_H
#define MPROPERTIES_H

#include <QObject>
#include <QColor>
#include <QPointF>
#include <QQmlListProperty>
#include <QPair>
#include <qqmlintegration.h>

class MProperties : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    // Basic Mobject
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QPointF pos READ pos WRITE setPos NOTIFY posChanged)
    Q_PROPERTY(QPair<qreal, qreal> size READ size WRITE setSize NOTIFY sizeChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

    // Lines
    Q_PROPERTY(QPair<QPointF, QPointF> linePoints READ linePoints WRITE setLinePoints NOTIFY linePointsChanged)
    Q_PROPERTY(qreal thickness READ thickness WRITE setThickness NOTIFY thicknessChanged)

    // Curves
    Q_PROPERTY(QString curveXFunc READ curveXFunc WRITE setCurveXFunc NOTIFY curveXFuncChanged)
    Q_PROPERTY(QString curveYFunc READ curveYFunc WRITE setCurveYFunc NOTIFY curveYFuncChanged)
    Q_PROPERTY(QPair<qreal, qreal> tRange READ tRange WRITE setTRange NOTIFY tRangeChanged)

    // Polygon
    Q_PROPERTY(QList<QPointF> endPoints READ endPoints WRITE setEndPoints NOTIFY endPointsChanged)

    // Circle
    Q_PROPERTY(qreal radius READ radius WRITE setRadius NOTIFY radiusChanged)

public:
    explicit MProperties(QObject *parent = nullptr);

    // Getters
    QString name() const;
    QPointF pos() const;
    QPair<qreal, qreal> size() const;
    QColor color() const;
    QPair<QPointF, QPointF> linePoints() const;
    qreal thickness() const;
    QString curveXFunc() const;
    QString curveYFunc() const;
    QPair<qreal, qreal> tRange() const;
    QList<QPointF> endPoints() const;
    qreal radius() const;

// public slots:

    void setName(const QString &name);
    void setPos(const QPointF &pos);
    void setSize(const QPair<qreal, qreal> &size);
    void setColor(const QColor &color);
    void setLinePoints(const QPair<QPointF, QPointF> &points);
    void setThickness(qreal thickness);
    void setCurveXFunc(const QString &func);
    void setCurveYFunc(const QString &func);
    void setTRange(const QPair<qreal, qreal> &range);
    void setEndPoints(const QList<QPointF> &points);
    void setRadius(qreal radius);

signals:
    void nameChanged();
    void posChanged();
    void sizeChanged();
    void colorChanged();
    void linePointsChanged();
    void thicknessChanged();
    void curveXFuncChanged();
    void curveYFuncChanged();
    void tRangeChanged();
    void endPointsChanged();
    void radiusChanged();

private:
    QString m_Name = "Mobject";
    QPointF m_pos{0,0};
    QPair<qreal, qreal> m_size{0,0};
    QColor m_color = Qt::transparent;
    QPair<QPointF, QPointF> m_linepoints{{0,0},{0,0}};
    qreal m_thickness = 0;
    QString m_Curve_X_func = "";
    QString m_Curve_Y_func = "";
    QPair<qreal, qreal> m_tRange{0,0};
    QList<QPointF> m_endPoints = {};
    qreal m_radius = 0;
};

#endif // MPROPERTIES_H
