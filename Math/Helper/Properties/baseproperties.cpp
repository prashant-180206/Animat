#include "baseproperties.h"

BaseProperties::BaseProperties(QObject *parent)
    : QObject(parent)
{
    setParent(parent);
}

QString BaseProperties::name() const { return m_Name; }

QPointF BaseProperties::pos() const { return m_pos; }

QPointF BaseProperties::size() const { return m_size; }

QColor BaseProperties::color() const { return m_color; }

qreal BaseProperties::opacity() const {return m_opacity;}

QString BaseProperties::type(){return m_type;}

void BaseProperties::setName(const QString &name) {
    if (m_Name != name) {
        m_Name = name;
        emit nameChanged();
    }
}

void BaseProperties::setPos(const QPointF &pos) {
    if (m_pos != pos) {
        m_pos = pos;
        emit posChanged(pos);
    }
}

void BaseProperties::setSize(const QPointF &size) {
    if (m_size != size) {
        m_size = size;
        emit sizeChanged(size);
    }
}

void BaseProperties::setColor(const QColor &color) {
    if (m_color != color) {
        m_color = color;
        emit colorChanged();
    }
}

void BaseProperties::setOpacity(qreal op){
    if(m_opacity!=op){
        m_opacity=op;
        emit opacityChanged(op);
    }
}

void BaseProperties::setType(const QString &q){
    if(q!=m_type){
        m_type = q;
        emit typeChanged();
    }
}
