#include "FrameValidator.hpp"

FrameValidator::FrameValidator(QObject *parent) : QValidator(parent)
{

}

QValidator::State FrameValidator::validate(QString & input, int & pos) const
{
    uint32_t frame;
    bool pass;
    (void) pos;
    if (input != "")
    {
        frame = input.toUInt(&pass, 10);
        if (!pass)
        {
            return QValidator::Invalid;
        }
        else if(frame > UINT32_MAX)
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
