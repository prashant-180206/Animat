#include "animation.h"
#include "Scene.h"
#include "funcmap.h"



Animation::Animation(qreal startOffset, qreal duration, QEasingCurve::Type easingtype)
    : m_startOffset(startOffset), m_duration(duration), m_ltime(0) {}

Animation::~Animation() {}

void Animation::setLtime(qreal sceneTime) {
    qreal localTime = sceneTime - m_startOffset;
    if (localTime < 0) m_ltime = 0;
    else if (localTime > m_duration) m_ltime = 1;
    else m_ltime = localTime / m_duration;
}

qreal Animation::progress() const { return m_ltime; }



qreal Animation::getStartOffset() const { return m_startOffset; }

qreal Animation::getDuration() const { return m_duration; }

MoveAnimation::MoveAnimation(ClickableMobject *mobj, QPointF startPos, QPointF targetPos, qreal startOffset, qreal duration)
    : Animation(startOffset, duration), m_mobj(mobj),
    m_startPos(startPos), m_targetPos(targetPos)
{}

void MoveAnimation::apply() {
    qreal t = easedProgress();
    QPointF diff = m_targetPos - m_startPos;
    QPointF newPos = m_startPos + diff * t;
    m_mobj->getProperties()->base()->setPos(newPos);
}



CreateAnimation::CreateAnimation(ClickableMobject *mobj, qreal startOffset, qreal duration)
    : Animation(startOffset, duration), m_mobj(mobj)
{}

void CreateAnimation::apply() {
    qreal newOpacity = easedProgress(); // from 0 to 1 with easing
    m_mobj->getProperties()->base()->setOpacity(newOpacity);
}

DestroyAnimation::DestroyAnimation(ClickableMobject *mobj, qreal startOffset, qreal duration)
    : Animation(startOffset, duration), m_mobj(mobj)
{}

void DestroyAnimation::apply() {
    qreal newOpacity = 1.0 - easedProgress(); // from 1 to 0 with easing
    m_mobj->getProperties()->base()->setOpacity(newOpacity);
}

CustomScalarAnimation::CustomScalarAnimation(ClickableMobject *mobj, QString prop, qreal startVal, qreal targetVal, qreal startOffset, qreal duration)
    : Animation(startOffset, duration), m_mobj(mobj), m_prop(prop),
    m_startVal(startVal), m_targetVal(targetVal)
{}

void CustomScalarAnimation::apply() {
    auto func = FuncMap::ConnectFunc[m_prop];
    if (!func) return;
    qreal t = easedProgress();
    qreal diff = m_targetVal - m_startVal;
    qreal newVal = m_startVal + diff * t;
    func(m_mobj, newVal);
}

CustomPointAnimation::CustomPointAnimation(ClickableMobject *mobj, QString prop, QPointF startVal, QPointF targetVal, qreal startOffset, qreal duration)
    : Animation(startOffset, duration), m_mobj(mobj), m_prop(prop),
    m_startVal(startVal), m_targetVal(targetVal)
{}

void CustomPointAnimation::apply() {
    auto func = FuncMap::PtConnectFunc[m_prop];
    if (!func) return;
    qreal t = easedProgress();
    QPointF diff = m_targetVal - m_startVal;
    QPointF newVal = m_startVal + diff * t;
    func(m_mobj, newVal);
}

ValueAnimation::ValueAnimation(qreal startVal, qreal endVal, qreal startOffset, qreal duration)
    : Animation(startOffset, duration), m_startVal(startVal), m_endVal(endVal) {}

void ValueAnimation::addTarget(ClickableMobject *mobj, QString prop) {
    m_targets.push_back({mobj, prop});
}

void ValueAnimation::apply() {
    qreal t = easedProgress();
    qreal currentVal = m_startVal + (m_endVal - m_startVal) * t;
    for (const auto& target : std::as_const(m_targets)) {
        auto func = FuncMap::ConnectFunc[target.prop];
        if (func) {
            func(target.mobj, currentVal);
        }
    }
}



