#ifndef MTTEST_HPP
#define MTTEST_HPP

#include <QObject>

class MTTest : public QObject
{
    Q_OBJECT
private slots:
    void advance_data();
    void advance();

    void next_data();
    void next();
};

#endif // MTTEST_HPP
