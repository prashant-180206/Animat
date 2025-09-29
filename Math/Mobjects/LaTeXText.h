#ifndef LATEXTEXT_H
#define LATEXTEXT_H

#include "Text.h"
#include <QQmlEngine>
#include <QTextDocument>
#include <QTextOption>
#include <QAbstractTextDocumentLayout>

class LaTeXText : public Text
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(bool renderAsHtml READ renderAsHtml WRITE setRenderAsHtml NOTIFY renderAsHtmlChanged)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)

public:
    explicit LaTeXText(Scene *canvas = nullptr, QQuickItem *parent = nullptr);

    bool renderAsHtml() const { return m_renderAsHtml; }
    QColor backgroundColor() const { return m_backgroundColor; }

    void setRenderAsHtml(bool renderAsHtml);
    void setBackgroundColor(const QColor &color);

    // Override essential methods for LaTeX rendering
    virtual void setCenter(qreal x, qreal y) override;

signals:
    void renderAsHtmlChanged();
    void backgroundColorChanged();

protected:
    QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *) override;
    QRectF boundingRect() const override;

private:
    bool m_renderAsHtml;
    QColor m_backgroundColor;
    QSizeF m_renderedSize;
    QPixmap m_renderedPixmap;      // Cache for rendered math
    QTextDocument *m_textDocument; // Qt's rich text document for math rendering

    void renderMath();                                   // Render math expression using Qt's capabilities
    QString latexToMathML(const QString &latex) const;   // Convert LaTeX to MathML if needed
    QString latexToRichText(const QString &latex) const; // Convert LaTeX to Qt rich text format
    void setupTextDocument();                            // Configure text document for math
    void updateMathMetrics();                            // Update size metrics for math content
};

#endif // LATEXTEXT_H