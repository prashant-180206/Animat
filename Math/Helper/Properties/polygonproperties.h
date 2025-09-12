#ifndef POLYGONPROPERTIES_H
#define POLYGONPROPERTIES_H

#include <QObject>
#include <QQmlEngine>
#include <qcolor.h>

class PolygonProperties : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor NOTIFY borderColorChanged FINAL)
    Q_PROPERTY(qreal thickness READ thickness WRITE setThickness NOTIFY thicknessChanged)


public:
    explicit PolygonProperties(QObject *parent = nullptr);

    QColor borderColor() const {return m_bordercolor;}
    qreal thickness() const { return m_thickness; }
public slots:
    void setBorderColor(const QColor &color){
        if (m_bordercolor != color) {
            m_bordercolor = color;
            emit borderColorChanged();
        }

    }
    void setThickness(qreal thickness) {
        if (!qFuzzyCompare(m_thickness, thickness)) {
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
