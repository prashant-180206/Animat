#pragma once
#include "ClickableMobject.h"
#include <QTextDocument>
#include <QTimer>
#include <QFont>

class MText : public ClickableMobject {
    Q_OBJECT

public:
    explicit MText(Scene *canvas, QQuickItem *parent = nullptr);

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

    bool m_editing = false;
    qreal m_committedWidth = 80;

};
