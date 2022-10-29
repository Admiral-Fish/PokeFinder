#ifndef IDGENERATOR8TEST_HPP
#define IDGENERATOR8TEST_HPP

#include <QObject>

class IDGenerator8Test : public QObject
{
    Q_OBJECT
private slots:
    void generate_data();
    void generate();
};

#endif // IDGENERATOR8TEST_HPP
