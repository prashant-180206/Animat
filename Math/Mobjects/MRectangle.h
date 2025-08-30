#ifndef MRECTANGLE_H
#define MRECTANGLE_H

#include "Polygon.h"
#include <QQuickItem>

class MRectangle : public Polygon
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(qreal rectWidth READ rectWidth WRITE setRectWidth NOTIFY rectWidthChanged)
    Q_PROPERTY(qreal rectHeight READ rectHeight WRITE setRectHeight NOTIFY rectHeightChanged)

public:
    explicit MRectangle(Scene *canvas, QQuickItem *parent = nullptr);

    qreal rectWidth() const;
    void setRectWidth(qreal width);

    qreal rectHeight() const;
    void setRectHeight(qreal height);

signals:
    void rectWidthChanged();
    void rectHeightChanged();

protected:
    void updatePoints(qreal height,qreal width , QPointF center);

private:
    qreal m_rectWidth = 2;
    qreal m_rectHeight = 1;
};

#endif // MRECTANGLE_H
