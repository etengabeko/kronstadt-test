#include <QtTest>

#include <bignumber.h>

namespace
{

QString errorString() { return "Error"; }

}

class BigNumberTest : public QObject
{
    Q_OBJECT

private slots:
    void slotSummatoryTest_data()
    {
        QTest::addColumn<QString>("num1");
        QTest::addColumn<QString>("num2");
        QTest::addColumn<QString>("result");

        QTest::newRow("a + b") << "a" << "b" << ::errorString();
        QTest::newRow("a + 1234567890") << "a" << "1234567890" << ::errorString();
        QTest::newRow("1234567890 + -b") << "1234567890" << "-b" << ::errorString();

        QTest::newRow("0 + 0") << "0" << "0" << "0";
        QTest::newRow("-0 + 0") << "-0" << "0" << "0";
        QTest::newRow("00 + 00") << "00" << "00" << "0";
        QTest::newRow("0001 + 0002") << "0001" << "0002" << "3";

        QTest::newRow("0 + 1234") << "0" << "1234" << "1234";
        QTest::newRow("0 + -1234") << "0" << "-1234" << "-1234";
        QTest::newRow("2345 + 0") << "2345" << "0" << "2345";
        QTest::newRow("-2345 + 0") << "-2345" << "0" << "-2345";

        QTest::newRow("123456789 + -123456789") << "123456789" << "-123456789" << "0";
        QTest::newRow("123456789 + 987654321") << "123456789" << "987654321" << "1111111110";

        QTest::newRow("100 + 1") << "100" << "1" << "101";
        QTest::newRow("100 + -1") << "100" << "-1" << "99";
        QTest::newRow("1 + -100") << "1" << "-100" << "-99";
        QTest::newRow("-1 + -100") << "-1" << "-100" << "-101";
        QTest::newRow("1000 + -999") << "1000" << "-999" << "1";

        QTest::newRow("12345 + 23456") << "12345" << "23456" << "35801";
        QTest::newRow("-5432 + -9876") << "-5432" << "-9876" << "-15308";

        QTest::newRow("123456789123456789123456789 + 9876543210123456789") << "123456789123456789123456789" << "9876543210123456789" << "123456798999999999246913578";

        QTest::newRow("12345678901234567890123456789012345678901234567890123456789012345678901234567890 + 12345678901234567890123456789012345678901234567890123456789012345678901234567890")
                << "12345678901234567890123456789012345678901234567890123456789012345678901234567890"
                << "12345678901234567890123456789012345678901234567890123456789012345678901234567890"
                << "24691357802469135780246913578024691357802469135780246913578024691357802469135780";
    }

    void slotSummatoryTest()
    {
        QFETCH(QString, num1);
        QFETCH(QString, num2);
        QFETCH(QString, result);

        try
        {
            MathTools::BigNumber first(num1);
            MathTools::BigNumber second(num2);
            MathTools::BigNumber sum = first + second;
            QCOMPARE(sum.toString(), result);
        }
        catch (MathTools::Exception&)
        {
            QCOMPARE(result, ::errorString());
        }
    }

};

QTEST_MAIN(BigNumberTest)

#include "main.moc"
