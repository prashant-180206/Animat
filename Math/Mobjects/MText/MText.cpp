#include "MText.h"
#include <QSGSimpleRectNode>
#include <QSGSimpleTextureNode>
#include <QPainter>
#include <QImage>
#include <QTextCursor>
#include <QTextDocument>
#include <QEvent>
#include <QPalette>

MText::MText(Scene *canvas, QQuickItem *parent)
    : ClickableMobject(canvas, parent)
{
    setAcceptedMouseButtons(Qt::LeftButton);
    setAcceptHoverEvents(true);
    setFlag(ItemIsFocusScope, true);
    setFocus(false);

    setRichText("Type Here");
    updateSizeToFitText();

    properties->setText(new TextProperties(this));

    connect(properties->text(), &TextProperties::fontFamilyChanged, this, [this]()
            {
        m_font.setFamily(properties->text()->fontFamily());
        updateSizeToFitText();
        update(); });

    connect(properties->text(), &TextProperties::fontSizeChanged, this, [this]()
            {
        m_font.setPointSize(properties->text()->fontSize());
        updateSizeToFitText();
        update(); });

    connect(properties->text(), &TextProperties::boldChanged, this, [this]()
            {
        m_font.setBold(properties->text()->bold());
        updateSizeToFitText();
        update(); });

    connect(properties->text(), &TextProperties::italicChanged, this, [this]()
            {   
        m_font.setItalic(properties->text()->italic());
        updateSizeToFitText();
        update(); });

    connect(properties->text(), &TextProperties::fontWeightChanged, this, [this]()
            {
        m_font.setWeight(QFont::Weight(properties->text()->fontWeight()));
        updateSizeToFitText();
        update(); });

    connect(properties->base(), &BaseProperties::colorChanged, this, [this]()
            {
        m_color = properties->base()->color();
        update(); });

    properties->text()->setFontSize(32);
}

QString MText::richText() const { return m_richText; }

void MText::setRichText(const QString &txt)
{
    if (txt != m_richText)
    {
        m_richText = txt;
        emit richTextChanged();
        updateSizeToFitText();
        update();
    }
}

void MText::mousePressEvent(QMouseEvent *event)
{
    ClickableMobject::mousePressEvent(event);
}

void MText::mouseMoveEvent(QMouseEvent *event)
{
    ClickableMobject::mouseMoveEvent(event);
}

void MText::mouseReleaseEvent(QMouseEvent *event)
{
    ClickableMobject::mouseReleaseEvent(event);
}

void MText::mouseDoubleClickEvent(QMouseEvent *event)
{
    m_editing = true;
    setFocus(true);
    update();
}

void MText::keyPressEvent(QKeyEvent *event)
{
    if (m_editing)
    {
        switch (event->key())
        {
        case Qt::Key_Return:
        case Qt::Key_Enter:
            m_editing = false;
            setFocus(false);
            updateSizeToFitText(); // ← fixes width on Enter
            break;
        case Qt::Key_Backspace:
            m_richText.chop(1);
            break;
        default:
            if (!event->text().isEmpty())
                m_richText += event->text();
        }
    }
    updateSizeToFitText();
    update();
}

void MText::focusOutEvent(QFocusEvent *event)
{
    m_editing = false;
    update();
}

void MText::updateSizeToFitText()
{
    QTextDocument doc;
    doc.setDefaultFont(m_font);
    doc.setHtml(m_richText);
    doc.setTextWidth(-1); // Let it size freely

    if (m_editing)
    {
        setWidth(doc.idealWidth() + 20); // Gradually expands
        m_committedWidth = width();      // Remember current width
    }
    else if (m_committedWidth == 0)
    {
        setWidth(100);
    }
    else
    {
        setWidth(m_committedWidth); // Clamp to last width on Enter
    }
    setHeight(doc.size().height() + 20);
}

//-------------------------------------------
// Painting logic
//-------------------------------------------
QSGNode *MText::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
    QSGNode *root = oldNode;
    if (!root)
        root = new QSGNode();

    // Clear old nodes
    while (QSGNode *child = root->firstChild())
    {
        root->removeChildNode(child);
        delete child;
    }

    QRectF bounds(0, 0, width(), height());
    QImage img(bounds.size().toSize(), QImage::Format_ARGB32_Premultiplied);
    img.fill(Qt::transparent);

    QPainter p(&img);
    p.setRenderHint(QPainter::Antialiasing);
    p.setRenderHint(QPainter::TextAntialiasing);

    // Draw background if focused or editing
    if (m_editing || hasFocus())
    {
        QPen border(Qt::cyan, 2);
        p.setPen(border);
        p.setBrush(Qt::NoBrush);
        p.drawRoundedRect(bounds.adjusted(1, 1, -1, -1), 6, 6);
    }

    // Draw the text
    // Draw the text
    QTextDocument doc;
    doc.setDefaultFont(m_font);
    doc.setHtml(m_richText);

    // Force all text to m_color from properties, unless <span style="color:..."> is present
    QTextCursor cursor(&doc);
    cursor.select(QTextCursor::Document);
    QTextCharFormat fmt;
    fmt.setForeground(m_color);
    cursor.mergeCharFormat(fmt);

    doc.setTextWidth(width() - 10);
    p.translate(5, 5);
    doc.drawContents(&p);
    p.translate(-5, -5);

    p.end();

    auto *textureNode = new QSGSimpleTextureNode();
    textureNode->setTexture(window()->createTextureFromImage(img));
    textureNode->setRect(bounds);
    root->appendChildNode(textureNode);

    return root;
}
