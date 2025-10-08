#pragma once
#include "ClickableMobject.h"
#include <QTextDocument>
#include <QTimer>
#include <QFont>

class MText : public ClickableMobject {
    Q_OBJECT
    Q_PROPERTY(QString richText READ richText WRITE setRichText NOTIFY richTextChanged)
    Q_PROPERTY(bool showPanel READ showPanel NOTIFY showPanelChanged)

public:
    explicit MText(Scene *canvas, QQuickItem *parent = nullptr);

    QString richText() const;
    void setRichText(const QString &text);

    bool showPanel() const { return m_showPanel; }

signals:
    void richTextChanged();
    void showPanelChanged();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void hoverEnterEvent(QHoverEvent *event) override;
    void hoverLeaveEvent(QHoverEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;
    QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *data) override;

private:
    void updateSizeToFitText();
    void drawFormattingPanel(QPainter &p);

private:
    QString m_richText;
    bool m_showPanel = false;
    QTimer m_panelHideTimer;
    bool m_editing = false;
    QFont m_font;
};
