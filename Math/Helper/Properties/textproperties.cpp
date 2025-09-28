#include "textproperties.h"

TextProperties::TextProperties(QObject *parent)
    : QObject(parent),
      m_textValue("Hello World"),
      m_fontSize(24),
      m_textColor(Qt::white),
      m_fontWeight(QFont::Normal),
      m_fontFamily("Arial"),
      m_bold(false),
      m_italic(false)
{
}

void TextProperties::setTextValue(const QString &textValue)
{
    if (m_textValue != textValue)
    {
        m_textValue = textValue;
        emit textValueChanged();
    }
}

void TextProperties::setFontSize(int fontSize)
{
    if (m_fontSize != fontSize)
    {
        m_fontSize = fontSize;
        emit fontSizeChanged();
    }
}

void TextProperties::setTextColor(const QColor &textColor)
{
    if (m_textColor != textColor)
    {
        m_textColor = textColor;
        emit textColorChanged();
    }
}

void TextProperties::setFontWeight(int fontWeight)
{
    if (m_fontWeight != fontWeight)
    {
        m_fontWeight = fontWeight;
        emit fontWeightChanged();
    }
}

void TextProperties::setFontFamily(const QString &fontFamily)
{
    if (m_fontFamily != fontFamily)
    {
        m_fontFamily = fontFamily;
        emit fontFamilyChanged();
    }
}

void TextProperties::setBold(bool bold)
{
    if (m_bold != bold)
    {
        m_bold = bold;
        // Update font weight based on bold state
        setFontWeight(bold ? QFont::Bold : QFont::Normal);
        emit boldChanged();
    }
}

void TextProperties::setItalic(bool italic)
{
    if (m_italic != italic)
    {
        m_italic = italic;
        emit italicChanged();
    }
}