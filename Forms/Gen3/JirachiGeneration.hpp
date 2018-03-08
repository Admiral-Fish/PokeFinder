#ifndef JIRACHIGENERATION_HPP
#define JIRACHIGENERATION_HPP

#include <QMainWindow>
#include <QStandardItemModel>
#include <QStandardItem>
#include <Forms/QTextBox.hpp>
#include <QString>
#include <QChar>
#include <libPokeFinder/RNG/LCRNG.hpp>

typedef uint32_t u32;

namespace Ui {
class JirachiGeneration;
}

class JirachiGeneration : public QMainWindow
{
    Q_OBJECT

public:
    explicit JirachiGeneration(QWidget *parent = 0);
    ~JirachiGeneration();

    QString flip(QString text);

private:
    QStandardItemModel *s = new QStandardItemModel(this);

    void setupModels();
    void genListOut(u32 seed);
    QString calcProbable(u32 seed);

private slots:
    void on_pushButtonGenerate_clicked();

private:
    Ui::JirachiGeneration *ui;
};

#endif // JIRACHIGENERATION_HPP
