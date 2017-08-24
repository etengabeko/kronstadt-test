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
 *
 * @note  Класс такого рода не должен обладать полиморфизмом.
 *        Потому как при использовании операторов +, -, * и /, исходя из общепринятой семантики таких функций,
 *        должен быть возвращён новый объект. При этом, если эти операторы будут использоваться производным классом,
 *        произойдёт срезка возращаемого по значению объекта.
 *        Чтобы избежать указанной проблемы, можно возвращать из перечисленных операторов
 *        объект std::unique_ptr<BaseClass>. Но это приведёт к нарушению общепринятой семантики
 *        и к появлению некоторых на первый взгляд не вполне очевидных ошибок.
 *        Например, следующий код вызовет ошибку компиляции:
 *            cout << (bignumber1 + bignumber2 + bignumber3) << endl;
 */
class BigNumber
{
public:
    BigNumber();
    explicit BigNumber(const QString& numStr);

    virtual ~BigNumber();

    BigNumber operator+ (BigNumber rhs) const;
    BigNumber operator- (BigNumber rhs) const;
    BigNumber operator* (BigNumber rhs) const;
    BigNumber operator/ (BigNumber rhs) const;

    virtual BigNumber& operator+= (const BigNumber& rhs);
    virtual BigNumber& operator-= (const BigNumber& rhs);
    virtual BigNumber& operator*= (const BigNumber& rhs);
    virtual BigNumber& operator/= (const BigNumber& rhs);

    QString toString() const;
    static BigNumber fromString(const QString& numStr, bool* ok = nullptr);

protected:
    bool isZero() const;
    bool signIgnoreLess(const BigNumber& rhs) const;

    void trim();
    void expand(int size);

    static void trim(QVector<quint8>& target);
    static void expand(QVector<quint8>& target, int size);
    
    static QVector<quint8> addition(const QVector<quint8>& first,
                                    const QVector<quint8>& second);
    static QVector<quint8> difference(const QVector<quint8>& greater,
                                      const QVector<quint8>& lesser);

protected:
    enum class Sign
    {
        Positive = 1,
        Negative = -1
    };

    QVector<quint8> m_digits;     //!< Поразрядное представление числа.
    Sign m_sign = Sign::Positive; //!< Знак числа (+/-).

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
