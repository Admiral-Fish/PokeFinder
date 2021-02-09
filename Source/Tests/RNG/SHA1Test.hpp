#ifndef SHA1TEST_HPP
#define SHA1TEST_HPP

#include <QObject>

class SHA1Test : public QObject
{
    Q_OBJECT
private slots:
    void hash_data();
    void hash();
};

#endif // SHA1TEST_HPP
