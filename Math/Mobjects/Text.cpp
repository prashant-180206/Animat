#include "Text.h"
#include "Math/Scene.h"
#include <QSGTextNode>
#include <QTextLayout>
#include <QFont>
#include <QFontMetrics>
#include <QQuickTextDocument>
#include <QMouseEvent>

Text::Text(Scene *canvas, QQuickItem *parent)
    : ClickableMobject(canvas, parent),
      m_text("Hello World"),
      m_color(Qt::white),
      m_fontSize(24),
      m_position(0, 0),
      m_textSize(0, 0)
{
    setFlag(ItemHasContents, true);
    setAcceptedMouseButtons(Qt::AllButtons);

    // Set up base properties like Line does
    properties->base()->setName("Text");
    properties->base()->setColor(m_color);
    properties->base()->setPos(QPointF(0, 0));
    properties->base()->setType("Text");

    // Initialize text properties
    properties->setText(new TextProperties(this));
    properties->text()->setTextValue(m_text);
    properties->text()->setFontSize(m_fontSize);
    properties->text()->setTextColor(m_color);

    // Connect text properties signals to updates
    connect(properties->text(), &TextProperties::textValueChanged, this, [this]()
            { setText(properties->text()->textValue()); });

    connect(properties->text(), &TextProperties::fontSizeChanged, this, [this]()
            { setFontSize(properties->text()->fontSize()); });

    connect(properties->text(), &TextProperties::textColorChanged, this, [this]()
            { setColor(properties->text()->textColor()); });

    // Connect to window changes for updates
    connect(this, &QQuickItem::windowChanged, this, [this](QQuickWindow *w)
            {
        if(w){
            updateTextMetrics(); // Update metrics when window is available
            update();
        } });

    // Connect properties changes to updates
    connect(this, &Text::textChanged, this, &Text::updateTextMetrics);
    connect(this, &Text::fontSizeChanged, this, &Text::updateTextMetrics);

    // Initial position setup
    m_position = properties->base()->pos();
    updateTextMetrics();
}

void Text::setText(const QString &text)
{
    if (text == m_text)
        return;
    m_text = text;
    if (properties->text())
    {
        properties->text()->setTextValue(text);
    }
    updateTextMetrics();
    emit textChanged();
    update();
}

void Text::setColor(const QColor &color)
{
    if (color == m_color)
        return;
    m_color = color;
    properties->base()->setColor(color); // Keep properties in sync
    if (properties->text())
    {
        properties->text()->setTextColor(color);
    }
    emit colorChanged();
    update();
}

void Text::setFontSize(int size)
{
    if (size == m_fontSize)
        return;
    m_fontSize = size;
    if (properties->text())
    {
        properties->text()->setFontSize(size);
    }
    updateTextMetrics();
    emit fontSizeChanged();
    update();
}

void Text::setCenter(qreal x, qreal y)
{
    QPointF newCenter(x, y);
    m_position = newCenter;
    properties->base()->setPos(newCenter);

    QPointF canvasCenter = getcanvas()->p2c(newCenter);
    setX(canvasCenter.x() - m_textSize.width() / 2); // Center the text
    setY(canvasCenter.y() - m_textSize.height() / 2);
    setZ(50);
}

void Text::mousePressEvent(QMouseEvent *event)
{
    m_position = properties->base()->pos();
    ClickableMobject::mousePressEvent(event);
}

void Text::updateTextMetrics()
{
    QFont font("Arial", m_fontSize);
    QFontMetrics metrics(font);
    m_textSize = QSizeF(metrics.horizontalAdvance(m_text), metrics.height());

    // Update the item's implicit size
    setImplicitWidth(m_textSize.width());
    setImplicitHeight(m_textSize.height());
}

QRectF Text::boundingRect() const
{
    // Return bounding rectangle in local coordinates
    return QRectF(-m_textSize.width() / 2, -m_textSize.height() / 2,
                  m_textSize.width(), m_textSize.height());
}

bool Text::contains(const QPointF &point) const
{
    // Check if point is within the text bounds
    return boundingRect().contains(point);
}

QSGNode *Text::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
    QSGTextNode *node = static_cast<QSGTextNode *>(oldNode);
    if (!node)
    {
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
    node->setColor(m_color);

    // Center the text at the origin (like Line does with its geometry)
    QPointF textOffset(-m_textSize.width() / 2, -m_textSize.height() / 2);
    node->addTextLayout(textOffset, layout);

    return node;
}
