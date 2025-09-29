#ifndef FUNCS_H
#define FUNCS_H

#include <QQuickItem>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QEasingCurve>
#include <QParallelAnimationGroup>

void morphItems(QQuickItem* from, QQuickItem* to)
{
    if (!from || !to) return;

    // Place 'to' at same position/size as 'from'
    to->setVisible(true);
    to->setOpacity(0.0);
    to->setPosition(from->position());
    to->setWidth(from->width());
    to->setHeight(from->height());

    // Fade out + shrink "from"
    auto* fadeOut = new QPropertyAnimation(from, "opacity");
    fadeOut->setDuration(1000);
    fadeOut->setStartValue(1.0);
    fadeOut->setEndValue(0.0);
    fadeOut->setEasingCurve(QEasingCurve::InOutQuad);

    auto* shrink = new QPropertyAnimation(from, "scale");
    shrink->setDuration(1000);
    shrink->setStartValue(1.0);
    shrink->setEndValue(0.5);
    shrink->setEasingCurve(QEasingCurve::InOutQuad);

    // Fade in + grow "to"
    auto* fadeIn = new QPropertyAnimation(to, "opacity");
    fadeIn->setDuration(1000);
    fadeIn->setStartValue(0.0);
    fadeIn->setEndValue(1.0);
    fadeIn->setEasingCurve(QEasingCurve::InOutQuad);

    auto* grow = new QPropertyAnimation(to, "scale");
    grow->setDuration(1000);
    grow->setStartValue(1.5);
    grow->setEndValue(1.0);
    grow->setEasingCurve(QEasingCurve::InOutQuad);

    // Run in parallel
    auto* group = new QParallelAnimationGroup(from);
    group->addAnimation(fadeOut);
    group->addAnimation(shrink);
    group->addAnimation(fadeIn);
    group->addAnimation(grow);

    // Cleanup when finished
    QObject::connect(group, &QParallelAnimationGroup::finished, [from]{
        from->setVisible(false);
        from->setOpacity(1.0);
        from->setScale(1.0);
    });

    group->start(QAbstractAnimation::DeleteWhenStopped);
}


#endif // FUNCS_H
