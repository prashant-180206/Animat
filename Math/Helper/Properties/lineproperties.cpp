#include "lineproperties.h"

LineProperties::LineProperties(QObject *parent)
    : QObject{parent}
{}

QPointF LineProperties::lineStart() const{return m_lineStart;}

QPointF LineProperties::lineEnd() const{return m_lineEnd;}

qreal LineProperties::thickness() const { return m_thickness; }

void LineProperties::setLineStart(const QPointF &p){
    if (p!=m_lineStart){
        m_lineStart = p;
        emit lineStartChanged(p);
    }
}

void LineProperties::setLineEnd(const QPointF &p){
    if (p!=m_lineEnd){
        m_lineEnd = p;
        emit lineEndChanged(p);
    }
}

void LineProperties::setThickness(qreal thickness) {
    if (!qFuzzyCompare(m_thickness, thickness)) {
        m_thickness = thickness;
        emit thicknessChanged();
    }
}




