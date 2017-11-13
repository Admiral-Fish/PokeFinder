#ifndef FRAMEVALIDATOR_HPP
#define FRAMEVALIDATOR_HPP

#include <QValidator>

typedef uint32_t u32;

class FrameValidator : public QValidator
{
public:
    FrameValidator(QObject* parent = 0);

    void fixup(QString &) const;

    QValidator::State validate(QString &, int &) const;
};

#endif // FRAMEVALIDATOR_HPP
