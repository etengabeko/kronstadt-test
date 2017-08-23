#include <QCoreApplication>
#include <QDebug>
#include <QString>

#include <bignumber.h>

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    QString first;
    QString second;

    if (argc > 1)
    {
        first = app.arguments()[1];
        second = (argc > 2) ? app.arguments()[2]
                            : first;
    }
    else
    {
        qCritical().noquote() << app.tr("Test for bignumber library. Return value of (number#1 + number#2).\n"
                                        "Usage: %1 number#1 number#2\n"
                                        "Arguments:\n"
                                        "number#1    First  summand.\n"
                                        "number#2    Second summand (default = number#1).")
                                 .arg(app.arguments()[0]);
        return EXIT_FAILURE;
    }

    try
    {
        MathTools::BigNumber firstSummand(first);
        MathTools::BigNumber secondSummand(second);

        MathTools::BigNumber sum = firstSummand + secondSummand;

        QString strSum = sum.toString();

        qInfo().noquote() << strSum;
    }
    catch (MathTools::Exception& e)
    {
        qCritical().noquote() << e.what();
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
