#include "FrameValidator.hpp"

FrameValidator::FrameValidator(QObject *parent) : QValidator(parent)
{

}

QValidator::State FrameValidator::validate(QString & input, int & pos) const
{
    bool pass;
    uint32_t frame = input.toUInt(&pass, 10);
    (void) pos;

    if (input == "")
    {
        return QValidator::Acceptable;
    }
    else if (!pass)
    {
        return QValidator::Invalid;
    }
    else if(frame > UINT32_MAX)
    {
        return QValidator::Invalid;
    }

    return QValidator::Acceptable;
}
