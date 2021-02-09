#ifndef RNGEUCLIDEANTEST_HPP
#define RNGEUCLIDEANTEST_HPP

#include <QObject>

class RNGEuclideanTest : public QObject
{
    Q_OBJECT
private slots:
    void ivs_data();
    void ivs();

    void pid_data();
    void pid();

    void ivsChannel_data();
    void ivsChannel();
};

#endif // RNGEUCLIDEANTEST_HPP
