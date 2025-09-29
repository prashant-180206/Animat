#include "LaTeXText.h"
#include "Math/Scene.h"
#include <QSGImageNode>
#include <QSGTexture>
#include <QFont>
#include <QFontMetrics>
#include <QPainter>
#include <QTextDocument>
#include <QTextCursor>
#include <QTextCharFormat>
#include <QRegularExpression>
#include <QDebug>
#include <QAbstractTextDocumentLayout>

LaTeXText::LaTeXText(Scene *canvas, QQuickItem *parent)
    : Text(canvas, parent),
      m_renderAsHtml(true),
      m_backgroundColor(Qt::transparent),
      m_renderedSize(0, 0)
{
    setFlag(ItemHasContents, true);

    // Set up base properties
    properties->base()->setName("LaTeXText");
    properties->base()->setType("LaTeXText");

    // Initialize with default LaTeX expression using inherited property
    setText("E = mc^2"); // This will set the textValue property

    // Initial rendering
    renderMath();
    updateMathMetrics();

    // Connect property changes for LaTeX-specific properties only
    connect(this, &LaTeXText::renderAsHtmlChanged, this, [this]()
            {
        renderMath();
        updateMathMetrics();
        update(); });

    connect(this, &LaTeXText::backgroundColorChanged, this, [this]()
            {
        renderMath();
        update(); });

    // Override parent's text property connections to trigger LaTeX re-rendering
    // Connect to textValueChanged to re-render LaTeX when the expression changes
    connect(properties->text(), &TextProperties::textValueChanged, this, [this]()
            {
        renderMath();
        updateMathMetrics(); });

    // Override parent's text property connections to trigger LaTeX re-rendering
    // These will be called AFTER the parent class connections
    connect(properties->text(), &TextProperties::fontSizeChanged, this, [this]()
            {
        renderMath();
        updateMathMetrics(); });

    connect(properties->text(), &TextProperties::fontWeightChanged, this, [this]()
            {
        renderMath();
        updateMathMetrics(); });

    connect(properties->text(), &TextProperties::boldChanged, this, [this]()
            {
        renderMath();
        updateMathMetrics(); });

    connect(properties->text(), &TextProperties::italicChanged, this, [this]()
            {
        renderMath();
        updateMathMetrics(); });

    connect(properties->text(), &TextProperties::fontFamilyChanged, this, [this]()
            {
        renderMath();
        updateMathMetrics(); });

    connect(properties->base(), &BaseProperties::colorChanged, this, [this]()
            {
                renderMath(); // Re-render with new color
            });
}

void LaTeXText::setRenderAsHtml(bool renderAsHtml)
{
    if (m_renderAsHtml == renderAsHtml)
        return;

    m_renderAsHtml = renderAsHtml;
    emit renderAsHtmlChanged();
}

void LaTeXText::setBackgroundColor(const QColor &color)
{
    if (m_backgroundColor == color)
        return;

    m_backgroundColor = color;
    emit backgroundColorChanged();
}

void LaTeXText::setCenter(qreal x, qreal y)
{
    QPointF newCenter(x, y);
    properties->base()->setPos(newCenter);

    QPointF canvasCenter = getcanvas()->p2c(newCenter);
    setX(canvasCenter.x() - m_renderedSize.width() / 2);
    setY(canvasCenter.y() - m_renderedSize.height() / 2);
    setZ(50);
}

void LaTeXText::renderMath()
{
    // Use inherited textValue property
    QString latexExpression = properties->text() ? properties->text()->textValue() : "E = mc^2";

    // Convert LaTeX to Unicode text
    QString unicodeText = latexToUnicodeText(latexExpression);

    // Set up font using inherited properties
    QString fontFamily = properties->text() && !properties->text()->fontFamily().isEmpty()
                             ? properties->text()->fontFamily()
                             : "Cambria Math";
    int fontSize = properties->text() ? properties->text()->fontSize() : 24;
    bool bold = properties->text() ? properties->text()->bold() : false;
    bool italic = properties->text() ? properties->text()->italic() : false;

    QFont mathFont(fontFamily, fontSize);

    // Try math fonts if current font doesn't support math symbols
    if (!QFontInfo(mathFont).exactMatch())
    {
        QStringList mathFonts = {"Cambria Math", "STIX Two Math", "Latin Modern Math", "Arial Unicode MS"};
        for (const QString &font : mathFonts)
        {
            mathFont.setFamily(font);
            if (QFontInfo(mathFont).exactMatch())
            {
                break;
            }
        }
    }

    mathFont.setBold(bold);
    mathFont.setItalic(italic);

    // Calculate text size
    QFontMetrics metrics(mathFont);
    QRect textRect = metrics.boundingRect(unicodeText);

    // Add padding
    int padding = 10;
    QSize pixmapSize = textRect.size() + QSize(padding * 2, padding * 2);

    // Handle high DPI
    qreal devicePixelRatio = this->window() ? this->window()->devicePixelRatio() : 1.0;
    QSize actualSize = pixmapSize * devicePixelRatio;

    m_renderedPixmap = QPixmap(actualSize);
    m_renderedPixmap.setDevicePixelRatio(devicePixelRatio);
    m_renderedPixmap.fill(m_backgroundColor);

    QPainter painter(&m_renderedPixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);

    painter.scale(devicePixelRatio, devicePixelRatio);
    painter.setFont(mathFont);
    painter.setPen(properties->base() ? properties->base()->color() : Qt::white);

    // Draw text centered
    QRect drawRect(0, 0, pixmapSize.width(), pixmapSize.height());
    painter.drawText(drawRect, Qt::AlignCenter, unicodeText);

    m_renderedSize = pixmapSize;
}

QString LaTeXText::latexToMathML(const QString &latex) const
{
    // Basic MathML support - could be expanded with a proper LaTeX to MathML converter
    // For now, return empty to use rich text fallback
    Q_UNUSED(latex)
    return QString();
}

QString LaTeXText::latexToUnicodeText(const QString &latex) const
{
    QString result = latex;

    // Basic LaTeX to Unicode math symbol conversions
    QHash<QString, QString> conversions = {
        // Greek letters
        {"\\alpha", "α"},
        {"\\beta", "β"},
        {"\\gamma", "γ"},
        {"\\delta", "δ"},
        {"\\epsilon", "ε"},
        {"\\zeta", "ζ"},
        {"\\eta", "η"},
        {"\\theta", "θ"},
        {"\\iota", "ι"},
        {"\\kappa", "κ"},
        {"\\lambda", "λ"},
        {"\\mu", "μ"},
        {"\\nu", "ν"},
        {"\\xi", "ξ"},
        {"\\pi", "π"},
        {"\\rho", "ρ"},
        {"\\sigma", "σ"},
        {"\\tau", "τ"},
        {"\\upsilon", "υ"},
        {"\\phi", "φ"},
        {"\\chi", "χ"},
        {"\\psi", "ψ"},
        {"\\omega", "ω"},

        // Capital Greek letters
        {"\\Alpha", "Α"},
        {"\\Beta", "Β"},
        {"\\Gamma", "Γ"},
        {"\\Delta", "Δ"},
        {"\\Epsilon", "Ε"},
        {"\\Zeta", "Ζ"},
        {"\\Eta", "Η"},
        {"\\Theta", "Θ"},
        {"\\Iota", "Ι"},
        {"\\Kappa", "Κ"},
        {"\\Lambda", "Λ"},
        {"\\Mu", "Μ"},
        {"\\Nu", "Ν"},
        {"\\Xi", "Ξ"},
        {"\\Pi", "Π"},
        {"\\Rho", "Ρ"},
        {"\\Sigma", "Σ"},
        {"\\Tau", "Τ"},
        {"\\Upsilon", "Υ"},
        {"\\Phi", "Φ"},
        {"\\Chi", "Χ"},
        {"\\Psi", "Ψ"},
        {"\\Omega", "Ω"},

        // Mathematical symbols
        {"\\infty", "∞"},
        {"\\partial", "∂"},
        {"\\nabla", "∇"},
        {"\\pm", "±"},
        {"\\times", "×"},
        {"\\div", "÷"},
        {"\\cdot", "·"},
        {"\\leq", "≤"},
        {"\\geq", "≥"},
        {"\\neq", "≠"},
        {"\\approx", "≈"},
        {"\\equiv", "≡"},
        {"\\in", "∈"},
        {"\\notin", "∉"},
        {"\\subset", "⊂"},
        {"\\supset", "⊃"},
        {"\\cap", "∩"},
        {"\\cup", "∪"},
        {"\\int", "∫"},
        {"\\sum", "∑"},
        {"\\prod", "∏"},
        {"\\sqrt", "√"},

        // Arrows
        {"\\rightarrow", "→"},
        {"\\leftarrow", "←"},
        {"\\leftrightarrow", "↔"},
        {"\\Rightarrow", "⇒"},
        {"\\Leftarrow", "⇐"},
        {"\\Leftrightarrow", "⇔"}};

    // Apply conversions
    for (auto it = conversions.begin(); it != conversions.end(); ++it)
    {
        result.replace(it.key(), it.value());
    }

    // Handle simple superscripts ^{...} or ^char
    QRegularExpression supRe(R"(\^(\{[^}]*\}|[a-zA-Z0-9]))");
    QRegularExpressionMatchIterator i = supRe.globalMatch(result);
    while (i.hasNext())
    {
        QRegularExpressionMatch match = i.next();
        QString captured = match.captured(1);
        if (captured.startsWith("{") && captured.endsWith("}"))
        {
            captured = captured.mid(1, captured.length() - 2);
        }

        // Convert to Unicode superscript
        QString superscript = captured;
        superscript.replace("0", "⁰").replace("1", "¹").replace("2", "²").replace("3", "³").replace("4", "⁴").replace("5", "⁵").replace("6", "⁶").replace("7", "⁷").replace("8", "⁸").replace("9", "⁹").replace("+", "⁺").replace("-", "⁻");

        result.replace(match.captured(0), superscript);
    }

    // Handle simple subscripts _{...} or _char
    QRegularExpression subRe(R"(_(\{[^}]*\}|[a-zA-Z0-9]))");
    i = subRe.globalMatch(result);
    while (i.hasNext())
    {
        QRegularExpressionMatch match = i.next();
        QString captured = match.captured(1);
        if (captured.startsWith("{") && captured.endsWith("}"))
        {
            captured = captured.mid(1, captured.length() - 2);
        }

        // Convert to Unicode subscript
        QString subscript = captured;
        subscript.replace("0", "₀").replace("1", "₁").replace("2", "₂").replace("3", "₃").replace("4", "₄").replace("5", "₅").replace("6", "₆").replace("7", "₇").replace("8", "₈").replace("9", "₉").replace("+", "₊").replace("-", "₋");

        result.replace(match.captured(0), subscript);
    }

    return result;
}
{
    QString result = latex;

    // Convert LaTeX commands to HTML/rich text equivalents
    QHash<QString, QString> conversions = {
        // Greek letters (using HTML entities where possible)
        {"\\alpha", "&alpha;"},
        {"\\beta", "&beta;"},
        {"\\gamma", "&gamma;"},
        {"\\delta", "&delta;"},
        {"\\epsilon", "&epsilon;"},
        {"\\zeta", "&zeta;"},
        {"\\eta", "&eta;"},
        {"\\theta", "&theta;"},
        {"\\iota", "&iota;"},
        {"\\kappa", "&kappa;"},
        {"\\lambda", "&lambda;"},
        {"\\mu", "&mu;"},
        {"\\nu", "&nu;"},
        {"\\xi", "&xi;"},
        {"\\pi", "&pi;"},
        {"\\rho", "&rho;"},
        {"\\sigma", "&sigma;"},
        {"\\tau", "&tau;"},
        {"\\upsilon", "&upsilon;"},
        {"\\phi", "&phi;"},
        {"\\chi", "&chi;"},
        {"\\psi", "&psi;"},
        {"\\omega", "&omega;"},

        // Capital Greek letters
        {"\\Alpha", "&Alpha;"},
        {"\\Beta", "&Beta;"},
        {"\\Gamma", "&Gamma;"},
        {"\\Delta", "&Delta;"},
        {"\\Epsilon", "&Epsilon;"},
        {"\\Zeta", "&Zeta;"},
        {"\\Eta", "&Eta;"},
        {"\\Theta", "&Theta;"},
        {"\\Iota", "&Iota;"},
        {"\\Kappa", "&Kappa;"},
        {"\\Lambda", "&Lambda;"},
        {"\\Mu", "&Mu;"},
        {"\\Nu", "&Nu;"},
        {"\\Xi", "&Xi;"},
        {"\\Pi", "&Pi;"},
        {"\\Rho", "&Rho;"},
        {"\\Sigma", "&Sigma;"},
        {"\\Tau", "&Tau;"},
        {"\\Upsilon", "&Upsilon;"},
        {"\\Phi", "&Phi;"},
        {"\\Chi", "&Chi;"},
        {"\\Psi", "&Psi;"},
        {"\\Omega", "&Omega;"},

        // Mathematical symbols
        {"\\infty", "&infin;"},
        {"\\partial", "&part;"},
        {"\\nabla", "&nabla;"},
        {"\\pm", "&plusmn;"},
        {"\\times", "&times;"},
        {"\\div", "&divide;"},
        {"\\leq", "&le;"},
        {"\\geq", "&ge;"},
        {"\\neq", "&ne;"},
        {"\\approx", "&asymp;"},
        {"\\equiv", "&equiv;"},
        {"\\in", "&isin;"},
        {"\\notin", "&notin;"},
        {"\\subset", "&sub;"},
        {"\\supset", "&sup;"},
        {"\\cap", "&cap;"},
        {"\\cup", "&cup;"},
        {"\\int", "&int;"},
        {"\\sum", "&sum;"},
        {"\\prod", "&prod;"},
        {"\\sqrt", "&radic;"},

        // Arrows
        {"\\rightarrow", "&rarr;"},
        {"\\leftarrow", "&larr;"},
        {"\\leftrightarrow", "&harr;"},
        {"\\Rightarrow", "&rArr;"},
        {"\\Leftarrow", "&lArr;"},
        {"\\Leftrightarrow", "&hArr;"}};

    // Apply basic conversions
    for (auto it = conversions.begin(); it != conversions.end(); ++it)
    {
        result.replace(it.key(), it.value());
    }

    // Handle fractions \frac{num}{den}
    QRegularExpression fracRe(R"(\\frac\{([^}]+)\}\{([^}]+)\})");
    result.replace(fracRe, R"(<table style="display:inline-table; vertical-align: middle; border-collapse: collapse; margin: 0 2px;"><tr><td style="text-align: center; border-bottom: 1px solid; padding: 1px;">\1</td></tr><tr><td style="text-align: center; padding: 1px;">\2</td></tr></table>)");

    // Handle matrices \begin{matrix}...\end{matrix} or \begin{pmatrix}...\end{pmatrix}
    QRegularExpression matrixRe(R"(\\begin\{(p?matrix)\}([^\\]+)\\end\{\1\})");
    QRegularExpressionMatchIterator matrixIterator = matrixRe.globalMatch(result);
    while (matrixIterator.hasNext())
    {
        QRegularExpressionMatch match = matrixIterator.next();
        QString matrixType = match.captured(1);
        QString matrixContent = match.captured(2).trimmed();

        // Split by rows (\\)
        QStringList rows = matrixContent.split("\\\\", Qt::SkipEmptyParts);

        QString tableStyle = "display:inline-table; vertical-align: middle; border-collapse: separate; border-spacing: 8px 4px; margin: 0 4px;";
        if (matrixType == "pmatrix")
        {
            tableStyle += " border-left: 1px solid; border-right: 1px solid; border-radius: 8px;"; // Parentheses effect
        }

        QString tableHtml = QString("<table style=\"%1\"><tbody>").arg(tableStyle);

        for (const QString &row : rows)
        {
            QStringList cols = row.split("&", Qt::SkipEmptyParts);
            tableHtml += "<tr>";
            for (const QString &col : cols)
            {
                tableHtml += QString("<td style=\"text-align: center; padding: 2px 6px;\">%1</td>").arg(col.trimmed());
            }
            tableHtml += "</tr>";
        }

        tableHtml += "</tbody></table>";
        result.replace(match.captured(0), tableHtml);
    }

    // Handle vectors \begin{bmatrix}...\end{bmatrix}
    QRegularExpression bmatrixRe(R"(\\begin\{bmatrix\}([^\\]+)\\end\{bmatrix\})");
    QRegularExpressionMatchIterator bmatrixIterator = bmatrixRe.globalMatch(result);
    while (bmatrixIterator.hasNext())
    {
        QRegularExpressionMatch match = bmatrixIterator.next();
        QString matrixContent = match.captured(1).trimmed();

        QStringList rows = matrixContent.split("\\\\", Qt::SkipEmptyParts);
        QString tableHtml = "<table style=\"display:inline-table; vertical-align: middle; border-collapse: separate; border-spacing: 8px 4px; margin: 0 4px; border-left: 2px solid; border-right: 2px solid;\"><tbody>";

        for (const QString &row : rows)
        {
            QStringList cols = row.split("&", Qt::SkipEmptyParts);
            tableHtml += "<tr>";
            for (const QString &col : cols)
            {
                tableHtml += QString("<td style=\"text-align: center; padding: 2px 6px;\">%1</td>").arg(col.trimmed());
            }
            tableHtml += "</tr>";
        }

        tableHtml += "</tbody></table>";
        result.replace(match.captured(0), tableHtml);
    }

    // Handle superscripts ^{...} or ^char
    QRegularExpression supRe(R"(\^(\{[^}]+\}|[a-zA-Z0-9]))");
    result.replace(supRe, R"(<sup>\1</sup>)");
    result.replace("{", "").replace("}", ""); // Remove remaining braces from superscripts

    // Handle subscripts _{...} or _char
    QRegularExpression subRe(R"(_(\{[^}]+\}|[a-zA-Z0-9]))");
    result.replace(subRe, R"(<sub>\1</sub>)");

    // Handle square root \\sqrt{...}
    QRegularExpression sqrtRe(R"(\\sqrt\{([^}]+)\})");
    result.replace(sqrtRe, R"(&radic;<span style="text-decoration: overline;">\1</span>)");

    // Wrap in HTML with proper styling using inherited properties
    QString fontFamily = properties->text() && !properties->text()->fontFamily().isEmpty()
                             ? properties->text()->fontFamily()
                             : "Cambria Math, STIX Two Math, Times New Roman";
    int fontSize = properties->text() ? properties->text()->fontSize() : 24;
    bool bold = properties->text() ? properties->text()->bold() : false;
    bool italic = properties->text() ? properties->text()->italic() : false;
    QColor textColor = properties->base() ? properties->base()->color() : Qt::white;

    QString fontWeight = bold ? "bold" : "normal";
    QString fontStyle = italic ? "italic" : "normal";

    QString html = QString(R"(<html><body style="color: %1; font-family: %2; font-size: %3pt; font-weight: %4; font-style: %5;">%6</body></html>)")
                       .arg(textColor.name())
                       .arg(fontFamily)
                       .arg(fontSize)
                       .arg(fontWeight)
                       .arg(fontStyle)
                       .arg(result);

    return html;
}

void LaTeXText::updateMathMetrics()
{
    m_renderedSize = m_renderedPixmap.size();

    // Update the item's implicit size
    properties->base()->setSize({m_renderedSize.height(), m_renderedSize.width()});
    setImplicitWidth(m_renderedSize.width());
    setImplicitHeight(m_renderedSize.height());
}

QRectF LaTeXText::boundingRect() const
{
    return QRectF(-m_renderedSize.width() / 2, -m_renderedSize.height() / 2,
                  m_renderedSize.width(), m_renderedSize.height());
}

QSGNode *LaTeXText::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
    QSGImageNode *node = static_cast<QSGImageNode *>(oldNode);
    if (!node)
    {
        node = this->window()->createImageNode();
    }

    if (m_renderedPixmap.isNull())
    {
        return node;
    }

    // Create texture from the rendered pixmap
    QSGTexture *texture = this->window()->createTextureFromImage(m_renderedPixmap.toImage());
    node->setTexture(texture);
    node->setOwnsTexture(true);

    // Center the image at the origin
    QRectF rect(-m_renderedSize.width() / 2, -m_renderedSize.height() / 2,
                m_renderedSize.width(), m_renderedSize.height());
    node->setRect(rect);

    return node;
}