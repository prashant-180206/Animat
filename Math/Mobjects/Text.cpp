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
      m_position(0, 0),
      m_textSize(0, 0)
{
    setFlag(ItemHasContents, true);
    setAcceptedMouseButtons(Qt::AllButtons);

    // Set up base properties
    properties->base()->setName("Text");
    properties->base()->setColor(Qt::white);
    properties->base()->setPos(QPointF(0, 0));
    properties->base()->setType("Text");

    // Initialize text properties
    properties->setText(new TextProperties(this));
    properties->text()->setTextValue("Hello World");
    properties->text()->setFontSize(24);

    // Connect property changes to updates - use base color instead of text color
    connect(properties->text(), &TextProperties::textValueChanged, this, [this]()
            {
        updateTextMetrics();
        update();
        emit textChanged(); });

    connect(properties->text(), &TextProperties::fontSizeChanged, this, [this]()
            {
        updateTextMetrics();
        update();
        emit fontSizeChanged(); });

    // Connect font weight, bold, italic changes
    connect(properties->text(), &TextProperties::fontWeightChanged, this, [this]()
            {
        updateTextMetrics();
        update(); });

    connect(properties->text(), &TextProperties::boldChanged, this, [this]()
            {
        updateTextMetrics();
        update(); });

    connect(properties->text(), &TextProperties::italicChanged, this, [this]()
            {
        updateTextMetrics();
        update(); });

    connect(properties->text(), &TextProperties::fontFamilyChanged, this, [this]()
            {
        updateTextMetrics();
        update(); });

    connect(properties->base(), &BaseProperties::colorChanged, this, [this]()
            {
        update();
        emit colorChanged(); });

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
    if (properties->text() && properties->text()->textValue() == text)
        return;
    if (properties->text())
    {
        properties->text()->setTextValue(text);
    }
    // Note: updateTextMetrics and signals are handled by property change connections
}

void Text::setColor(const QColor &color)
{
    if (properties->base() && properties->base()->color() == color)
        return;
    if (properties->base())
    {
        properties->base()->setColor(color);
    }
    // Note: update and signals are handled by property change connections
}

void Text::setFontSize(int size)
{
    if (properties->text() && properties->text()->fontSize() == size)
        return;
    if (properties->text())
    {
        properties->text()->setFontSize(size);
    }
    // Note: updateTextMetrics and signals are handled by property change connections
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
    // Don't update m_position here - let the base class handle dragging logic
    // Just pass through to base class for proper drag initialization
    ClickableMobject::mousePressEvent(event);
}

void Text::mouseMoveEvent(QMouseEvent *event)
{
    // Let base class handle the dragging logic
    ClickableMobject::mouseMoveEvent(event);

    // Update our cached position after base class moves the object
    if (properties->base())
    {
        m_position = properties->base()->pos() - properties->base()->size()/2;
    }
}

void Text::mouseReleaseEvent(QMouseEvent *event)
{
    // Let base class handle the release logic
    ClickableMobject::mouseReleaseEvent(event);

    // Update our cached position after dragging is complete
    if (properties->base())
    {
        m_position = properties->base()->pos();
    }
}

void Text::updateTextMetrics()
{
    if (!properties->text())
        return;

    QFont font(properties->text()->fontFamily().isEmpty() ? "Arial" : properties->text()->fontFamily(),
               properties->text()->fontSize());

    // Set font weight properly - use QFont::Weight enum values
    int weight = properties->text()->fontWeight();
    if (weight >= 0 && weight <= 900)
    {
        font.setWeight(QFont::Weight(weight));
    }

    // Override with bold if set
    if (properties->text()->bold())
    {
        font.setBold(true);
    }

    font.setItalic(properties->text()->italic());

    QFontMetrics metrics(font);
    m_textSize = QSizeF(metrics.horizontalAdvance(properties->text()->textValue()),
                        metrics.height());

    // Update the item's implicit size
    properties->base()->setSize({m_textSize.height(), m_textSize.width()});
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

    if (!properties->text() || !properties->base())
    {
        return node;
    }

    // Configure font using properties
    QFont font(properties->text()->fontFamily().isEmpty() ? "Arial" : properties->text()->fontFamily(),
               properties->text()->fontSize());

    // Set font weight properly - use QFont::Weight enum values
    int weight = properties->text()->fontWeight();
    if (weight >= 0 && weight <= 900)
    {
        font.setWeight(QFont::Weight(weight));
    }

    // Override with bold if set
    if (properties->text()->bold())
    {
        font.setBold(true);
    }

    font.setItalic(properties->text()->italic());

    // Create layout (ownership passed to node)
    QTextLayout *layout = new QTextLayout(properties->text()->textValue(), font);

    QTextOption option;
    option.setWrapMode(QTextOption::NoWrap);
    layout->setTextOption(option);

    layout->beginLayout();
    layout->createLine();
    layout->endLayout();

    node->clear();
    node->setColor(properties->base()->color()); // Use base color

    // Center the text at the origin (like Line does with its geometry)
    QPointF textOffset(-m_textSize.width() / 2, -m_textSize.height() / 2);
    node->addTextLayout(textOffset, layout);

    return node;
}
