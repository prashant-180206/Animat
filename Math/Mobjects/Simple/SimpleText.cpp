#include "SimpleText.h"
#include "Math/Scene.h"
#include <QFont>
#include <QFontMetrics>
#include <QPainter>
#include <QSGSimpleTextureNode>
#include <QQuickWindow>
#include <QPixmap>

SimpleText::SimpleText(Scene *canvas, QQuickItem *parent)
    : Mobject(parent),
      m_text("Label"),
      m_color(Qt::white),
      m_fontSize(16),
      m_fontFamily("Arial"),
      m_bold(false),
      m_italic(false),
      m_position(0, 0),
      m_textSize(0, 0)
{
    setFlag(ItemHasContents, true);
    updateTextMetrics();
}

void SimpleText::setText(const QString &text)
{
    if (m_text != text)
    {
        m_text = text;
        updateTextMetrics();
        update();
        emit textChanged();
    }
}

void SimpleText::setColor(const QColor &color)
{
    if (m_color != color)
    {
        m_color = color;
        update();
        emit colorChanged();
    }
}

void SimpleText::setFontSize(int size)
{
    if (m_fontSize != size && size > 0)
    {
        m_fontSize = size;
        updateTextMetrics();
        update();
        emit fontSizeChanged();
    }
}

void SimpleText::setFontFamily(const QString &family)
{
    if (m_fontFamily != family)
    {
        m_fontFamily = family;
        updateTextMetrics();
        update();
        emit fontFamilyChanged();
    }
}

void SimpleText::setBold(bool bold)
{
    if (m_bold != bold)
    {
        m_bold = bold;
        updateTextMetrics();
        update();
        emit boldChanged();
    }
}

void SimpleText::setItalic(bool italic)
{
    if (m_italic != italic)
    {
        m_italic = italic;
        updateTextMetrics();
        update();
        emit italicChanged();
    }
}

void SimpleText::setPosition(const QPointF &pos)
{
    if (m_position != pos)
    {
        m_position = pos;
        setX(pos.x() - m_textSize.width() / 2);
        setY(pos.y() - m_textSize.height() / 2);
        emit positionChanged();
    }
}

void SimpleText::setCenter(qreal x, qreal y)
{
    setPosition(QPointF(x, y));
}

QFont SimpleText::createFont() const
{
    QFont font(m_fontFamily, m_fontSize);
    font.setBold(m_bold);
    font.setItalic(m_italic);
    return font;
}

void SimpleText::updateTextMetrics()
{
    QFont font = createFont();
    QFontMetrics metrics(font);

    m_textSize = QSizeF(metrics.horizontalAdvance(m_text), metrics.height());

    // Update item size
    setImplicitWidth(m_textSize.width());
    setImplicitHeight(m_textSize.height());

    // Update position to keep text centered
    setX(m_position.x() - m_textSize.width() / 2);
    setY(m_position.y() - m_textSize.height() / 2);
}

QRectF SimpleText::boundingRect() const
{
    return QRectF(0, 0, m_textSize.width(), m_textSize.height());
}

QSGNode *SimpleText::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
    // For SimpleText, we'll use a simplified approach
    // Instead of complex text node, use QPainter to create texture

    if (m_text.isEmpty())
    {
        delete oldNode;
        return nullptr;
    }

    QSGSimpleTextureNode *textureNode = static_cast<QSGSimpleTextureNode *>(oldNode);
    if (!textureNode)
    {
        textureNode = new QSGSimpleTextureNode();
    }

    // Create font and calculate text size
    QFont font = createFont();
    QFontMetrics metrics(font);

    int textWidth = metrics.horizontalAdvance(m_text);
    int textHeight = metrics.height();

    if (textWidth <= 0 || textHeight <= 0)
    {
        textureNode->setRect(QRectF());
        return textureNode;
    }

    // Create pixmap and draw text
    QPixmap pixmap(textWidth, textHeight);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setFont(font);
    painter.setPen(m_color);

    // Draw text at baseline position
    int baseline = metrics.ascent();
    painter.drawText(0, baseline, m_text);
    painter.end();

    // Convert to texture and set on node
    if (window())
    {
        QSGTexture *texture = window()->createTextureFromImage(pixmap.toImage());
        if (texture)
        {
            textureNode->setTexture(texture);
            textureNode->setRect(QRectF(0, 0, textWidth, textHeight));
            textureNode->setOwnsTexture(true);
        }
    }

    return textureNode;
}