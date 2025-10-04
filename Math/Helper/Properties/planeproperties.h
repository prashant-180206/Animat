#ifndef PLANEPROPERTIES_H
#define PLANEPROPERTIES_H

#include <QObject>
#include <QQmlEngine>
#include <qqmlintegration.h>
#include <QColor>
#include <qjsondocument.h>
#include <QJsonObject>

class PlaneProperties : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QColor axisColor READ axisColor WRITE setAxisColor NOTIFY axisColorChanged)
    Q_PROPERTY(QColor lineColor READ lineColor WRITE setLineColor NOTIFY lineColorChanged)
    Q_PROPERTY(QColor labelColor READ labelColor WRITE setLabelColor NOTIFY labelColorChanged)
    Q_PROPERTY(qreal step READ step WRITE setStep NOTIFY stepChanged)
    Q_PROPERTY(bool showLabels READ showLabels WRITE setShowLabels NOTIFY showLabelsChanged)
    Q_PROPERTY(bool showGrid READ showGrid WRITE setShowGrid NOTIFY showGridChanged)
    Q_PROPERTY(bool showAxes READ showAxes WRITE setShowAxes NOTIFY showAxesChanged)
    Q_PROPERTY(qreal axisThickness READ axisThickness WRITE setAxisThickness NOTIFY axisThicknessChanged)
    Q_PROPERTY(qreal gridThickness READ gridThickness WRITE setGridThickness NOTIFY gridThicknessChanged)
    Q_PROPERTY(int labelFontSize READ labelFontSize WRITE setLabelFontSize NOTIFY labelFontSizeChanged)
    Q_PROPERTY(qreal height READ height WRITE setHeight NOTIFY heightChanged)
    Q_PROPERTY(qreal width READ width WRITE setWidth NOTIFY widthChanged)

public:
    explicit PlaneProperties(QObject *parent = nullptr);

    QColor axisColor() const { return m_axisColor; }
    QColor lineColor() const { return m_lineColor; }
    QColor labelColor() const { return m_labelColor; }
    qreal step() const { return m_step; }
    bool showLabels() const { return m_showLabels; }
    bool showGrid() const { return m_showGrid; }
    bool showAxes() const { return m_showAxes; }
    qreal axisThickness() const { return m_axisThickness; }
    qreal gridThickness() const { return m_gridThickness; }
    int labelFontSize() const { return m_labelFontSize; }
    qreal height() const;
    qreal width() const;

    struct PlanePropData
    {
        QColor axisColor;
        QColor lineColor;
        QColor labelColor;
        qreal step = 0;
        bool showLabels = false;
        bool showGrid = false;
        bool showAxes = false;
        qreal axisThickness = 0;
        qreal gridThickness = 0;
        int labelFontSize = 0;
        qreal height = 0;
        qreal width = 0;
        QJsonDocument toJson() const;
        static PlanePropData fromJSON(const QJsonObject &o);
    };

    PlanePropData getData() const;

    void setfromJSON(const QJsonObject &o);

    // Property setters
    void setAxisColor(const QColor &color);
    void setLineColor(const QColor &color);
    void setLabelColor(const QColor &color);
    void setStep(qreal step);
    void setShowLabels(bool show);
    void setShowGrid(bool show);
    void setShowAxes(bool show);
    void setAxisThickness(qreal thickness);
    void setGridThickness(qreal thickness);
    void setLabelFontSize(int size);
    void setHeight(qreal height);
    void setWidth(qreal width);

signals:
    void axisColorChanged();
    void lineColorChanged();
    void labelColorChanged();
    void stepChanged();
    void showLabelsChanged();
    void showGridChanged();
    void showAxesChanged();
    void axisThicknessChanged();
    void gridThicknessChanged();
    void labelFontSizeChanged();
    void heightChanged();
    void widthChanged();

private:
    QColor m_axisColor = Qt::white;
    QColor m_lineColor = Qt::black;
    QColor m_labelColor = Qt::black;
    qreal m_step = 1.0;
    bool m_showLabels = true;
    bool m_showGrid = true;
    bool m_showAxes = true;
    qreal m_axisThickness = 1.0;
    qreal m_gridThickness = 1.0;
    int m_labelFontSize = 12;
    qreal m_height = 4;
    qreal m_width = 4;
};

#endif // PLANEPROPERTIES_H
