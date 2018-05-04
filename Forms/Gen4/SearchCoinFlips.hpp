#ifndef SEARCHCOINFLIPS_HPP
#define SEARCHCOINFLIPS_HPP

#include <QDialog>
#include <QPixmap>
#include <QImage>
#include <Util/DateTime.hpp>
#include <PokeFinderCore/Objects/Utilities.hpp>

using std::vector;

namespace Ui
{
    class SearchCoinFlips;
}

class SearchCoinFlips : public QDialog
{
    Q_OBJECT

private:
    Ui::SearchCoinFlips *ui;
    vector<DateTime> data;
    vector<bool> possible;

private slots:
    void on_pushButtonHeads_clicked();
    void on_pushButtonTails_clicked();
    void on_lineEditFlips_textChanged(const QString &arg1);
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

signals:
    void possibleResults(vector<bool>);

public:
    explicit SearchCoinFlips(vector<DateTime> model, QWidget *parent = 0);
    ~SearchCoinFlips();
};

#endif // SEARCHCOINFLIPS_HPP
