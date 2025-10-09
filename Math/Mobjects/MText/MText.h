#pragma once
#include "ClickableMobject.h"
#include <QTextDocument>
#include <QTimer>
#include <QFont>

class MText : public ClickableMobject {
    Q_OBJECT
    Q_PROPERTY(QString richText READ richText WRITE setRichText NOTIFY richTextChanged)

public:
    explicit MText(Scene *canvas, QQuickItem *parent = nullptr);

    QString richText() const;
    void setRichText(const QString &text);

signals:
    void richTextChanged();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;
    QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *data) override;

private:
    void updateSizeToFitText();
private:
    qreal m_committedWidth = 0;
    QString m_richText;
    bool m_editing = false;
    QFont m_font;
    QColor m_color = Qt::white;
};
