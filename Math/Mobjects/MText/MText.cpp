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
    qInfo()<<"CONSTRUCTOR CALLED";
    setAcceptedMouseButtons(Qt::LeftButton);

    setFocus(false);

    properties->setText(new TextProperties(this));

    properties->text()->setFontSize(32);
    properties->text()->setTextValue("Type Here");
    properties->text()->setFontFamily("Arial");
    properties->text()->setFontWeight(600);
    properties->text()->setBold(false);
    properties->text()->setItalic(false);
    properties->base()->setType("Text");
    properties->base()->setName("Text");
    properties->base()->setColor(Qt::yellow);

    connect(properties->text(), &TextProperties::fontFamilyChanged, this, [this]()
            {

                updateSizeToFitText();
                update(); });

    connect(properties->text(), &TextProperties::fontSizeChanged, this, [this]()
            {

                updateSizeToFitText();
                update(); });

    connect(properties->text(), &TextProperties::boldChanged, this, [this]()
            {

                updateSizeToFitText();
                update(); });

    connect(properties->text(), &TextProperties::italicChanged, this, [this]()
            {   

                updateSizeToFitText();
                update(); });

    connect(properties->text(), &TextProperties::fontWeightChanged, this, [this]()
            {

                updateSizeToFitText();
                update(); });

    connect(properties->base(), &BaseProperties::colorChanged, this, [this]()
            {

                update(); });

    updateSizeToFitText();

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
            properties->text()->textValue().chop(1);
            break;
        default:
            if (!event->text().isEmpty()){
                properties->text()->setTextValue(properties->text()->textValue()+event->text());
            }

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
    doc.setDefaultFont(properties->text()->fontFamily());
    doc.setHtml(properties->text()->textValue());
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
    doc.setDefaultFont(properties->text()->fontFamily());
    doc.setHtml(properties->text()->textValue());

    // Force all text to m_color from properties, unless <span style="color:..."> is present
    QTextCursor cursor(&doc);
    cursor.select(QTextCursor::Document);
    QTextCharFormat fmt;
    fmt.setForeground(properties->base()->color());
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
