#ifndef BIG_NUMBER_H
#define BIG_NUMBER_H

#include <stdexcept>

#include <QVector>

class QString;

namespace MathTools
{

/**
 * @class BigNumber
 * @brief Класс представляет большое целое число и реализует операцию сложения.
 *        Является базовым для других классов, реализующих остальные арифметические операции.
 */
class BigNumber
{
public:
    BigNumber();
    virtual ~BigNumber();

    explicit BigNumber(const QString& numStr);

    virtual BigNumber operator+ (const BigNumber& rhs) const;
    virtual BigNumber operator- (const BigNumber& rhs) const;
    virtual BigNumber operator* (const BigNumber& rhs) const;
    virtual BigNumber operator/ (const BigNumber& rhs) const;

    QString toString() const;
    static BigNumber fromString(const QString& numStr, bool* ok = nullptr);

private:
    bool isZero() const;
    bool signIgnoreLess(const BigNumber& rhs) const;

    void trim();
    void expand(int size);

    static void expand(QVector<quint8>& target, int size);

private:
    enum Sign
    {
        Positive = 1,
        Negative = -1
    };

    QVector<quint8> m_digits;
    Sign m_sign = Positive;

};

/**
 * @class Exception
 * @brief Ошибки в процессе работы с BigNumber будут передаваться клиентскому коду в объектах исключений.
 */
class Exception : public std::runtime_error
{
public:
    explicit Exception(const QString& message);

};

} // MathTools

#endif // BIG_NUMBER_H
