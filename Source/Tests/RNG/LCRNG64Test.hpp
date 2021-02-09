#ifndef LCRNG64TEST_HPP
#define LCRNG64TEST_HPP

#include <QObject>

class LCRNG64Test : public QObject
{
    Q_OBJECT
private slots:
    void advance_data();
    void advance();

    void next_data();
    void next();
};

#endif // LCRNG64TEST_HPP
