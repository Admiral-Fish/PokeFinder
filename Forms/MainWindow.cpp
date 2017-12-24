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

#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include "Researcher.hpp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    idVal = new IDValidator();
    seedVal = new SeedValidator();
    frameVal = new FrameValidator();

    setupModels();
    updateProfiles();

    QFile file(QApplication::applicationDirPath() + "/profiles.xml");

    if(!file.exists())
        createProfileXml();

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
                setupModels();
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
    u32 seed = ui->initialSeed->text().toUInt(NULL, 16);
    u32 startingFrame = ui->startingFrame->text().toUInt(NULL, 10);
    u32 maxResults = ui->maxResults->text().toUInt(NULL, 10);
    u32 tid = ui->id->text().toUInt(NULL, 10);
    u32 sid = ui->sid->text().toUInt(NULL, 10);
    u32 offset = ui->delay->text().toUInt(NULL, 10);

    // Force early garbage collection
    QStandardItemModel *model = new QStandardItemModel(this);
    model->setHorizontalHeaderLabels({tr("Frame"), tr("PID"), tr("!!!"), tr("Nature"), tr("Ability"), tr("HP"), tr("Atk"), tr("Def"), tr("SpA"), tr("SpD"), tr("Spe"), tr("Hidden"), tr("Power"), tr("Gender"), tr("Time")});

    int genderRatioIndex = ui->comboBoxGenderRatio->currentIndex();
    GeneratorGen3 generator = GeneratorGen3(maxResults, startingFrame, seed, tid, sid, offset);
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

    vector<FrameGen3> frames = generator.Generate(compare);
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

void MainWindow::createProfileXml()
{
    QFile file(QApplication::applicationDirPath() + "/profiles.xml");
    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QDomDocument doc;
        QDomElement profiles = doc.createElement(QString("Profiles"));
        doc.appendChild(profiles);
        QTextStream stream( &file );
        stream << doc.toString();
        file.close();
    }
}

void MainWindow::setupModels()
{
    QStandardItemModel *natures = new QStandardItemModel(26, 1, this);
    vector<QString> natureList = Nature::GetNatures();
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
    vector<QString> powerList = Power::GetPowers();
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
    model->setHorizontalHeaderLabels({tr("Frame"), tr("PID"), tr("!!!"), tr("Nature"), tr("Ability"), tr("HP"), tr("Atk"), tr("Def"), tr("SpA"), tr("SpD"), tr("Spe"), tr("Hidden"), tr("Power"), tr("Gender"), tr("Time")});
    ui->tableView->setModel(model);
    ui->tableView->verticalHeader()->setVisible(false);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    connect(ui->id, SIGNAL(textChanged(QString)), this, SLOT(checkLineEdits(QString)));
    connect(ui->sid, SIGNAL(textChanged(QString)), this, SLOT(checkLineEdits(QString)));
    connect(ui->startingFrame, SIGNAL(textChanged(QString)), this, SLOT(checkLineEdits(QString)));
    connect(ui->maxResults, SIGNAL(textChanged(QString)), this, SLOT(checkLineEdits(QString)));
    connect(ui->initialSeed, SIGNAL(textChanged(QString)), this, SLOT(checkLineEdits(QString)));
    connect(ui->delay, SIGNAL(textChanged(QString)), this, SLOT(checkLineEdits(QString)));

    ui->comboBoxMethod->setEditable(true);
    ui->comboBoxMethod->lineEdit()->setAlignment(Qt::AlignCenter);
    for(int i = 0; i < ui->comboBoxMethod->count(); i++)
        ui->comboBoxMethod->setItemData(i, Qt::AlignCenter, Qt::TextAlignmentRole);
    ui->comboBoxMethod->setEditable(false);
}

void MainWindow::on_saveProfile_clicked()
{
    ProfileManagerGen3* manager = new ProfileManagerGen3();
    connect(manager, SIGNAL(updateProfiles()), this, SLOT(updateProfiles()));
    manager->exec();
}

void MainWindow::updateProfiles()
{
    profiles = ProfileGen3::loadProfileList();

    QStandardItemModel *profile = new QStandardItemModel(profiles.size() + 1, 1, this);
    QStandardItem* firstProfile = new QStandardItem(tr("None"));
    profile->setItem(0, firstProfile);
    for(int i = 0; i < (int)profiles.size(); i++)
    {
        QStandardItem* item = new QStandardItem(profiles.at(i).profileName);
        profile->setItem(i + 1, item);
    }
    ui->comboBoxProfiles->setModel(profile);
}

void MainWindow::checkLineEdits(QString str)
{
    (void) str;
    int pos = 0;
    QString id = ui->id->text();
    QString sid = ui->sid->text();
    QString startingFrame = ui->startingFrame->text();
    QString maxResults = ui->maxResults->text();
    QString initialSeed = ui->initialSeed->text().toUpper();
    QString delay = ui->delay->text();
    ui->initialSeed->setText(initialSeed);
    if(idVal->validate(id, pos) == QValidator::Acceptable && idVal->validate(sid, pos) == QValidator::Acceptable && frameVal->validate(startingFrame, pos) == QValidator::Acceptable && frameVal->validate(maxResults, pos) == QValidator::Acceptable && seedVal->validate(initialSeed, pos) == QValidator::Acceptable && frameVal->validate(delay, pos))
    {
        ui->generate->setEnabled(true);
    }
    else
    {
        idVal->fixup(id);
        idVal->fixup(sid);
        frameVal->fixup(startingFrame);
        frameVal->fixup(maxResults);
        frameVal->fixup(delay);
        seedVal->fixup(initialSeed);

        ui->id->setText(id);
        ui->sid->setText(sid);
        ui->startingFrame->setText(startingFrame);
        ui->maxResults->setText(maxResults);
        ui->initialSeed->setText(initialSeed);
        ui->delay->setText(delay);

        ui->generate->setEnabled(false);
        checkLineEdits(str);
    }
}

void MainWindow::on_comboBoxProfiles_currentIndexChanged(int index)
{
    if(index == 0)
    {
        ui->id->setReadOnly(false);
        ui->sid->setReadOnly(false);
        ui->id->setText("");
        ui->sid->setText("");
    }
    else
    {
        ui->id->setReadOnly(true);
        ui->sid->setReadOnly(true);
        ui->id->setText(QString::number(profiles.at(index - 1).tid));
        ui->sid->setText(QString::number(profiles.at(index - 1).sid));
    }
}

void MainWindow::on_anyNature_clicked()
{
    for(int i = 1; i < 26; i++)
    {
        ui->comboBoxNature->model()->setData(ui->comboBoxNature->model()->index(i, 0), Qt::Unchecked, Qt::CheckStateRole);

    }

    ui->comboBoxNature->model()->setData(ui->comboBoxNature->model()->index(0, 0), tr("Any"));

}

void MainWindow::on_anyHiddenPower_clicked()
{
    for(int i = 1; i < 17; i++)
    {
        ui->comboBoxHiddenP->model()->setData(ui->comboBoxHiddenP->model()->index(i, 0), Qt::Unchecked, Qt::CheckStateRole);

    }

    ui->comboBoxHiddenP->model()->setData(ui->comboBoxHiddenP->model()->index(0, 0), tr("Any"));
}

void MainWindow::on_checkBoxDelay_clicked()
{
    if(ui->checkBoxDelay->isChecked())
    {
        ui->delay->setEnabled(true);
    }
    else
    {
        ui->delay->setEnabled(false);
        ui->delay->setText("");
    }
}


void MainWindow::on_actionResearcher_triggered()
{

    r = new Researcher();
    r->show();

}
