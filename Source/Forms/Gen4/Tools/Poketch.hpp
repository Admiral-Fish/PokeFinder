#ifndef POKETCH_HPP
#define POKETCH_HPP

#include <QWidget>
#include <Core/Util/Utilities.hpp>

namespace Ui {
class Poketch;
}

class Poketch : public QWidget
{
    Q_OBJECT

public:
    explicit Poketch(u32 advances, QWidget *parent = nullptr);
    ~Poketch();

private slots:
    void on_pushButtonClose_clicked();

private:
    Ui::Poketch *ui;
    void setupModels(u32 advances);
};

#endif // POKETCH_HPP
