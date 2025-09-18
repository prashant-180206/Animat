#include "valuetracker.h"

ValueTracker::ValueTracker()
{    
}
void ValueTracker::setValue(qreal v) {
    if (v != m_value) {
        m_value = v;
        emit valueChanged(v);
    }
}






