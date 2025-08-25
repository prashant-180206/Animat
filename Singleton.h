#ifndef CONSTANTSSINGLETON_H
#define CONSTANTSSINGLETON_H

#include <QObject>
#include <QColor>
#include <qqmlintegration.h>
#include "Constants.h"  // Include your original C++ constants

class ConstantsSingleton : public QObject
{
    Q_OBJECT
    QML_SINGLETON

    // Sizes
    Q_PROPERTY(int canvasWidth READ canvasWidth CONSTANT)
    Q_PROPERTY(int canvasHeight READ canvasHeight CONSTANT)

    // Colors
    Q_PROPERTY(QColor canvasBg READ canvasBg CONSTANT)
    Q_PROPERTY(QColor blueA READ blueA CONSTANT)
    Q_PROPERTY(QColor blueB READ blueB CONSTANT)
    Q_PROPERTY(QColor blueC READ blueC CONSTANT)

    Q_PROPERTY(QColor textLight READ textLight CONSTANT)
    Q_PROPERTY(QColor iconLight READ iconLight CONSTANT)
    Q_PROPERTY(QColor accentLight READ accentLight CONSTANT)
    Q_PROPERTY(QColor highlightLight READ highlightLight CONSTANT)

    Q_PROPERTY(QColor darkBlack READ darkBlack CONSTANT)
    Q_PROPERTY(QColor darkGrayA READ darkGrayA CONSTANT)
    Q_PROPERTY(QColor darkGrayB READ darkGrayB CONSTANT)
    Q_PROPERTY(QColor darkGrayC READ darkGrayC CONSTANT)

public:
    explicit ConstantsSingleton(QObject* parent = nullptr) : QObject(parent) {}

    // Sizes
    int canvasWidth() const { return DEF_CANVAS_WIDTH; }
    int canvasHeight() const { return DEF_CANVAS_HEIGHT; }

    // Colors
    QColor canvasBg() const { return DEF_CANVAS_BG; }
    QColor blueA() const { return BLUE_A; }
    QColor blueB() const { return BLUE_B; }
    QColor blueC() const { return BLUE_C; }

    QColor textLight() const { return TEXT_LIGHT; }
    QColor iconLight() const { return ICON_LIGHT; }
    QColor accentLight() const { return ACCENT_LIGHT; }
    QColor highlightLight() const { return HIGHLIGHT_LIGHT; }

    QColor darkBlack() const { return DARK_BLACK; }
    QColor darkGrayA() const { return DARK_GRAY_A; }
    QColor darkGrayB() const { return DARK_GRAY_B; }
    QColor darkGrayC() const { return DARK_GRAY_C; }
};

#endif // CONSTANTSSINGLETON_H
