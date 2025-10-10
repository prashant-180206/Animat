#include "command.h"
#include "../ValueTracker/trackermanager.h"
#include "../Scene.h"




QRegularExpression Command::regex() const
{
    return m_regex;
}

void Command::setRegex(const QRegularExpression &newRegex)
{
    m_regex = newRegex;
}

QString Command::line() const
{
    return m_line;
}
