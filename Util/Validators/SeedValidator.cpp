#include "SeedValidator.hpp"

SeedValidator::SeedValidator(QObject *parent) : QValidator(parent)
{

}

QValidator::State SeedValidator::validate(QString & input, int & pos) const
{
    uint32_t seed;
    bool pass;
    (void) pos;
    if (input != "")
    {
        seed = input.toUInt(&pass, 16);
        if (!pass)
        {
            return QValidator::Invalid;
        }
        else if(seed > 0xffffffff)
        {
            return QValidator::Invalid;
        }
    }
    else
    {
        return QValidator::Invalid;
    }

    return QValidator::Acceptable;

}
