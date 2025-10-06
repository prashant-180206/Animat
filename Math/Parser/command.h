#ifndef COMMAND_H
#define COMMAND_H

#include <QString>
#include <QRegularExpression>

class TrackerManager;
class Scene;

class Command
{
public:

    explicit Command(QString l):m_line(l){}
    // Execute the command with captured groups
    virtual void execute(Scene * c,TrackerManager * tm) const{ }
    QRegularExpression regex() const;

    void setRegex(const QRegularExpression &newRegex);

    QString line() const;

private:

    QRegularExpression m_regex;
    QString m_line;

};

#endif // COMMAND_H
