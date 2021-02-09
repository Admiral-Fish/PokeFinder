#ifndef RNGCACHETEST_HPP
#define RNGCACHETEST_HPP

#include <QObject>

class RNGCacheTest : public QObject
{
    Q_OBJECT
private slots:
    void ivs_data();
    void ivs();

    void pid_data();
    void pid();
};

#endif // RNGCACHETEST_HPP
