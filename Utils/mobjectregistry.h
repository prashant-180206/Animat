#pragma once
#include <QAbstractListModel>
#include <QList>

class MobjectRegistry : public QAbstractListModel {
    Q_OBJECT
public:
    explicit MobjectRegistry(QObject *parent = nullptr);

    struct Item {
        QString name;
        QString iconpath;
    };

    enum Roles { NameRole = Qt::UserRole + 1 ,IconRole};
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

protected:
    QHash<int, QByteArray> roleNames() const override;
private:
    QList<Item> m_items;
};
