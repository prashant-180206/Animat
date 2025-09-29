#include <QRegularExpression>
#include <QDebug>
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    // Test the new regex pattern
    QRegularExpression reValExpr(R"(\s*val\s+(\w+)\s*=\s*([^;]+?)\s*;\s*)");

    // Test cases
    QStringList testCases = {
        "val x = 4;",
        " val x = 4; ",
        "val  x  =  4  ;",
        "\tval\tx\t=\t4\t;",
        "val y = 5;",
        "val z = sin(t);",
        "  val  name  =  expression  ;  "};

    for (const QString &test : testCases)
    {
        qDebug() << "Testing:" << test;
        QRegularExpressionMatch match = reValExpr.match(test);
        if (match.hasMatch())
        {
            qDebug() << "  MATCH!";
            qDebug() << "  Full match:" << match.captured(0);
            qDebug() << "  Name (group 1):" << match.captured(1);
            qDebug() << "  Expression (group 2):" << match.captured(2);
        }
        else
        {
            qDebug() << "  NO MATCH";
        }
        qDebug() << "";
    }

    return 0;
}