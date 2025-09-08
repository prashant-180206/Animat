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
    Q_PROPERTY(QPointF size READ size WRITE setSize NOTIFY sizeChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(QString type READ type WRITE setType NOTIFY typeChanged)


    // Lines
    Q_PROPERTY(QPointF lineStart READ lineStart WRITE setLineStart NOTIFY lineStartChanged)
    Q_PROPERTY(QPointF lineEnd READ lineEnd WRITE setLineEnd NOTIFY lineEndChanged)
    Q_PROPERTY(qreal thickness READ thickness WRITE setThickness NOTIFY thicknessChanged)

    // Curves
    Q_PROPERTY(QString curveXFunc READ curveXFunc WRITE setCurveXFunc NOTIFY curveXFuncChanged)
    Q_PROPERTY(QString curveYFunc READ curveYFunc WRITE setCurveYFunc NOTIFY curveYFuncChanged)
    Q_PROPERTY(QPointF tRange READ tRange WRITE setTRange NOTIFY tRangeChanged)
    Q_PROPERTY(int segments READ segments WRITE setSegments NOTIFY segmentsChanged FINAL)

    // Polygon
    Q_PROPERTY(QList<QPointF> endPoints READ endPoints WRITE setEndPoints NOTIFY endPointsChanged)
    Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor NOTIFY borderColorChanged FINAL)

    // Circle
    Q_PROPERTY(qreal radius READ radius WRITE setRadius NOTIFY radiusChanged)

public:

    MProperties(QObject *parent = nullptr);

    // Getters
    QString name() const;
    QPointF pos() const;
    QPointF size() const;
    QColor color() const;
    QString type(){return m_type;};
    QColor borderColor() const ;
    QPointF lineStart() const{return m_lineStart;};
    QPointF lineEnd() const{return m_lineEnd;};
    qreal thickness() const;
    QString curveXFunc() const;
    QString curveYFunc() const;
    QPointF tRange() const { return m_tRange;};
    int segments(){return m_segments;};
    QList<QPointF> endPoints() const;
    qreal radius() const;

public slots:

    void setName(const QString &name);
    void setPos(const QPointF &pos);
    void setSize(const QPointF &size);
    void setColor(const QColor &color);
    void setType(const QString &q);;
    void setBorderColor(const QColor &color);
    void setLineStart(const QPointF &p);
    void setLineEnd(const QPointF &p);
    void setThickness(qreal thickness);
    void setCurveXFunc(const QString &func);
    void setCurveYFunc(const QString &func);
    Q_INVOKABLE void setTRange(const QPointF &range);
    void setSegments(int segments);
    void setEndPoints(const QList<QPointF> &points);
    void setRadius(qreal radius);

signals:

    void nameChanged();
    void posChanged(const QPointF& p);
    void sizeChanged(const QPointF& p);
    void colorChanged();
    void typeChanged();
    void lineStartChanged(const QPointF &p);
    void lineEndChanged(const QPointF &p);
    void thicknessChanged();
    void curveXFuncChanged();
    void curveYFuncChanged();
    void tRangeChanged();
    void segmentsChanged();
    void endPointsChanged();
    void radiusChanged();
    void borderColorChanged();

private:
    QString m_Name = "Mobject";
    QPointF m_pos{0,0};
    QPointF m_size{0,0};
    QColor m_color = Qt::transparent;
    QString m_type = "Mobject";
    QColor m_bordercolor = Qt::white;
    QPointF m_lineStart{0,0};
    QPointF m_lineEnd{0,0};
    qreal m_thickness = 0;
    QString m_Curve_X_func="";
    QString m_Curve_Y_func ="";
    QPointF m_tRange{0,0};
    QList<QPointF> m_endPoints = {};
    qreal m_radius = 0;
    int m_segments=30;
};

#endif // MPROPERTIES_H
