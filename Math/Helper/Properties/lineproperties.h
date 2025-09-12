#ifndef LINEPROPERTIES_H
#define LINEPROPERTIES_H

#include <QObject>
#include <QQmlEngine>
#include <qpoint.h>

class LineProperties : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QPointF lineStart READ lineStart WRITE setLineStart NOTIFY lineStartChanged)
    Q_PROPERTY(QPointF lineEnd READ lineEnd WRITE setLineEnd NOTIFY lineEndChanged)
    Q_PROPERTY(qreal thickness READ thickness WRITE setThickness NOTIFY thicknessChanged)

public:
    explicit LineProperties(QObject *parent = nullptr);
    QPointF lineStart() const;;
    QPointF lineEnd() const;;
    qreal thickness() const;

public slots:
    void setLineStart(const QPointF &p);
    void setLineEnd(const QPointF &p);
    void setThickness(qreal thickness);

signals:

    void lineStartChanged(const QPointF &p);
    void lineEndChanged(const QPointF &p);
    void thicknessChanged();

private:
    QPointF m_lineStart{0,0};
    QPointF m_lineEnd{0,0};
    qreal m_thickness = 0;

};

#endif // LINEPROPERTIES_H
