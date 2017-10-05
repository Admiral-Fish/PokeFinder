#ifndef SEEDVALIDATOR_HPP
#define SEEDVALIDATOR_HPP

#include <QValidator>

class SeedValidator : public QValidator
{
public:
    SeedValidator(QObject* parent = 0);

    QValidator::State validate(QString &, int &) const;
};

#endif // SEEDVALIDATOR_HPP
