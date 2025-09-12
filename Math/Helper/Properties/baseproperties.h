#ifndef BASEPROPERTIES_H
#define BASEPROPERTIES_H

#include <QObject>
#include <QQmlEngine>
#include <qcolor.h>
#include <qpoint.h>

class BaseProperties : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    // Basic Mobject
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QPointF pos READ pos WRITE setPos NOTIFY posChanged)
    Q_PROPERTY(QPointF size READ size WRITE setSize NOTIFY sizeChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(QString type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity NOTIFY opacityChanged FINAL)
public:
    explicit BaseProperties(QObject *parent = nullptr);
    QString name() const;
    QPointF pos() const;
    QPointF size() const;
    QColor color() const;
    qreal opacity() const;;
    QString type();;

public slots:
    void setName(const QString &name);
    void setPos(const QPointF &pos);
    void setSize(const QPointF &size);
    void setColor(const QColor &color);
    void setOpacity(qreal op);
    void setType(const QString &q);;

signals:

    void nameChanged();
    void posChanged(const QPointF& p);
    void sizeChanged(const QPointF& p);
    void colorChanged();
    void opacityChanged(qreal op);
    void typeChanged();

private:
    QString m_Name = "Mobject";
    QPointF m_pos{0,0};
    QPointF m_size{0,0};
    QColor m_color = Qt::transparent;
    qreal m_opacity =0.5;
    QString m_type = "Mobject";

};

#endif // BASEPROPERTIES_H
