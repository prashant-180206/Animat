#ifndef SIMPLETEXT_H
#define SIMPLETEXT_H

#include "Math/Mobjects/Base/Mobject.h"
#include <QColor>
#include <QString>
#include <QFont>
#include <QSGNode>
#include <qqmlintegration.h>

class Scene;

// SimpleText: A lightweight text mobject for labels without clicking functionality
class SimpleText : public Mobject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(int fontSize READ fontSize WRITE setFontSize NOTIFY fontSizeChanged)
    Q_PROPERTY(QString fontFamily READ fontFamily WRITE setFontFamily NOTIFY fontFamilyChanged)
    Q_PROPERTY(bool bold READ bold WRITE setBold NOTIFY boldChanged)
    Q_PROPERTY(bool italic READ italic WRITE setItalic NOTIFY italicChanged)
    Q_PROPERTY(QPointF position READ position WRITE setPosition NOTIFY positionChanged)

public:
    explicit SimpleText(Scene *canvas = nullptr, QQuickItem *parent = nullptr);

    // Property getters
    QString text() const { return m_text; }
    QColor color() const { return m_color; }
    int fontSize() const { return m_fontSize; }
    QString fontFamily() const { return m_fontFamily; }
    bool bold() const { return m_bold; }
    bool italic() const { return m_italic; }
    QPointF position() const { return m_position; }

    // Property setters
    void setText(const QString &text);
    void setColor(const QColor &color);
    void setFontSize(int size);
    void setFontFamily(const QString &family);
    void setBold(bool bold);
    void setItalic(bool italic);
    void setPosition(const QPointF &pos);

    // Convenience methods
    void setCenter(qreal x, qreal y);
    QSizeF textSize() const { return m_textSize; }

signals:
    void textChanged();
    void colorChanged();
    void fontSizeChanged();
    void fontFamilyChanged();
    void boldChanged();
    void italicChanged();
    void positionChanged();

protected:
    QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *) override;
    QRectF boundingRect() const override;

private:
    QString m_text;
    QColor m_color;
    int m_fontSize;
    QString m_fontFamily;
    bool m_bold;
    bool m_italic;
    QPointF m_position;
    QSizeF m_textSize; // Cache text size for bounds calculation

    void updateTextMetrics(); // Helper to calculate text bounds
    QFont createFont() const; // Helper to create QFont from properties
};

#endif // SIMPLETEXT_H