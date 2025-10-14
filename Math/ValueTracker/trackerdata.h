#ifndef TRACKERDATA_H
#define TRACKERDATA_H

#include "ValueTracker.h"
#include <QQuickItem>

class TrackerData : public QQuickItem
{
    Q_OBJECT
    QML_ELEMENT
public:
    TrackerData();

    ValueTracker *tracker() const;
    void setTracker(ValueTracker *newTracker);

    qreal min() const;
    void setMin(qreal newMin);

    qreal max() const;
    void setMax(qreal newMax);

    qreal appeartime() const;
    void setAppeartime(qreal newAppeartime);

private:
    ValueTracker* m_tracker ;
    qreal m_min=0;
    qreal m_max=0;
    qreal m_appeartime=1000;

    Q_PROPERTY(qreal min READ min WRITE setMin NOTIFY minChanged FINAL)
    Q_PROPERTY(qreal max READ max WRITE setMax NOTIFY maxChanged FINAL)
    Q_PROPERTY(qreal appeartime READ appeartime WRITE setAppeartime NOTIFY appeartimeChanged FINAL)

signals:
    void minChanged();
    void maxChanged();
    void appeartimeChanged();
};

#endif // TRACKERDATA_H
