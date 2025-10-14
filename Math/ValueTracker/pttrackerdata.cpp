#include "pttrackerdata.h"


QPointF PtTrackerData::min() const
{
    return m_min;
}

void PtTrackerData::setMin(QPointF newMin)
{
    if (m_min == newMin)
        return;
    m_min = newMin;
    emit minChanged();
}

QPointF PtTrackerData::max() const
{
    return m_max;
}

void PtTrackerData::setMax(QPointF newMax)
{
    if (m_max == newMax)
        return;
    m_max = newMax;
    emit maxChanged();
}

PtTrackerData::PtTrackerData():m_tracker(new PtValueTracker()){
    m_tracker->setParent(this);
}

qreal PtTrackerData::appeartime() const
{
    return m_appeartime;
}

void PtTrackerData::setAppeartime(qreal newAppeartime)
{
    if (qFuzzyCompare(m_appeartime, newAppeartime))
        return;
    m_appeartime = newAppeartime;
    emit appeartimeChanged();
}
