#ifndef SEEDVALIDATOR_HPP
#define SEEDVALIDATOR_HPP

#include <QValidator>

typedef uint32_t u32;

class SeedValidator : public QValidator
{
public:
    SeedValidator(QObject* parent = 0);

    void fixup(QString &) const;

    QValidator::State validate(QString &, int &) const;
};

#endif // SEEDVALIDATOR_HPP
