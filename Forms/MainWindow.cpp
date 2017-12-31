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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setupModels();
    updateProfiles();

    QFile file(QApplication::applicationDirPath() + "/profiles.xml");

    if(!file.exists())
        createProfileXml();

    ui->tableViewStationary3->resizeColumnsToContents();
    ui->tableViewStationary3->setEditTriggers(QAbstractItemView::NoEditTriggers);

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
    m_langPath.append("/Languages");
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
    if(translator.load(QCoreApplication::applicationDirPath() + "/Languages/" + filename))
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

void MainWindow::on_generateStationary3_clicked()
{
    u32 seed = ui->initialSeedStationary3->text().toUInt(NULL, 16);
    u32 startingFrame = ui->startingFrameStationary3->text().toUInt(NULL, 10);
    u32 maxResults = ui->maxResultsStationary3->text().toUInt(NULL, 10);
    u32 tid = ui->idStationary3->text().toUInt(NULL, 10);
    u32 sid = ui->sidStationary3->text().toUInt(NULL, 10);
    u32 offset = ui->delayStationary3->text().toUInt(NULL, 10);

    // Force early garbage collection
    QStandardItemModel *model = new QStandardItemModel(this);
    model->setHorizontalHeaderLabels({tr("Frame"), tr("PID"), tr("!!!"), tr("Nature"), tr("Ability"), tr("HP"), tr("Atk"),
                                      tr("Def"), tr("SpA"), tr("SpD"),tr("Spe"), tr("Hidden"), tr("Power"), tr("Gender"), tr("Time")});

    int genderRatioIndex = ui->comboBoxGenderRatioStationary3->currentIndex();
    GeneratorGen3 generator = GeneratorGen3(maxResults, startingFrame, seed, tid, sid, offset);
    FrameCompare compare = FrameCompare(ui->comboBoxHPStationary3->currentIndex(), ui->spinBoxHPStationary3->value(), ui->comboBoxAtkStationary3->currentIndex(),
                                        ui->spinBoxAtkStationary3->value(), ui->comboBoxDefStationary3->currentIndex(), ui->spinBoxDefStationary3->value(),
                                        ui->comboBoxSpAStationary3->currentIndex(), ui->spinBoxSpAStationary3->value(), ui->comboBoxSpDStationary3->currentIndex(),
                                        ui->spinBoxSpDStationary3->value(), ui->comboBoxSpeStationary3->currentIndex(), ui->spinBoxSpeStationary3->value(),
                                        ui->comboBoxGenderStationary3->currentIndex(), genderRatioIndex, ui->comboBoxAbilityStationary3->currentIndex(),
                                        ui->comboBoxNatureStationary3->getChecked(), ui->comboBoxHiddenPowerStationary3->getChecked(),
                                        ui->checkBoxShinyStationary3->isChecked(), ui->checkBoxDisableStationary3->isChecked());

    int method = ui->comboBoxMethodStationary3->currentIndex();
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
    model->setRowCount(size);
    model->setColumnCount(15);

    QModelIndex m = QModelIndex();
    for (int i = 0; i < size; i++)
    {
        model->setData(model->index(i, 0, m), frames[i].frame);
        model->setData(model->index(i, 1, m), QString::number(frames[i].pid, 16).toUpper().rightJustified(8,'0'));
        model->setData(model->index(i, 2, m), frames[i].GetShiny());
        model->setData(model->index(i, 3, m), frames[i].GetNature());
        model->setData(model->index(i, 4, m), frames[i].ability);
        model->setData(model->index(i, 5, m), frames[i].ivs[0]);
        model->setData(model->index(i, 6, m), frames[i].ivs[1]);
        model->setData(model->index(i, 7, m), frames[i].ivs[2]);
        model->setData(model->index(i, 8, m), frames[i].ivs[3]);
        model->setData(model->index(i, 9, m), frames[i].ivs[4]);
        model->setData(model->index(i, 10, m), frames[i].ivs[5]);
        model->setData(model->index(i, 11, m), frames[i].GetPower());
        model->setData(model->index(i, 12, m), frames[i].power);
        model->setData(model->index(i, 13, m), frames[i].GetFemale25());
        model->setData(model->index(i, 14, m), frames[i].GetTime());
    }

    ui->tableViewStationary3->setModel(model);
}

void MainWindow::on_generateWild3_clicked()
{
    u32 seed = ui->initialSeedWild3->text().toUInt(NULL, 16);
    u32 startingFrame = ui->startingFrameWild3->text().toUInt(NULL, 10);
    u32 maxResults = ui->maxResultsWild3->text().toUInt(NULL, 10);
    u32 tid = ui->idWild3->text().toUInt(NULL, 10);
    u32 sid = ui->sidWild3->text().toUInt(NULL, 10);
    u32 offset = ui->delayWild3->text().toUInt(NULL, 10);

    // Force early garbage collection
    QStandardItemModel *model = new QStandardItemModel(this);
    model->setHorizontalHeaderLabels({tr("Frame"), tr("Occidentary"), tr("Slot"), tr("PID"), tr("!!!"), tr("Nature"), tr("Ability"), tr("HP"), tr("Atk"),
                                      tr("Def"), tr("SpA"), tr("SpD"), tr("Spe"), tr("Hidden"), tr("Power"), tr("Gender"), tr("Time")});

    int genderRatioIndex = ui->comboBoxGenderRatioWild3->currentIndex();
    GeneratorGen3 generator = GeneratorGen3(maxResults, startingFrame, seed, tid, sid, offset);
    FrameCompare compare = FrameCompare(ui->comboBoxHPWild3->currentIndex(), ui->spinBoxHPWild3->value(), ui->comboBoxAtkWild3->currentIndex(),
                                        ui->spinBoxAtkWild3->value(), ui->comboBoxDefWild3->currentIndex(), ui->spinBoxDefWild3->value(),
                                        ui->comboBoxSpAWild3->currentIndex(), ui->spinBoxSpAWild3->value(), ui->comboBoxSpDWild3->currentIndex(),
                                        ui->spinBoxSpDWild3->value(), ui->comboBoxSpeWild3->currentIndex(), ui->spinBoxSpeWild3->value(),
                                        ui->comboBoxGenderWild3->currentIndex(), genderRatioIndex, ui->comboBoxAbilityWild3->currentIndex(),
                                        ui->comboBoxNatureWild3->getChecked(), ui->comboBoxHiddenPowerWild3->getChecked(),
                                        ui->checkBoxShinyWild3->isChecked(), ui->checkBoxDisableWild3->isChecked());

    int method = ui->comboBoxMethodWild3->currentIndex();
    if (method == 0)
        generator.frameType = MethodH1;
    else if (method == 1)
        generator.frameType = MethodH2;
    else
        generator.frameType = MethodH4;

    vector<FrameGen3> frames = generator.Generate(compare);
    int size = frames.size();
    model->setRowCount(size);
    model->setColumnCount(17);

    QModelIndex m = QModelIndex();
    for (int i = 0; i < size; i++)
    {
        model->setData(model->index(i, 0, m), frames[i].frame);
        model->setData(model->index(i, 1, m), frames[i].occidentary);
        model->setData(model->index(i, 2, m), frames[i].encounterSlot);
        model->setData(model->index(i, 3, m), QString::number(frames[i].pid, 16).toUpper().rightJustified(8,'0'));
        model->setData(model->index(i, 4, m), frames[i].GetShiny());
        model->setData(model->index(i, 5, m), frames[i].GetNature());
        model->setData(model->index(i, 6, m), frames[i].ability);
        model->setData(model->index(i, 7, m), frames[i].ivs[0]);
        model->setData(model->index(i, 8, m), frames[i].ivs[1]);
        model->setData(model->index(i, 9, m), frames[i].ivs[2]);
        model->setData(model->index(i, 10, m), frames[i].ivs[3]);
        model->setData(model->index(i, 11, m), frames[i].ivs[4]);
        model->setData(model->index(i, 12, m), frames[i].ivs[5]);
        model->setData(model->index(i, 13, m), frames[i].GetPower());
        model->setData(model->index(i, 14, m), frames[i].power);
        model->setData(model->index(i, 15, m), frames[i].GetFemale25());
        model->setData(model->index(i, 16, m), frames[i].GetTime());
    }

    ui->tableViewWild3->setModel(model);
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
    QStringList natureList = Nature::GetNatures();
    ui->comboBoxNatureStationary3->addCheckItems(natureList, QVariant(), Qt::Unchecked);
    ui->comboBoxNatureWild3->addCheckItems(natureList, QVariant(), Qt::Unchecked);

    QStringList powerList = Power::GetPowers();
    ui->comboBoxHiddenPowerStationary3->addCheckItems(powerList, QVariant(), Qt::Unchecked);
    ui->comboBoxHiddenPowerWild3->addCheckItems(powerList, QVariant(), Qt::Unchecked);

    QStandardItemModel *model = new QStandardItemModel(this);
    model->setHorizontalHeaderLabels({tr("Frame"), tr("PID"), tr("!!!"), tr("Nature"), tr("Ability"), tr("HP"), tr("Atk"),
                                      tr("Def"), tr("SpA"), tr("SpD"), tr("Spe"), tr("Hidden"), tr("Power"), tr("Gender"), tr("Time")});
    ui->tableViewStationary3->setModel(model);
    ui->tableViewStationary3->verticalHeader()->setVisible(false);
    ui->tableViewStationary3->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    model = new QStandardItemModel(this);
    model->setHorizontalHeaderLabels({tr("Frame"), tr("Occidentary"), tr("Slot"), tr("PID"), tr("!!!"), tr("Nature"), tr("Ability"), tr("HP"), tr("Atk"),
                                      tr("Def"), tr("SpA"), tr("SpD"), tr("Spe"), tr("Hidden"), tr("Power"), tr("Gender"), tr("Time")});
    ui->tableViewWild3->setModel(model);
    ui->tableViewWild3->verticalHeader()->setVisible(false);
    ui->tableViewWild3->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->initialSeedStationary3->SetValues("[^0-9A-F]", 0xffffffff, 16);
    ui->idStationary3->SetValues("[^0-9]", 0xffff, 10);
    ui->sidStationary3->SetValues("[^0-9]", 0xffff, 10);
    ui->startingFrameStationary3->SetValues("[^0-9]", 0xffffffff, 10);
    ui->maxResultsStationary3->SetValues("[^0-9]", 0xffffffff, 10);
    ui->delayStationary3->SetValues("[^0-9]", 0xffffffff, 10);

    ui->initialSeedWild3->SetValues("[^0-9A-F]", 0xffffffff, 16);
    ui->idWild3->SetValues("[^0-9]", 0xffff, 10);
    ui->sidWild3->SetValues("[^0-9]", 0xffff, 10);
    ui->startingFrameWild3->SetValues("[^0-9]", 0xffffffff, 10);
    ui->maxResultsWild3->SetValues("[^0-9]", 0xffffffff, 10);
    ui->delayWild3->SetValues("[^0-9]", 0xffffffff, 10);

    ui->comboBoxMethodStationary3->setEditable(true);
    ui->comboBoxMethodStationary3->lineEdit()->setAlignment(Qt::AlignCenter);
    for(int i = 0; i < ui->comboBoxMethodStationary3->count(); i++)
        ui->comboBoxMethodStationary3->setItemData(i, Qt::AlignCenter, Qt::TextAlignmentRole);
    ui->comboBoxMethodStationary3->setEditable(false);
}

void MainWindow::on_saveProfileStationary3_clicked()
{
    ProfileManagerGen3* manager = new ProfileManagerGen3();
    manager->setAttribute(Qt::WA_QuitOnClose, false);
    connect(manager, SIGNAL(updateProfiles()), this, SLOT(updateProfiles()));
    manager->show();
}

void MainWindow::on_saveWild3_clicked()
{
    ProfileManagerGen3* manager = new ProfileManagerGen3();
    manager->setAttribute(Qt::WA_QuitOnClose, false);
    connect(manager, SIGNAL(updateProfiles()), this, SLOT(updateProfiles()));
    manager->show();
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

void MainWindow::on_comboBoxProfiles_currentIndexChanged(int index)
{
    if(index == 0)
    {
        ui->idStationary3->setReadOnly(false);
        ui->sidStationary3->setReadOnly(false);
        ui->idStationary3->setText("");
        ui->sidStationary3->setText("");
    }
    else
    {
        ui->idStationary3->setReadOnly(true);
        ui->sidStationary3->setReadOnly(true);
        ui->idStationary3->setText(QString::number(profiles.at(index - 1).tid));
        ui->sidStationary3->setText(QString::number(profiles.at(index - 1).sid));
    }
}

void MainWindow::on_anyNatureStationary3_clicked()
{
    ui->comboBoxNatureStationary3->uncheckAll();
}

void MainWindow::on_anyHiddenPowerStationary3_clicked()
{
    ui->comboBoxHiddenPowerStationary3->uncheckAll();
}

void MainWindow::on_anyHiddenPowerWild3_clicked()
{
    ui->comboBoxHiddenPowerWild3->uncheckAll();
}

void MainWindow::on_anyNatureWild3_clicked()
{
    ui->comboBoxNatureWild3->uncheckAll();
}

void MainWindow::on_checkBoxDelayStationary3_clicked()
{
    if(ui->checkBoxDelayStationary3->isChecked())
    {
        ui->delayStationary3->setEnabled(true);
    }
    else
    {
        ui->delayStationary3->setEnabled(false);
        ui->delayStationary3->setText("");
    }
}


void MainWindow::on_actionResearcher_triggered()
{
    r = new Researcher();
    r->setAttribute(Qt::WA_QuitOnClose, false);
    r->show();
}
