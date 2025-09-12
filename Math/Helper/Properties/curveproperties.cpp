#include "curveproperties.h"



QString CurveProperties::curveXFunc() const { return m_Curve_X_func; }

QString CurveProperties::curveYFunc() const { return m_Curve_Y_func; }

QPointF CurveProperties::tRange() const { return m_tRange;}

int CurveProperties::segments(){return m_segments;}

void CurveProperties::setCurveXFunc(const QString &func) {
    if (m_Curve_X_func != func) {
        m_Curve_X_func = func;
        emit curveXFuncChanged();
    }
}

void CurveProperties::setCurveYFunc(const QString &func) {
    if (m_Curve_Y_func != func) {
        m_Curve_Y_func = func;
        emit curveYFuncChanged();
    }
}

void CurveProperties::setTRange(const QPointF &range) {
    if (m_tRange != range) {
        m_tRange = range;
        emit tRangeChanged();
    }
}

void CurveProperties::setSegments(int segments)
{
    if (m_segments != segments) {
        m_segments = segments;
        emit segmentsChanged();
    }
}
