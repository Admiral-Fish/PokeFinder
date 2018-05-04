#include "SearchCoinFlips.hpp"
#include "ui_SearchCoinFlips.h"

SearchCoinFlips::SearchCoinFlips(vector<DateTime> model, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SearchCoinFlips)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::Widget | Qt::MSWindowsFixedSizeDialogHint);

    data = model;

    QIcon heads(QPixmap::fromImage(QImage(":/heads.png")));
    ui->pushButtonHeads->setIcon(heads);

    QIcon tails(QPixmap::fromImage(QImage(":/tails.png")));
    ui->pushButtonTails->setIcon(tails);
}

SearchCoinFlips::~SearchCoinFlips()
{
    delete ui;
}

void SearchCoinFlips::on_pushButtonHeads_clicked()
{
    QString string = ui->lineEditFlips->text();

    if (string.isEmpty())
        string = "H";
    else
        string += ", H";

    ui->lineEditFlips->setText(string);
}

void SearchCoinFlips::on_pushButtonTails_clicked()
{
    QString string = ui->lineEditFlips->text();

    if (string.isEmpty())
        string = "T";
    else
        string += ", T";

    ui->lineEditFlips->setText(string);
}

void SearchCoinFlips::on_lineEditFlips_textChanged(const QString &arg1)
{
    QStringList results = arg1.split(",", QString::SkipEmptyParts);

    int num = 0;

    possible.clear();
    for (int i = 0; i < data.size(); i++)
    {
        QStringList compare = Utilities::coinFlips(data[i].getSeed(), 15).split(",", QString::SkipEmptyParts);

        bool pass = true;
        for (int j = 0; j < results.size(); j++)
        {
            if (results[j] != compare[j])
            {
                pass = false;
                break;
            }
        }
        possible.push_back(pass);
        if (pass)
            num++;
    }

    ui->labelPossibleResults->setText(tr("Possible Results: ") + QString::number(num));
}

void SearchCoinFlips::on_buttonBox_accepted()
{
    emit possibleResults(possible);
    done(QDialog::Accepted);
}

void SearchCoinFlips::on_buttonBox_rejected()
{
    done(QDialog::Rejected);
}
