#ifndef TRACKERCOMMAND_H
#define TRACKERCOMMAND_H

#include <QString>
#include <QPointF>
#include <QStringList>

class TrackerCommand
{
public:
    enum CommandType
    {
        VALUE_TRACKER,            // val name = 5;
        EXPRESSION_TRACKER,       // val name = sin(t*2);
        POINT_TRACKER,            // pval name = (1, 2);
        POINT_EXPRESSION_TRACKER, // pval name = (sin(t), cos(t));
        DYNAMIC_VALUE_TRACKER,    // dval name = [val1] + [val2];
        DYNAMIC_POINT_TRACKER,    // dpval name = ([val1], [val2]);
        CONNECTION                // connect(tracker, object.property);
    };

    // Constructors for different command types
    TrackerCommand(const QString &name, double value);
    TrackerCommand(const QString &name, const QString &expression, CommandType type);
    TrackerCommand(const QString &name, const QPointF &point);
    TrackerCommand(const QString &name, const QString &exprX, const QString &exprY, CommandType type);
    TrackerCommand(const QString &trackerName, const QString &objectName, const QString &propertyName);

    // Getters
    CommandType getType() const { return m_type; }
    QString getName() const { return m_name; }
    double getValue() const { return m_value; }
    QString getExpression() const { return m_expression; }
    QPointF getPoint() const { return m_point; }
    QString getExpressionX() const { return m_expressionX; }
    QString getExpressionY() const { return m_expressionY; }
    QString getObjectName() const { return m_objectName; }
    QString getPropertyName() const { return m_propertyName; }
    QStringList getDependencies() const { return m_dependencies; }

    // For dynamic trackers
    void setDependencies(const QStringList &deps) { m_dependencies = deps; }

    // Utility
    bool isValid() const { return m_valid; }
    QString toString() const;

private:
    CommandType m_type;
    QString m_name;
    double m_value = 0.0;
    QString m_expression;
    QPointF m_point;
    QString m_expressionX;
    QString m_expressionY;
    QString m_objectName;
    QString m_propertyName;
    QStringList m_dependencies;
    bool m_valid = true;
};

#endif // TRACKERCOMMAND_H