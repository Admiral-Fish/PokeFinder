#ifndef EGG3_HPP
#define EGG3_HPP

#include <QMainWindow>

namespace Ui {
class Egg3;
}

class Egg3 : public QMainWindow
{
    Q_OBJECT

protected:
    void changeEvent(QEvent*);

public:
    explicit Egg3(QWidget *parent = 0);
    ~Egg3();

private:
    Ui::Egg3 *ui;

    void setupModels();
};

#endif // EGG3_HPP
