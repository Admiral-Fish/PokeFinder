/*
 * This file is part of PokéFinder
 * Copyright (C) 2017 by Admiral_Fish and bumba
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setupModels();
    ui->tableView->resizeColumnsToContents();
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    createLanguageMenu();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent* event)
{
    if(event != 0)
    {
        switch(event->type())
        {
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            break;

        case QEvent::LocaleChange:
        {
            QString locale = QLocale::system().name();
            locale.truncate(locale.lastIndexOf('_'));
            loadLanguage(locale);
        }
            break;
        default:
            break;
        }
    }
    QMainWindow::changeEvent(event);
}

void MainWindow::createLanguageMenu(void)
{
    QActionGroup* langGroup = new QActionGroup(ui->menuLanguage);
    langGroup->setExclusive(true);

    connect(langGroup, SIGNAL (triggered(QAction *)), this, SLOT (slotLanguageChanged(QAction *)));

    QString defaultLocale = QLocale::system().name();
    defaultLocale.truncate(defaultLocale.lastIndexOf('_'));

    m_langPath = QApplication::applicationDirPath();
    m_langPath.append("/languages");
    QDir dir(m_langPath);
    QStringList fileNames = dir.entryList(QStringList("PokeFinder_*.qm"));

    for(int i = 0; i < fileNames.size(); i++)
    {
        QString locale;
        locale = fileNames[i];
        locale.truncate(locale.lastIndexOf('.'));
        locale.remove(0, locale.indexOf('_') + 1);

        QString lang = QLocale::languageToString(QLocale(locale).language());
        QAction *action = new QAction(lang, this);
        action->setCheckable(true);
        action->setData(locale);

        ui->menuLanguage->addAction(action);
        langGroup->addAction(action);

        if(defaultLocale == locale)
            action->setChecked(true);
    }

}

void MainWindow::slotLanguageChanged(QAction* action)
{
    if(action != 0)
        loadLanguage(action->data().toString());
}

void switchTranslator(QTranslator& translator, const QString& filename)
{
    qApp->removeTranslator(&translator);
    if(translator.load(QCoreApplication::applicationDirPath() + "/languages/" + filename))
        qApp->installTranslator(&translator);
}

void MainWindow::loadLanguage(const QString& rLanguage)
{
    if(m_currLang != rLanguage)
    {
        m_currLang = rLanguage;
        QLocale locale = QLocale(m_currLang);
        QLocale::setDefault(locale);
        QString languageName = QLocale::languageToString(locale.language());
        switchTranslator(m_translator, QString("PokeFinder_%1.qm").arg(rLanguage));
        switchTranslator(m_translatorQt, QString("qt_%1.qm").arg(rLanguage));
    }
}

void MainWindow::on_generate_clicked()
{
    bool pass;
    QMessageBox error;
    QString input;

    uint32_t seed = 0;
    input = ui->initialSeed->text();
    if (input != "")
    {
        seed = input.toUInt(&pass, 16);
        if (!pass)
        {
            error.setText("Please enter seed in valid hexadecimal format.");
            error.exec();
            return;
        }
    }

    uint32_t startingFrame = 1;
    input = ui->startingFrame->text();
    if (input != "")
    {
        startingFrame = input.toUInt(&pass, 10);
        if (!pass)
        {
            error.setText("Please enter starting frame in valid decimal format.");
            error.exec();
            return;
        }
        else if (startingFrame > 0xffffffff)
        {
            error.setText("Please enter a value lower then 4294967296.");
            error.exec();
            return;
        }
    }

    uint32_t maxResults = 100000;
    input = ui->maxResults->text();
    if (input != "")
    {
        maxResults = input.toUInt(&pass, 10);
        if (!pass)
        {
            error.setText("Please enter max results in valid decimal format.");
            error.exec();
            return;
        }
        else if (maxResults > 0xffffffff)
        {
            error.setText("Please enter a value lower then 4294967296.");
            error.exec();
            return;
        }
    }

    uint32_t tid = 12345;
    input = ui->id->text();
    if (input != "")
    {
        tid = input.toUInt(&pass, 10);
        if (!pass)
        {
            error.setText("Please enter Trainer ID in valid decimal format.");
            error.exec();
            return;
        }
        else if (tid > 0xffff)
        {
            error.setText("Please enter a value lower then 65536.");
            error.exec();
            return;
        }
    }

    uint32_t sid = 54321;
    input = ui->sid->text();
    if (input != "")
    {
        sid = input.toUInt(&pass, 10);
        if (!pass)
        {
            error.setText("Please enter Trainer SID in valid decimal format.");
            error.exec();
            return;
        }
        else if (sid > 0xffff)
        {
            error.setText("Please enter a value lower then 65536.");
            error.exec();
            return;
        }
    }

    // Force early garbage collection
    QStandardItemModel *model = new QStandardItemModel(this);
    model->setHorizontalHeaderLabels({tr("Frame"), tr("PID"), tr("!!!"), tr("Nature"), tr("Ability"), tr("HP"), tr("Atk"), tr("Def"), tr("SpA"), tr("SpD"), tr("Spe"), tr("Hidden"), tr("Power"), tr("Gender"), tr("Seed Time"), tr("Real Time")});

    int genderRatioIndex = ui->comboBoxGenderRatio->currentIndex();
    GeneratorGen3 generator = GeneratorGen3(maxResults, startingFrame, seed, tid, sid);
    FrameCompare compare = FrameCompare(ui->comboBoxHP->currentIndex(), ui->spinBoxHP->value(), ui->comboBoxAtk->currentIndex(), ui->spinBoxAtk->value(), ui->comboBoxDef->currentIndex(), ui->spinBoxDef->value(), ui->comboBoxSpA->currentIndex(), ui->spinBoxSpA->value(), ui->comboBoxSpD->currentIndex(), ui->spinBoxSpD->value(), ui->comboBoxSpe->currentIndex(), ui->spinBoxSpe->value(), ui->comboBoxGender->currentIndex(), genderRatioIndex, ui->comboBoxAbility->currentIndex(), ui->comboBoxNature, ui->comboBoxHiddenP, ui->checkBoxShiny->isChecked(), ui->checkBoxDisable->isChecked());
    int method = ui->comboBoxMethod->currentIndex();

    if (method == 0)
        generator.frameType = Method1;
    else if (method == 1)
        generator.frameType = Method2;
    else if (method == 2)
        generator.frameType = Method4;
    else if (method == 3)
        generator.frameType = XDColo;
    else
        generator.frameType = Channel;

    std::vector<FrameGen3> frames = generator.Generate(compare);
    int size = frames.size();

    for (int i = 0; i < size; i++)
        model->appendRow(frames[i].GetTableRow(genderRatioIndex));

    ui->tableView->setModel(model);
}

void MainWindow::natureItemCheck(QModelIndex a, QModelIndex b)
{
    (void)a;
    (void)b;

    QString newFirst = "";
    for(int i = 1; i < 26; i++)
    {
        if(ui->comboBoxNature->model()->data(ui->comboBoxNature->model()->index(i, 0), Qt::CheckStateRole).toBool())
        {
            QString text = ui->comboBoxNature->model()->data(ui->comboBoxNature->model()->index(i, 0)).toString();
            text.append(", ");
            newFirst += text;
        }
    }
    if(newFirst.length() != 0)
    {
        newFirst.remove(newFirst.length() - 2, 2);
        ui->comboBoxNature->model()->setData(ui->comboBoxNature->model()->index(0, 0), newFirst);
    }
    else
    {
        ui->comboBoxNature->model()->setData(ui->comboBoxNature->model()->index(0, 0), tr("Any"));
    }
}

void MainWindow::hiddenItemCheck(QModelIndex a, QModelIndex b)
{
    (void)a;
    (void)b;

    QString newFirst = "";
    for(int i = 1; i < 17; i++)
    {
        if(ui->comboBoxHiddenP->model()->data(ui->comboBoxHiddenP->model()->index(i, 0), Qt::CheckStateRole).toBool())
        {
            QString text = ui->comboBoxHiddenP->model()->data(ui->comboBoxHiddenP->model()->index(i, 0)).toString();
            text.append(", ");
            newFirst += text;
        }
    }
    if(newFirst.length() != 0)
    {
        newFirst.remove(newFirst.length() - 2, 2);
        ui->comboBoxHiddenP->model()->setData(ui->comboBoxHiddenP->model()->index(0, 0), newFirst);
    }
    else
    {
        ui->comboBoxHiddenP->model()->setData(ui->comboBoxHiddenP->model()->index(0, 0), tr("Any"));
    }
}

void MainWindow::setupModels()
{
    QStandardItemModel *natures = new QStandardItemModel(26, 1, this);
    QString natureList[25] = {QObject::tr("Hardy"), QObject::tr("Lonely"), QObject::tr("Brave"), QObject::tr("Adamant"), QObject::tr("Naughty"), QObject::tr("Bold"), QObject::tr("Docile"), QObject::tr("Relaxed"), QObject::tr("Impish"), QObject::tr("Lax"), QObject::tr("Timid"), QObject::tr("Hasty"), QObject::tr("Serious"), QObject::tr("Jolly"), QObject::tr("Naive"), QObject::tr("Modest"), QObject::tr("Mild"), QObject::tr("Quiet"), QObject::tr("Bashful"), QObject::tr("Rash"), QObject::tr("Calm"), QObject::tr("Gentle"), QObject::tr("Sassy"), QObject::tr("Careful"), QObject::tr("Quirky") };
    QStandardItem* firstNature = new QStandardItem(tr("Any"));
    natures->setItem(0, firstNature);
    for(int i = 0; i < 25; i++)
    {
        QStandardItem* item = new QStandardItem(natureList[i]);
        item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        item->setData(Qt::Unchecked, Qt::CheckStateRole);

        natures->setItem(i + 1, item);
    }
    ui->comboBoxNature->setModel(natures);
    connect(ui->comboBoxNature->model(), SIGNAL(dataChanged(QModelIndex, QModelIndex, QVector<int>)), this, SLOT(natureItemCheck(QModelIndex, QModelIndex)));

    QStandardItemModel *hidden = new QStandardItemModel(17, 1, this);
    QString powerList[16] = {QObject::tr("Fighting"), QObject::tr("Flying"), QObject::tr("Poison"), QObject::tr("Ground"), QObject::tr("Rock"), QObject::tr("Bug"), QObject::tr("Ghost"), QObject::tr("Steel"), QObject::tr("Fire"), QObject::tr("Water"), QObject::tr("Grass"), QObject::tr("Electric"), QObject::tr("Psychic"), QObject::tr("Ice"), QObject::tr("Dragon"), QObject::tr("Dark") };
    QStandardItem* firstPower = new QStandardItem(tr("Any"));
    hidden->setItem(0, firstPower);
    for(int i = 0; i < 16; i++)
    {
        QStandardItem* item = new QStandardItem(powerList[i]);
        item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        item->setData(Qt::Unchecked, Qt::CheckStateRole);

        hidden->setItem(i + 1, item);
    }
    ui->comboBoxHiddenP->setModel(hidden);
    connect(ui->comboBoxHiddenP->model(), SIGNAL(dataChanged(QModelIndex, QModelIndex, QVector<int>)), this, SLOT(hiddenItemCheck(QModelIndex, QModelIndex)));

    QStandardItemModel *model = new QStandardItemModel(this);
    model->setHorizontalHeaderLabels({tr("Frame"), tr("PID"), tr("!!!"), tr("Nature"), tr("Ability"), tr("HP"), tr("Atk"), tr("Def"), tr("SpA"), tr("SpD"), tr("Spe"), tr("Hidden"), tr("Power"), tr("Gender"), tr("Seed Time"), tr("Real Time")});
    ui->tableView->setModel(model);
    ui->tableView->verticalHeader()->setVisible(false);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->resizeColumnsToContents();
}
