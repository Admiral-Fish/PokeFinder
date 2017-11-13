#ifndef IDVALIDATOR_HPP
#define IDVALIDATOR_HPP

#include <QValidator>

typedef uint32_t u32;

class IDValidator : public QValidator
{
public:
    IDValidator(QObject* parent = 0);

    void fixup(QString &) const;

    QValidator::State validate(QString &, int &) const;
};

#endif // IDVALIDATOR_HPP
