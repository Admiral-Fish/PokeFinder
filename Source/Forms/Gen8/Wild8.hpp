#ifndef WILD8_HPP
#define WILD8_HPP

#include <QWidget>

namespace Ui {
    class Wild8;
}

class Wild8 : public QWidget
{
    Q_OBJECT

        public:
                 explicit Wild8(QWidget *parent = nullptr);
    ~Wild8();

private:
    Ui::Wild8 *ui;
};

#endif // WILD8_HPP
