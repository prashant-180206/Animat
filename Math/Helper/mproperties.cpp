#include "mproperties.h"


MProperties::MProperties(QObject *parent)
{

}

QString MProperties::name() const { return m_Name; }

QPointF MProperties::pos() const { return m_pos; }

QPair<qreal, qreal> MProperties::size() const { return m_size; }

QColor MProperties::color() const { return m_color; }

QPair<QPointF, QPointF> MProperties::linePoints() const { return m_linepoints; }

qreal MProperties::thickness() const { return m_thickness; }

QString MProperties::curveXFunc() const { return m_Curve_X_func; }

QString MProperties::curveYFunc() const { return m_Curve_Y_func; }

QPair<qreal, qreal> MProperties::tRange() const { return m_tRange; }

QList<QPointF> MProperties::endPoints() const { return m_endPoints; }

qreal MProperties::radius() const { return m_radius; }

void MProperties::setName(const QString &name) {
    if (m_Name != name) {
        m_Name = name;
        emit nameChanged();
    }
}

void MProperties::setPos(const QPointF &pos) {
    if (m_pos != pos) {
        m_pos = pos;
        emit posChanged();
    }
}

void MProperties::setSize(const QPair<qreal, qreal> &size) {
    if (m_size != size) {
        m_size = size;
        emit sizeChanged();
    }
}

void MProperties::setColor(const QColor &color) {
    if (m_color != color) {
        m_color = color;
        emit colorChanged();
    }
}

void MProperties::setLinePoints(const QPair<QPointF, QPointF> &points) {
    if (m_linepoints != points) {
        m_linepoints = points;
        emit linePointsChanged();
    }
}

void MProperties::setThickness(qreal thickness) {
    if (!qFuzzyCompare(m_thickness, thickness)) {
        m_thickness = thickness;
        emit thicknessChanged();
    }
}

void MProperties::setCurveXFunc(const QString &func) {
    if (m_Curve_X_func != func) {
        m_Curve_X_func = func;
        emit curveXFuncChanged();
    }
}

void MProperties::setCurveYFunc(const QString &func) {
    if (m_Curve_Y_func != func) {
        m_Curve_Y_func = func;
        emit curveYFuncChanged();
    }
}

void MProperties::setTRange(const QPair<qreal, qreal> &range) {
    if (m_tRange != range) {
        m_tRange = range;
        emit tRangeChanged();
    }
}

void MProperties::setEndPoints(const QList<QPointF> &points) {
    if (m_endPoints != points) {
        m_endPoints = points;
        emit endPointsChanged();
    }
}

void MProperties::setRadius(qreal radius) {
    if (!qFuzzyCompare(m_radius, radius)) {
        m_radius = radius;
        emit radiusChanged();
    }
}
