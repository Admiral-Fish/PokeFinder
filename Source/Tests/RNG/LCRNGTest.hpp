#ifndef LCRNGTEST_HPP
#define LCRNGTEST_HPP

#include <QObject>

class LCRNGTest : public QObject
{
    Q_OBJECT
private slots:
    void advance_data();
    void advance();

    void next_data();
    void next();
};

#endif // LCRNGTEST_HPP
