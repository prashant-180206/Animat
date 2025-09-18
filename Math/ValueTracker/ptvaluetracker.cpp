#include "ptvaluetracker.h"



PtValueTracker::PtValueTracker()
{
}

QPointF PtValueTracker::value() const { return m_value; }



void PtValueTracker::setValue(QPointF v) {
    if (v != m_value) {
        m_value = v;
        emit valueChanged(v);
    }
}






