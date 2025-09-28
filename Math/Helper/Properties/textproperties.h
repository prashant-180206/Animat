#ifndef TEXTPROPERTIES_H
#define TEXTPROPERTIES_H

#include <QObject>
#include <QColor>
#include <QString>
#include <QFont>
#include <qqmlintegration.h>

class TextProperties : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString textValue READ textValue WRITE setTextValue NOTIFY textValueChanged)
    Q_PROPERTY(int fontSize READ fontSize WRITE setFontSize NOTIFY fontSizeChanged)
    Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor NOTIFY textColorChanged)
    Q_PROPERTY(int fontWeight READ fontWeight WRITE setFontWeight NOTIFY fontWeightChanged)
    Q_PROPERTY(QString fontFamily READ fontFamily WRITE setFontFamily NOTIFY fontFamilyChanged)
    Q_PROPERTY(bool bold READ bold WRITE setBold NOTIFY boldChanged)
    Q_PROPERTY(bool italic READ italic WRITE setItalic NOTIFY italicChanged)

public:
    explicit TextProperties(QObject *parent = nullptr);

    QString textValue() const { return m_textValue; }
    int fontSize() const { return m_fontSize; }
    QColor textColor() const { return m_textColor; }
    int fontWeight() const { return m_fontWeight; }
    QString fontFamily() const { return m_fontFamily; }
    bool bold() const { return m_bold; }
    bool italic() const { return m_italic; }

public slots:
    void setTextValue(const QString &textValue);
    void setFontSize(int fontSize);
    void setTextColor(const QColor &textColor);
    void setFontWeight(int fontWeight);
    void setFontFamily(const QString &fontFamily);
    void setBold(bool bold);
    void setItalic(bool italic);

signals:
    void textValueChanged();
    void fontSizeChanged();
    void textColorChanged();
    void fontWeightChanged();
    void fontFamilyChanged();
    void boldChanged();
    void italicChanged();

private:
    QString m_textValue;
    int m_fontSize;
    QColor m_textColor;
    int m_fontWeight;
    QString m_fontFamily;
    bool m_bold;
    bool m_italic;
};

#endif // TEXTPROPERTIES_H