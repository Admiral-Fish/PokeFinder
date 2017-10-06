#include "IDValidator.hpp"

IDValidator::IDValidator(QObject *parent) : QValidator(parent)
{

}

void IDValidator::fixup(QString & input) const
{
    input.remove(QRegExp("[^0-9]"));
    uint32_t temp = input.toUInt(NULL, 10);
    if(temp > 0xffff)
    {
        input.remove(input.length() - 1, 1);
    }
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
