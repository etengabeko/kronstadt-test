#include "bignumber.h"

#include <QCoreApplication>
#include <QString>

namespace MathTools
{

Exception::Exception(const QString& message) :
    std::runtime_error(message.toStdString())
{

}

BigNumber::BigNumber() :
    m_digits(QVector<quint8>({ 0 }))
{

}

BigNumber::~BigNumber()
{

}

BigNumber::BigNumber(const QString& numStr) :
    BigNumber()
{
    if (!numStr.isEmpty())
    {
        bool isNegative = numStr.startsWith('-');

        m_sign = isNegative ? Sign::Negative
                            : Sign::Positive;

        m_digits.fill(0, numStr.size());
        for (int i = 0, sz = numStr.size(); i < sz; ++i)
        {
            if (   i == 0
                && isNegative)
            {
                continue;
            }

            if (!numStr[i].isDigit())
            {
                throw Exception(QCoreApplication::tr("Invalid initializator \"%1\": '%2' is not a digit.")
                                .arg(numStr)
                                .arg(numStr[i]));
            }
            m_digits[i] = numStr[i].digitValue();
        }

        trim();
    }
}

void BigNumber::trim()
{
    trim(m_digits);
}

void BigNumber::trim(QVector<quint8>& target)
{
    QVector<quint8>::iterator first = target.begin(),
                              last  = target.end()-1;
    while (first != last)
    {
        if (*first == 0)
        {
            ++first;
        }
        else
        {
            break;
        }
    }
    target.erase(target.begin(), first);
    target.squeeze();
}

void BigNumber::expand(int size)
{
    expand(m_digits, size);
}

void BigNumber::expand(QVector<quint8>& target, int size)
{
    if (target.size() < size)
    {
        QVector<quint8> copy;
        copy.fill(0, size);

        QVector<quint8>::iterator dest = copy.end() - target.size();
        for (auto first = target.cbegin(), last = target.cend(); first != last; ++first)
        {
            *(dest++) = *first;
        }

        std::swap(copy, target);
    }
}

BigNumber BigNumber::fromString(const QString& numStr, bool* ok)
{
    if (ok != nullptr)
    {
        *ok = false;
    }

    try
    {
        BigNumber result(numStr);
        if (ok != nullptr)
        {
            *ok = true;
        }
        return result;
    }
    catch (Exception&)
    {
        qt_noop();
    }

    return BigNumber();
}

QString BigNumber::toString() const
{
    QString result;

    if (isZero())
    {
        result = QString::number(0);
    }
    else
    {
        bool isNegative = (m_sign == Sign::Negative);
        result.reserve(m_digits.size() + (isNegative ? 1 : 0));

        if (isNegative)
        {
            result += "-";
        }

        for (quint8 each : m_digits)
        {
            result += QString::number(each);
        }
    }

    return result;
}

bool BigNumber::isZero() const
{
    return (   m_digits.size() == 1
            && m_digits.first() == 0);
}

BigNumber BigNumber::operator+ (BigNumber rhs) const
{
    return rhs += *this;
}

BigNumber& BigNumber::operator+= (const BigNumber& rhs)
{
    if (rhs.isZero())
    {
        return *this;
    }
    else if (isZero())
    {
        *this = rhs;
        return *this;
    }
    else if (   m_digits == rhs.m_digits
             && m_sign != rhs.m_sign)
    {
        *this = BigNumber();
        return *this;
    }
    else if (m_digits.size() > rhs.m_digits.size())
    {
        *this = *this + rhs;
        return *this;
    }

    expand(rhs.m_digits.size());

    if (m_sign == rhs.m_sign)
    {
        m_digits = addition(m_digits, rhs.m_digits);
    }
    else
    {
        bool rhsIsGreater = signIgnoreLess(rhs);
        m_digits = rhsIsGreater ? difference(rhs.m_digits, m_digits)
                                : difference(m_digits, rhs.m_digits);
        m_sign = rhsIsGreater ? rhs.m_sign
                              : m_sign;
    }

    trim();
    return *this;
}

bool BigNumber::signIgnoreLess(const BigNumber& rhs) const
{
    int lhsSize = m_digits.size(),
        rhsSize = rhs.m_digits.size();

    if (lhsSize != rhsSize)
    {
        return lhsSize < rhsSize;
    }

    for (int i = 0; i < lhsSize; ++i)
    {
        if (m_digits[i] != rhs.m_digits[i])
        {
            return m_digits[i] < rhs.m_digits[i];
        }
    }

    return false;
}

QVector<quint8> BigNumber::addition(const QVector<quint8>& first,
                                    const QVector<quint8>& second)
{
    Q_ASSERT(first.size() == second.size());
    const int size = first.size();

    QVector<quint8> result;
    result.fill(0, size+1);

    quint8 reminder = 0;
    for (int i = size-1; i >= 0; --i)
    {
        quint8 sum = first[i] + second[i] + reminder;
        reminder = sum / 10;
        result[i+1] = sum % 10;
    }
    result[0] = reminder;

    return result;
}

QVector<quint8> BigNumber::difference(const QVector<quint8>& greater,
                                      const QVector<quint8>& lesser)
{
    Q_ASSERT(greater.size() == lesser.size());
    const int size = greater.size();

    QVector<quint8> result;
    result.fill(0, size);

    quint8 reminder = 0;
    for (int i = size-1; i >= 0; --i)
    {
        qint8 diff = greater[i] - lesser[i] - reminder;
        reminder = (diff < 0) ? 1 : 0;
        if (diff < 0)
        {
            diff += 10;
        }
        result[i] = diff;
    }
    Q_ASSERT(reminder == 0);

    return result;
}

BigNumber BigNumber::operator- (BigNumber rhs) const
{
    return rhs -= *this;
}

BigNumber& BigNumber::operator-= (const BigNumber& rhs)
{
    Q_UNUSED(rhs);
    throw Exception(QCoreApplication::tr("Operator -= is not implemented is base class BigNumber."));
    return *this;
}

BigNumber BigNumber::operator* (BigNumber rhs) const
{
    return rhs *= *this;
}

BigNumber& BigNumber::operator*= (const BigNumber& rhs)
{
    Q_UNUSED(rhs);
    throw Exception(QCoreApplication::tr("Operator *= is not implemented is base class BigNumber."));
    return *this;
}

BigNumber BigNumber::operator/ (BigNumber rhs) const
{
    return rhs /= *this;
}

BigNumber& BigNumber::operator/= (const BigNumber& rhs)
{
    Q_UNUSED(rhs);
    throw Exception(QCoreApplication::tr("Operator /= is not implemented is base class BigNumber."));
    return *this;
}

} // MathTools
