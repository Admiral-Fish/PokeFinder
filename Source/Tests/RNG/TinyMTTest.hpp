#ifndef TINYMTTEST_HPP
#define TINYMTTEST_HPP

#include <QObject>

class TinyMTTest : public QObject
{
    Q_OBJECT
private slots:
    void advance_data();
    void advance();

    void next_data();
    void next();
};

#endif // TINYMTTEST_HPP
