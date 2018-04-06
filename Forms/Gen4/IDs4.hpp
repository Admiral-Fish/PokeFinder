#ifndef IDS4_HPP
#define IDS4_HPP

#include <QMainWindow>

namespace Ui {
class IDs4;
}

class IDs4 : public QMainWindow
{
    Q_OBJECT

public:
    explicit IDs4(QWidget *parent = 0);
    ~IDs4();

private:
    Ui::IDs4 *ui;
};

#endif // IDS4_HPP
