#include "IDValidator.hpp"

IDValidator::IDValidator(QObject *parent) : QValidator(parent)
{

}

QValidator::State IDValidator::validate(QString & input, int & pos) const
{
    (void) pos;
    bool pass;
    uint32_t tid;
    if (input != "")
    {
        tid = input.toUInt(&pass, 10);
        if (!pass)
        {
            return QValidator::Invalid;
        }
        else if (tid > 0xffff)
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
