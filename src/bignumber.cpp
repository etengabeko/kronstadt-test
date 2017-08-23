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

        m_sign = isNegative ? Negative
                            : Positive;

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
    QVector<quint8>::iterator first = m_digits.begin(),
                              last  = m_digits.end()-1;
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
    m_digits.erase(m_digits.begin(), first);
    m_digits.squeeze();
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
        bool isNegative = m_sign == Negative;
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

BigNumber BigNumber::operator+ (const BigNumber& rhs) const
{
    if (isZero())
    {
        return rhs;
    }
    else if (rhs.isZero())
    {
        return *this;
    }
    else if (   m_digits == rhs.m_digits
             && m_sign != rhs.m_sign)
    {
        return BigNumber();
    }

    BigNumber result;
    result.m_digits.fill(0, 1 + std::max(m_digits.size(), rhs.m_digits.size()));

    bool rhsIsGreater = signIgnoreLess(rhs);
    const QVector<quint8>& first  = rhsIsGreater ? rhs.m_digits
                                                 : m_digits;
    QVector<quint8> second = rhsIsGreater ? m_digits
                                          : rhs.m_digits;
    expand(second, first.size());

    quint8 reminder = 0;
    for (int i = first.size()-1; i >= 0; --i)
    {
        if (m_sign == rhs.m_sign)
        {
            quint8 sum = first[i] + second[i] + reminder;
            reminder = sum / 10;
            result.m_digits[i+1] = sum % 10;
        }
        else
        {
            qint8 diff = first[i] - second[i] - reminder;
            reminder = (diff < 0) ? 1 : 0;
            if (diff < 0)
            {
                diff += 10;
            }
            result.m_digits[i+1] = diff;
        }
    }
    result.m_digits[0] = reminder;
    result.m_sign = rhsIsGreater ? rhs.m_sign
                                 : m_sign;

    result.trim();

    return result;
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
        for (QVector<quint8>::const_iterator first = target.cbegin(), last = target.cend(); first != last; ++first)
        {
            *(dest++) = *first;
        }
        std::swap(copy, target);
    }
}

BigNumber BigNumber::operator- (const BigNumber& rhs) const
{
    Q_UNUSED(rhs);
    throw Exception(QCoreApplication::tr("Operator - is not implemented is base class BigNumber."));
    return BigNumber();
}

BigNumber BigNumber::operator* (const BigNumber& rhs) const
{
    Q_UNUSED(rhs);
    throw Exception(QCoreApplication::tr("Operator * is not implemented is base class BigNumber."));
    return BigNumber();
}

BigNumber BigNumber::operator/ (const BigNumber& rhs) const
{
    Q_UNUSED(rhs);
    throw Exception(QCoreApplication::tr("Operator / is not implemented is base class BigNumber."));
    return BigNumber();
}

} // MathTools
