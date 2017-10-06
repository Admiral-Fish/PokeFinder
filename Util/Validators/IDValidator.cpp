#include "IDValidator.hpp"

IDValidator::IDValidator(QObject *parent) : QValidator(parent)
{

}

QValidator::State IDValidator::validate(QString & input, int & pos) const
{
    bool pass;
    uint32_t tid = input.toUInt(&pass, 10);
    (void) pos;

    if (input == "")
    {
        return QValidator::Acceptable;
    }
    else if (!pass)
    {
        return QValidator::Invalid;
    }
    else if (tid > 0xffff)
    {
        return QValidator::Invalid;
    }

    return QValidator::Acceptable;
}
