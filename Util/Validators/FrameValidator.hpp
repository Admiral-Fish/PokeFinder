#ifndef FRAMEVALIDATOR_HPP
#define FRAMEVALIDATOR_HPP

#include <QValidator>

class FrameValidator : public QValidator
{
public:
    FrameValidator(QObject* parent = 0);

    QValidator::State validate(QString &, int &) const;
};

#endif // FRAMEVALIDATOR_HPP
