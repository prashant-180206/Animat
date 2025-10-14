#ifndef PTTRACKERDATA_H
#define PTTRACKERDATA_H

#include "PtValueTracker.h"
#include <QQuickItem>

class PtTrackerData : public QQuickItem
{
    Q_OBJECT
    QML_ELEMENT
public:
    PtTrackerData();;

    PtValueTracker *tracker() const
    {
        return m_tracker;
    }
    void setTracker(PtValueTracker *newTracker)
    {
        m_tracker = newTracker;
    }

    qreal appeartime() const;
    void setAppeartime(qreal newAppeartime);

    QPointF min() const;
    void setMin(QPointF newMin);

    QPointF max() const;
    void setMax(QPointF newMax);

private:
    PtValueTracker* m_tracker ;
    QPointF m_min={0,0};
    QPointF m_max={0,0};
    qreal m_appeartime=1000;

    Q_PROPERTY(qreal appeartime READ appeartime WRITE setAppeartime NOTIFY appeartimeChanged FINAL)
    Q_PROPERTY(QPointF min READ min WRITE setMin NOTIFY minChanged FINAL)
    Q_PROPERTY(QPointF max READ max WRITE setMax NOTIFY maxChanged FINAL)

signals:

    void appeartimeChanged();
    void minChanged();
    void maxChanged();
};

#endif // PTTRACKERDATA_H
