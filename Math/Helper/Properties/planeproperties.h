#ifndef PLANEPROPERTIES_H
#define PLANEPROPERTIES_H

#include <QObject>
#include <QQmlEngine>
#include <qqmlintegration.h>
#include <QColor>

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

public:
    explicit PlaneProperties(QObject *parent = nullptr);

    // Property getters
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

private:
    QColor m_axisColor;
    QColor m_lineColor;
    QColor m_labelColor;
    qreal m_step;
    bool m_showLabels;
    bool m_showGrid;
    bool m_showAxes;
    qreal m_axisThickness;
    qreal m_gridThickness;
    int m_labelFontSize;
};

#endif // PLANEPROPERTIES_H