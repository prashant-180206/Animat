#include "trackerdata.h"

TrackerData::TrackerData():m_tracker(new ValueTracker()) {
    m_tracker->setParent(this);
}

ValueTracker *TrackerData::tracker() const
{
    return m_tracker;
}

void TrackerData::setTracker(ValueTracker *newTracker)
{
    m_tracker = newTracker;
}

qreal TrackerData::min() const
{
    return m_min;
}

void TrackerData::setMin(qreal newMin)
{
    if (qFuzzyCompare(m_min, newMin))
        return;
    m_min = newMin;
    emit minChanged();
}

qreal TrackerData::max() const
{
    return m_max;
}

void TrackerData::setMax(qreal newMax)
{
    if (qFuzzyCompare(m_max, newMax))
        return;
    m_max = newMax;
    emit maxChanged();
}

qreal TrackerData::appeartime() const
{
    return m_appeartime;
}

void TrackerData::setAppeartime(qreal newAppeartime)
{
    if (qFuzzyCompare(m_appeartime, newAppeartime))
        return;
    m_appeartime = newAppeartime;
    emit appeartimeChanged();
}
