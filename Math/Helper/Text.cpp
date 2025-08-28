#include "Text.h"
#include <QSGTextNode>
#include <QTextLayout>
#include <QFont>
#include <QQuickTextDocument>


Text::Text(Scene *canvas, QQuickItem *parent)
    : Mobject(canvas, parent),
    m_text("Hello World"),
    m_color(Qt::white),
    m_fontSize(24)
{
    setFlag(ItemHasContents, true);
}

void Text::setText(const QString &text)
{
    if (text == m_text) return;
    m_text = text;
    emit textChanged();
    update();
}

void Text::setColor(const QColor &color)
{
    if (color == m_color) return;
    m_color = color;
    emit colorChanged();
    update();
}

void Text::setFontSize(int size)
{
    if (size == m_fontSize) return;
    m_fontSize = size;
    emit fontSizeChanged();
    update();
}

QSGNode *Text::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
    QSGTextNode *node = static_cast<QSGTextNode *>(oldNode);
    if (!node) {
        node = this->window()->createTextNode();
    }

    // Configure font
    QFont font("Arial", m_fontSize);

    // Create layout (ownership passed to node)
    QTextLayout *layout = new QTextLayout(m_text, font);

    QTextOption option;
    option.setWrapMode(QTextOption::NoWrap);
    layout->setTextOption(option);

    layout->beginLayout();
    layout->createLine();

    layout->endLayout();

    node->clear();
    node->setColor(m_color);  // use your class member color
    node->addTextLayout(QPointF(0, 0), layout);  // anchor at top-left

    return node;
}
