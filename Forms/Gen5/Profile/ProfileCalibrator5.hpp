#ifndef PROFILECALIBRATOR5_HPP
#define PROFILECALIBRATOR5_HPP

#include <QWidget>

namespace Ui
{
    class ProfileCalibrator5;
}

class ProfileCalibrator5 : public QWidget
{
    Q_OBJECT

public:
    explicit ProfileCalibrator5(QWidget *parent = nullptr);
    ~ProfileCalibrator5();

private:
    Ui::ProfileCalibrator5 *ui;
};

#endif // PROFILECALIBRATOR5_HPP
