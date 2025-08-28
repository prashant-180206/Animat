#ifndef TEXT_H
#define TEXT_H


#include <QColor>
#include <QString>
#include <Math/Mobjects/Mobject.h>

class Text : public Mobject
{
    Q_OBJECT
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(int fontSize READ fontSize WRITE setFontSize NOTIFY fontSizeChanged)

public:
    explicit Text(Scene *canvas = nullptr, QQuickItem *parent = nullptr);

    QString text() const { return m_text; }
    QColor color() const { return m_color; }
    int fontSize() const { return m_fontSize; }

    void setText(const QString &text);
    void setColor(const QColor &color);
    void setFontSize(int size);

signals:
    void textChanged();
    void colorChanged();
    void fontSizeChanged();

protected:
    QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *) override;

private:
    QString m_text;
    QColor m_color;
    int m_fontSize;
};

#endif // TEXT_H
