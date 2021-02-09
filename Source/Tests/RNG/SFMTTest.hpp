#ifndef SFMTTEST_HPP
#define SFMTTEST_HPP

#include <QObject>

class SFMTTest : public QObject
{
    Q_OBJECT
private slots:
    void advance_data();
    void advance();

    void next_data();
    void next();

    void nextUInt_data();
    void nextUInt();
};

#endif // SFMTTEST_HPP
