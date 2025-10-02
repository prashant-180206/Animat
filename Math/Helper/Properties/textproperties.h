#ifndef TEXTPROPERTIES_H
#define TEXTPROPERTIES_H

#include <QObject>
#include <QColor>
#include <QString>
#include <QFont>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qqmlintegration.h>

class TextProperties : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString textValue READ textValue WRITE setTextValue NOTIFY textValueChanged)
    Q_PROPERTY(int fontSize READ fontSize WRITE setFontSize NOTIFY fontSizeChanged)
    Q_PROPERTY(int fontWeight READ fontWeight WRITE setFontWeight NOTIFY fontWeightChanged)
    Q_PROPERTY(QString fontFamily READ fontFamily WRITE setFontFamily NOTIFY fontFamilyChanged)
    Q_PROPERTY(bool bold READ bold WRITE setBold NOTIFY boldChanged)
    Q_PROPERTY(bool italic READ italic WRITE setItalic NOTIFY italicChanged)

public:
    explicit TextProperties(QObject *parent = nullptr);

    QString textValue() const { return m_textValue; }
    int fontSize() const { return m_fontSize; }
    int fontWeight() const { return m_fontWeight; }
    QString fontFamily() const { return m_fontFamily; }
    bool bold() const { return m_bold; }
    bool italic() const { return m_italic; }

    struct TextPropData
    {
        QString textValue;
        int fontSize = 0;
        int fontWeight = 0;
        QString fontFamily;
        bool bold = false;
        bool italic = false;
        QJsonDocument toJson() const
        {
            QJsonObject o;
            o["textValue"] = textValue;
            o["fontSize"] = fontSize;
            o["fontWeight"] = fontWeight;
            o["fontFamily"] = fontFamily;
            o["bold"] = bold;
            o["italic"] = italic;
            return QJsonDocument(o);
        }
        static TextPropData fromJSON(const QJsonObject &o)
        {
            TextPropData d;
            d.textValue = o["textValue"].toString();
            d.fontSize = o["fontSize"].toInt();
            d.fontWeight = o["fontWeight"].toInt();
            d.fontFamily = o["fontFamily"].toString();
            d.bold = o["bold"].toBool();
            d.italic = o["italic"].toBool();
            return d;
        }
    };

    TextPropData getData() const
    {
        TextPropData d;
        d.textValue = m_textValue;
        d.fontSize = m_fontSize;
        d.fontWeight = m_fontWeight;
        d.fontFamily = m_fontFamily;
        d.bold = m_bold;
        d.italic = m_italic;
        return d;
    }

    void setfromJSON(const QJsonObject &o)
    {
        TextPropData d = TextPropData::fromJSON(o);
        setTextValue(d.textValue);
        setFontSize(d.fontSize);
        setFontWeight(d.fontWeight);
        setFontFamily(d.fontFamily);
        setBold(d.bold);
        setItalic(d.italic);
    }

public slots:
    void setTextValue(const QString &textValue);
    void setFontSize(int fontSize);
    void setFontWeight(int fontWeight);
    void setFontFamily(const QString &fontFamily);
    void setBold(bool bold);
    void setItalic(bool italic);
signals:
    void textValueChanged();
    void fontSizeChanged();
    void fontWeightChanged();
    void fontFamilyChanged();
    void boldChanged();
    void italicChanged();

private:
    QString m_textValue;
    int m_fontSize;
    int m_fontWeight;
    QString m_fontFamily;
    bool m_bold;
    bool m_italic;
};

#endif // TEXTPROPERTIES_H
