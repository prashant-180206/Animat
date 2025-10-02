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
        QJsonDocument toJson() const
        {
            QJsonObject o;
            o["axisColor"] = QJsonObject{{"r", axisColor.red()}, {"g", axisColor.green()}, {"b", axisColor.blue()}, {"a", axisColor.alpha()}};
            o["lineColor"] = QJsonObject{{"r", lineColor.red()}, {"g", lineColor.green()}, {"b", lineColor.blue()}, {"a", lineColor.alpha()}};
            o["labelColor"] = QJsonObject{{"r", labelColor.red()}, {"g", labelColor.green()}, {"b", labelColor.blue()}, {"a", labelColor.alpha()}};
            o["step"] = step;
            o["showLabels"] = showLabels;
            o["showGrid"] = showGrid;
            o["showAxes"] = showAxes;
            o["axisThickness"] = axisThickness;
            o["gridThickness"] = gridThickness;
            o["labelFontSize"] = labelFontSize;
            return QJsonDocument(o);
        }
        static PlanePropData fromJSON(const QJsonObject &o)
        {
            PlanePropData d;
            auto ac = o["axisColor"].toObject();
            d.axisColor = QColor(ac["r"].toInt(), ac["g"].toInt(), ac["b"].toInt(), ac["a"].toInt());
            auto lc = o["lineColor"].toObject();
            d.lineColor = QColor(lc["r"].toInt(), lc["g"].toInt(), lc["b"].toInt(), lc["a"].toInt());
            auto labc = o["labelColor"].toObject();
            d.labelColor = QColor(labc["r"].toInt(), labc["g"].toInt(), labc["b"].toInt(), labc["a"].toInt());
            d.step = o["step"].toDouble();
            d.showLabels = o["showLabels"].toBool();
            d.showGrid = o["showGrid"].toBool();
            d.showAxes = o["showAxes"].toBool();
            d.axisThickness = o["axisThickness"].toDouble();
            d.gridThickness = o["gridThickness"].toDouble();
            d.labelFontSize = o["labelFontSize"].toInt();
            return d;
        }
    };

    PlanePropData getData() const
    {
        PlanePropData d;
        d.axisColor = m_axisColor;
        d.lineColor = m_lineColor;
        d.labelColor = m_labelColor;
        d.step = m_step;
        d.showLabels = m_showLabels;
        d.showGrid = m_showGrid;
        d.showAxes = m_showAxes;
        d.axisThickness = m_axisThickness;
        d.gridThickness = m_gridThickness;
        d.labelFontSize = m_labelFontSize;
        return d;
    }

    void setfromJSON(const QJsonObject &o)
    {
        PlanePropData d = PlanePropData::fromJSON(o);
        setAxisColor(d.axisColor);
        setLineColor(d.lineColor);
        setLabelColor(d.labelColor);
        setStep(d.step);
        setShowLabels(d.showLabels);
        setShowGrid(d.showGrid);
        setShowAxes(d.showAxes);
        setAxisThickness(d.axisThickness);
        setGridThickness(d.gridThickness);
        setLabelFontSize(d.labelFontSize);
    }

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
