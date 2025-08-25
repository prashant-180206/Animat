#ifndef MOBJECT_H
#define MOBJECT_H

#include <QQuickItem>
#include <QPointF>
#include <QString>


class Mobject : public QQuickItem
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QPointF center READ center WRITE setCenter NOTIFY centerChanged FINAL)
    Q_PROPERTY(QString id READ id WRITE setId NOTIFY idChanged FINAL)


public:
    Mobject(QQuickItem* parent = nullptr);

    QPointF center() const;
    QString id() const;
    void setCenter(const QPointF& v);
    void setId(QString i);

signals:
    void centerChanged();
    void idChanged();

private:
    QPointF m_center;
    QString m_id;

};

#endif // MOBJECT_H
