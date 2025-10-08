#include "MText.h"
#include <QSGSimpleRectNode>
#include <QSGSimpleTextureNode>
#include <QPainter>
#include <QImage>
#include <QTextCursor>
#include <QTextDocument>
#include <QEvent>

MText::MText(Scene *canvas, QQuickItem *parent)
    : ClickableMobject(canvas, parent)
{
    setAcceptedMouseButtons(Qt::LeftButton);
    setAcceptHoverEvents(true);
    setFlag(ItemIsFocusScope, true);
    setFocus(false);

    m_font.setPointSize(16);
    m_font.setFamily("Segoe UI");

    connect(&m_panelHideTimer, &QTimer::timeout, this, [this]
            {
        m_showPanel = false;
        emit showPanelChanged();
        update(); });
    m_panelHideTimer.setInterval(300);
    m_panelHideTimer.setSingleShot(true);

    setRichText("<b>TYPE HERE</b>");
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

//-------------------------------------------
// Mouse & keyboard events
//-------------------------------------------
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

void MText::hoverEnterEvent(QHoverEvent *event)
{
    m_showPanel = true;
    emit showPanelChanged();
    update();
}

void MText::hoverLeaveEvent(QHoverEvent *event)
{
    m_panelHideTimer.start();
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
    doc.adjustSize();

    setWidth(doc.idealWidth() + 20); // small padding
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
    QTextDocument doc;
    doc.setDefaultFont(m_font);
    doc.setHtml(m_richText);
    doc.setTextWidth(width() - 10);
    p.translate(5, 5);
    doc.drawContents(&p);
    p.translate(-5, -5);

    // Draw format panel if visible
    if (m_showPanel)
    {
        drawFormattingPanel(p);
    }

    p.end();

    auto *textureNode = new QSGSimpleTextureNode();
    textureNode->setTexture(window()->createTextureFromImage(img));
    textureNode->setRect(bounds);
    root->appendChildNode(textureNode);

    return root;
}

//-------------------------------------------
// Formatting panel rendering
//-------------------------------------------
void MText::drawFormattingPanel(QPainter &p)
{
    // const int w = 140, h = 36;
    // QRectF panelRect(width() - w - 5, 5, w, h);

    // // panel background
    // p.setPen(Qt::NoPen);
    // p.setBrush(QColor(35, 35, 35, 230));
    // p.drawRoundedRect(panelRect, 6, 6);

    // p.setPen(Qt::white);
    // QFont iconFont = m_font;
    // iconFont.setBold(true);
    // p.setFont(iconFont);

    // // Draw options: Bold, Italic, Bullet (just icons for now)
    // p.drawText(panelRect.adjusted(10, 0, -90, 0), Qt::AlignVCenter, "B");
    // QFont italicFont = iconFont;
    // italicFont.setItalic(true);
    // p.setFont(italicFont);
    // p.drawText(panelRect.adjusted(35, 0, -60, 0), Qt::AlignVCenter, "I");

    // p.setFont(m_font);
    // p.drawText(panelRect.adjusted(65, 0, -30, 0), Qt::AlignVCenter, "•");

    // // small “panel” label (optional)
    // p.setPen(QColor(180, 180, 180));
    // p.setFont(QFont("Segoe UI", 9));
    // p.drawText(panelRect.adjusted(95, 0, -5, 0), Qt::AlignVCenter | Qt::AlignRight, "⋯");
}
