#ifndef TEXT_H
#define TEXT_H

#include "Math/Helper/ClickableMobject.h"
#include <QColor>
#include <QString>
// #include <Math/Mobjects/Mobject.h>

class Text : public ClickableMobject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(int fontSize READ fontSize WRITE setFontSize NOTIFY fontSizeChanged)

public:
    explicit Text(Scene *canvas = nullptr, QQuickItem *parent = nullptr);

    QString text() const { return properties->text() ? properties->text()->textValue() : "Hello World"; }
    QColor color() const { return properties->base() ? properties->base()->color() : Qt::white; }
    int fontSize() const { return properties->text() ? properties->text()->fontSize() : 24; }

    void setText(const QString &text);
    void setColor(const QColor &color);
    void setFontSize(int size);

    // Override essential methods for proper mobject behavior
    virtual void setCenter(qreal x, qreal y) override;

signals:
    void textChanged();
    void colorChanged();
    void fontSizeChanged();

protected:
    QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *) override;
    bool contains(const QPointF &point) const override;
    QRectF boundingRect() const override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QPointF m_position; // Store logical position
    QSizeF m_textSize;  // Cache text size for bounds calculation

    void updateTextMetrics(); // Helper to calculate text bounds
};

#endif // TEXT_H
