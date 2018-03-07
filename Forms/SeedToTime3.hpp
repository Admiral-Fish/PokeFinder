#ifndef SEEDTOTIME3_HPP
#define SEEDTOTIME3_HPP

#include <QMainWindow>
#include <QStandardItemModel>

namespace Ui {
class SeedToTime3;
}

class SeedToTime3 : public QMainWindow
{
    Q_OBJECT

public:
    explicit SeedToTime3(QWidget *parent = 0);
    ~SeedToTime3();

private:
    Ui::SeedToTime3 *ui;
    QStandardItemModel *m;

    void setupModels();
};

#endif // SEEDTOTIME3_HPP
