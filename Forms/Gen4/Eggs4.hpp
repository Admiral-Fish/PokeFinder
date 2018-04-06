#ifndef EGGS4_HPP
#define EGGS4_HPP

#include <QMainWindow>

namespace Ui {
class Eggs4;
}

class Eggs4 : public QMainWindow
{
    Q_OBJECT

public:
    explicit Eggs4(QWidget *parent = 0);
    ~Eggs4();

private:
    Ui::Eggs4 *ui;
};

#endif // EGGS4_HPP
