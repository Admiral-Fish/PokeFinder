#ifndef SEEDVALIDATOR_HPP
#define SEEDVALIDATOR_HPP

#include <QValidator>

class SeedValidator : public QValidator
{
public:
    SeedValidator(QObject* parent = 0);

    void fixup(QString &) const;

    QValidator::State validate(QString &, int &) const;
};

#endif // SEEDVALIDATOR_HPP
