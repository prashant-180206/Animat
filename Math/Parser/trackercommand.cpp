#include "trackercommand.h"

// Value tracker constructor
TrackerCommand::TrackerCommand(const QString &name, double value)
    : m_type(VALUE_TRACKER), m_name(name), m_value(value)
{
    // Validate that name is not empty
    if (name.isEmpty())
    {
        m_valid = false;
    }
}

// Expression tracker constructor
TrackerCommand::TrackerCommand(const QString &name, const QString &expression, CommandType type)
    : m_type(type), m_name(name), m_expression(expression)
{
    if (type != EXPRESSION_TRACKER && type != DYNAMIC_VALUE_TRACKER)
    {
        m_valid = false;
    }
    // Validate that name is not empty
    if (name.isEmpty())
    {
        m_valid = false;
    }
}

// Point tracker constructor
TrackerCommand::TrackerCommand(const QString &name, const QPointF &point)
    : m_type(POINT_TRACKER), m_name(name), m_point(point)
{
    // Validate that name is not empty
    if (name.isEmpty())
    {
        m_valid = false;
    }
}

// Point expression tracker constructor
TrackerCommand::TrackerCommand(const QString &name, const QString &exprX, const QString &exprY, CommandType type)
    : m_type(type), m_name(name), m_expressionX(exprX), m_expressionY(exprY)
{
    if (type != POINT_EXPRESSION_TRACKER && type != DYNAMIC_POINT_TRACKER)
    {
        m_valid = false;
    }
    // Validate that name is not empty
    if (name.isEmpty())
    {
        m_valid = false;
    }
}

// Connection constructor
TrackerCommand::TrackerCommand(const QString &trackerName, const QString &objectName, const QString &propertyName)
    : m_type(CONNECTION), m_name(trackerName), m_objectName(objectName), m_propertyName(propertyName)
{
    // Validate that names are not empty
    if (trackerName.isEmpty() || objectName.isEmpty() || propertyName.isEmpty())
    {
        m_valid = false;
    }
}

QString TrackerCommand::toString() const
{
    switch (m_type)
    {
    case VALUE_TRACKER:
        return QString("val %1 = %2;").arg(m_name).arg(m_value);
    case EXPRESSION_TRACKER:
        return QString("val %1 = %2;").arg(m_name, m_expression);
    case POINT_TRACKER:
        return QString("pval %1 = (%2, %3);").arg(m_name).arg(m_point.x()).arg(m_point.y());
    case POINT_EXPRESSION_TRACKER:
        return QString("pval %1 = (%2, %3);").arg(m_name, m_expressionX, m_expressionY);
    case DYNAMIC_VALUE_TRACKER:
        return QString("dval %1 = %2;").arg(m_name, m_expression);
    case DYNAMIC_POINT_TRACKER:
        return QString("dpval %1 = (%2, %3);").arg(m_name, m_expressionX, m_expressionY);
    case CONNECTION:
        return QString("connect(%1, %2.%3);").arg(m_name, m_objectName, m_propertyName);
    }
    return "Invalid Command";
}