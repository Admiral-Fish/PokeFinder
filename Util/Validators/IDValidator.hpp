#ifndef IDVALIDATOR_HPP
#define IDVALIDATOR_HPP

#include <QValidator>

class IDValidator : public QValidator
{
public:
    IDValidator(QObject* parent = 0);

    QValidator::State validate(QString &, int &) const;
};

#endif // IDVALIDATOR_HPP
