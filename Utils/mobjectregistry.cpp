#include "mobjectregistry.h"


MobjectRegistry::MobjectRegistry(QObject *parent)
    : QAbstractListModel(parent) {

    m_items = {
        {"Circle", "qrc:assets/icons/circle.svg"},
        {"Line", "qrc:assets/icons/matrix.svg"},
        {"Text", "qrc:assets/icons/text.svg"},
        {"Curve", "qrc:assets/icons/matrix.svg"},
        {"Rectangle", "qrc:assets/icons/rect.svg"},
        {"Polygon", "qrc:assets/icons/polygon"},
        {"Circle", "qrc:assets/icons/circle"},
    };
}

int MobjectRegistry::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return m_items.count();
}

QVariant MobjectRegistry::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() < 0 || index.row() >= m_items.count())
        return QVariant();

    const Item &item = m_items.at(index.row());
    switch(role) {
    case NameRole:
        return item.name;
    case IconRole:
        return item.iconpath;
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> MobjectRegistry::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[IconRole]= "iconpath";
    return roles;
}
