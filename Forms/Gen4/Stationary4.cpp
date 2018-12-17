/*
 * This file is part of PokéFinder
 * Copyright (C) 2017 by Admiral_Fish, bumba, and EzPzStreamz
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
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

#include "Stationary4.hpp"
#include "ui_Stationary4.h"

Stationary4::Stationary4(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Stationary4)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    updateProfiles();
    setupModels();

    qRegisterMetaType<QVector<Frame4>>("QVector<Frame4>");
}

Stationary4::~Stationary4()
{
    QSettings setting;
    setting.setValue("stationary4Profile", ui->comboBoxProfiles->currentIndex());
    setting.setValue("stationary4MinDelay", ui->minDelay->text());
    setting.setValue("stationary4MaxDelay", ui->maxDelay->text());
    setting.setValue("stationary4MinFrame", ui->minFrame->text());
    setting.setValue("stationary4MaxFrame", ui->maxFrame->text());

    delete ui;
    delete g;
    delete s;
    delete searcherMenu;
}

void Stationary4::updateProfiles()
{
    profiles = Profile4::loadProfileList();
    profiles.insert(profiles.begin(), Profile4());

    ui->comboBoxProfiles->clear();

    for (const auto &profile : profiles)
    {
        ui->comboBoxProfiles->addItem(profile.getProfileName());
    }

    QSettings setting;
    int val = setting.value("stationary4Profile").toInt();
    if (val < ui->comboBoxProfiles->count())
    {
        ui->comboBoxProfiles->setCurrentIndex(val >= 0 ? val : 0);
    }
}

void Stationary4::setupModels()
{
    ui->tableViewGenerator->setModel(g);
    ui->tableViewSearcher->setModel(s);

    ui->initialSeedGenerator->setValues(InputType::Seed32Bit);
    ui->idGenerator->setValues(InputType::TIDSID);
    ui->sidGenerator->setValues(InputType::TIDSID);
    ui->startingFrameGenerator->setValues(InputType::Frame32Bit);
    ui->maxResultsGenerator->setValues(InputType::Frame32Bit);
    ui->delayGenerator->setValues(InputType::Frame32Bit);

    ui->idSearcher->setValues(InputType::TIDSID);
    ui->sidSearcher->setValues(InputType::TIDSID);
    ui->minDelay->setValues(InputType::Delay);
    ui->maxDelay->setValues(InputType::Delay);
    ui->minFrame->setValues(InputType::Frame32Bit);
    ui->maxFrame->setValues(InputType::Frame32Bit);

    ui->comboBoxLeadSearcher->setItemData(0, Lead::Search);
    ui->comboBoxLeadSearcher->setItemData(1, Lead::Synchronize);
    ui->comboBoxLeadSearcher->setItemData(2, Lead::CuteCharm);
    ui->comboBoxLeadSearcher->setItemData(3, Lead::None);

    ui->comboBoxLeadGenerator->addItem(tr("None"));
    ui->comboBoxLeadGenerator->addItems(Nature::getNatures());

    ui->comboBoxNatureGenerator->setup();
    ui->comboBoxNatureSearcher->setup();

    ui->comboBoxHiddenPowerGenerator->setup();
    ui->comboBoxHiddenPowerSearcher->setup();

    QAction *seedToTime = new QAction(tr("Generate times for seed"), this);
    connect(seedToTime, &QAction::triggered, this, &Stationary4::seedToTime);

    searcherMenu->addAction(seedToTime);

    QSettings setting;
    if (setting.contains("stationary4MinDelay")) ui->minDelay->setText(setting.value("stationary4MinDelay").toString());
    if (setting.contains("stationary4MaxDelay")) ui->maxDelay->setText(setting.value("stationary4MaxDelay").toString());
    if (setting.contains("stationary4MinFrame")) ui->minFrame->setText(setting.value("stationary4MinFrame").toString());
    if (setting.contains("stationary4MaxFrame")) ui->maxFrame->setText(setting.value("stationary4MaxFrame").toString());
}

void Stationary4::updateView(const QVector<Frame4> &frames, int progress)
{
    s->addItems(frames);
    ui->progressBar->setValue(progress);
}

void Stationary4::refreshProfiles()
{
    emit alertProfiles(4);
}

void Stationary4::on_generate_clicked()
{
    g->clear();
    g->setMethod(static_cast<Method>(ui->comboBoxMethodGenerator->currentData().toInt()));

    u32 seed = ui->initialSeedGenerator->text().toUInt(nullptr, 16);
    u32 startingFrame = ui->startingFrameGenerator->text().toUInt();
    u32 maxResults = ui->maxResultsGenerator->text().toUInt();
    u16 tid = ui->idGenerator->text().toUShort();
    u16 sid = ui->sidGenerator->text().toUShort();
    u32 offset = 0;
    if (ui->checkBoxDelayGenerator->isChecked())
    {
        offset = ui->delayGenerator->text().toUInt();
    }

    int genderRatioIndex = ui->comboBoxGenderRatioGenerator->currentIndex();
    Generator4 generator = Generator4(maxResults, startingFrame, seed, tid, sid, offset, static_cast<Method>(ui->comboBoxMethodGenerator->currentData().toInt()));
    FrameCompare compare = FrameCompare(ui->ivFilterGenerator->getEvals(), ui->ivFilterGenerator->getValues(),
                                        ui->comboBoxGenderGenerator->currentIndex(), genderRatioIndex, ui->comboBoxAbilityGenerator->currentIndex(),
                                        ui->comboBoxNatureGenerator->getChecked(), ui->comboBoxHiddenPowerGenerator->getChecked(),
                                        ui->checkBoxShinyGenerator->isChecked(), ui->checkBoxDisableGenerator->isChecked());

    generator.setEncounterType(Stationary);
    if (ui->pushButtonLeadGenerator->text() == tr("Cute Charm"))
    {
        generator.setLeadType(static_cast<Lead>(ui->comboBoxLeadGenerator->currentData().toInt()));
    }
    else
    {
        int num = ui->comboBoxLeadGenerator->currentIndex();
        if (num == 0)
        {
            generator.setLeadType(Lead::None);
        }
        else
        {
            generator.setLeadType(Lead::Synchronize);
            generator.setSynchNature(Nature::getAdjustedNature(static_cast<u32>(ui->comboBoxLeadGenerator->currentIndex() - 1)));
        }
    }

    QVector<Frame4> frames = generator.generate(compare);
    g->setModel(frames);
}

void Stationary4::on_search_clicked()
{
    s->clear();
    s->setMethod(static_cast<Method>(ui->comboBoxMethodSearcher->currentData().toInt()));

    ui->search->setEnabled(false);
    ui->cancel->setEnabled(true);

    u16 tid = ui->idSearcher->text().toUShort();
    u16 sid = ui->sidSearcher->text().toUShort();

    int genderRatioIndex = ui->comboBoxGenderRatioSearcher->currentIndex();
    FrameCompare compare = FrameCompare(ui->ivFilterSearcher->getEvals(), ui->ivFilterSearcher->getValues(), ui->comboBoxGenderSearcher->currentIndex(),
                                        genderRatioIndex, ui->comboBoxAbilitySearcher->currentIndex(), ui->comboBoxNatureSearcher->getChecked(),
                                        ui->comboBoxHiddenPowerSearcher->getChecked(), ui->checkBoxShinySearcher->isChecked(), false);
    Searcher4 searcher = Searcher4(tid, sid, static_cast<u32>(genderRatioIndex), ui->minDelay->text().toUInt(), ui->maxDelay->text().toUInt(), ui->minFrame->text().toUInt(), ui->maxFrame->text().toUInt(), compare, static_cast<Method>(ui->comboBoxMethodSearcher->currentData().toInt()));
    searcher.setLeadType(static_cast<Lead>(ui->comboBoxLeadSearcher->currentData().toInt()));

    QVector<u8> min = ui->ivFilterSearcher->getLower();
    QVector<u8> max = ui->ivFilterSearcher->getUpper();

    int maxProgress = 1;
    for (int i = 0; i < 6; i++)
    {
        maxProgress *= max[i] - min[i] + 1;
    }

    ui->progressBar->setValue(0);
    ui->progressBar->setMaximum(maxProgress);

    auto *search = new StationarySearcher4(searcher, min, max);
    auto *timer = new QTimer();

    connect(search, &StationarySearcher4::finished, timer, &QTimer::deleteLater);
    connect(search, &StationarySearcher4::finished, timer, &QTimer::stop);
    connect(search, &StationarySearcher4::finished, this, [ = ] { ui->search->setEnabled(true); ui->cancel->setEnabled(false); });
    connect(search, &StationarySearcher4::finished, this, [ = ] { updateView(search->getResults(), search->currentProgress()); });
    connect(timer, &QTimer::timeout, this, [ = ] { updateView(search->getResults(), search->currentProgress()); });
    connect(ui->cancel, &QPushButton::clicked, search, &StationarySearcher4::cancelSearch);

    search->start();
    timer->start(1000);
}

void Stationary4::on_comboBoxProfiles_currentIndexChanged(int index)
{
    if (index < 0)
    {
        return;
    }

    auto profile = profiles.at(index);
    QString tid = QString::number(profile.getTID());
    QString sid = QString::number(profile.getSID());

    ui->idGenerator->setText(tid);
    ui->sidGenerator->setText(sid);
    ui->idSearcher->setText(tid);
    ui->sidSearcher->setText(sid);
    ui->profileTID->setText(tid);
    ui->profileSID->setText(sid);
    ui->profileGame->setText(profile.getVersionString());

    bool flag = profile.getVersion() & Game::HGSS;

    ui->comboBoxMethodGenerator->clear();
    ui->comboBoxMethodGenerator->addItem(tr("Method 1"), Method::Method1);
    ui->comboBoxMethodGenerator->addItem(flag ? tr("Method K") : tr("Method J"), flag ? Method::MethodK : Method::MethodJ);
    ui->comboBoxMethodGenerator->addItem(tr("Wondercard IVs"), Method::WondercardIVs);

    ui->comboBoxMethodSearcher->clear();
    ui->comboBoxMethodSearcher->addItem(tr("Method 1"), Method::Method1);
    ui->comboBoxMethodSearcher->addItem(flag ? tr("Method K") : tr("Method J"), flag ? Method::MethodK : Method::MethodJ);
    ui->comboBoxMethodSearcher->addItem(tr("Wondercard IVs"), Method::WondercardIVs);
}

void Stationary4::on_pushButtonLeadGenerator_clicked()
{
    ui->comboBoxLeadGenerator->clear();
    QString text = ui->pushButtonLeadGenerator->text();
    if (text == tr("Synchronize"))
    {
        ui->pushButtonLeadGenerator->setText(tr("Cute Charm"));
        ui->comboBoxLeadGenerator->setEnabled(true);

        ui->comboBoxLeadGenerator->addItem(tr("♂ Lead"), Lead::CuteCharmFemale);
        ui->comboBoxLeadGenerator->addItem(tr("♀ Lead (50% ♂ Target)"), Lead::CuteCharm50M);
        ui->comboBoxLeadGenerator->addItem(tr("♀ Lead (75% ♂ Target)"), Lead::CuteCharm75M);
        ui->comboBoxLeadGenerator->addItem(tr("♀ Lead (25% ♂ Target)"), Lead::CuteCharm25M);
        ui->comboBoxLeadGenerator->addItem(tr("♀ Lead (87.5% ♂ Target)"), Lead::CuteCharm875M);
    }
    else
    {
        ui->pushButtonLeadGenerator->setText(tr("Synchronize"));

        ui->comboBoxLeadGenerator->addItem("None");
        ui->comboBoxLeadGenerator->addItems(Nature::getNatures());
    }
}

void Stationary4::on_anyNatureGenerator_clicked()
{
    ui->comboBoxNatureGenerator->uncheckAll();
}

void Stationary4::on_anyHiddenPowerGenerator_clicked()
{
    ui->comboBoxHiddenPowerGenerator->uncheckAll();
}

void Stationary4::on_anyNatureSearcher_clicked()
{
    ui->comboBoxNatureSearcher->uncheckAll();
}

void Stationary4::on_anyHiddenPowerSearcher_clicked()
{
    ui->comboBoxHiddenPowerSearcher->uncheckAll();
}

void Stationary4::seedToTime()
{
    QModelIndex index = ui->tableViewSearcher->currentIndex();
    auto *time = new SeedtoTime4(s->data(s->index(index.row(), 0), Qt::DisplayRole).toString(), profiles[ui->comboBoxProfiles->currentIndex()]);
    time->show();
    time->raise();
}

void Stationary4::on_tableViewSearcher_customContextMenuRequested(const QPoint &pos)
{
    if (s->rowCount() == 0)
    {
        return;
    }

    searcherMenu->popup(ui->tableViewSearcher->viewport()->mapToGlobal(pos));
}

void Stationary4::on_pushButtonProfileManager_clicked()
{
    auto *manager = new ProfileManager4();
    connect(manager, &ProfileManager4::updateProfiles, this, &Stationary4::refreshProfiles);
    manager->show();
}


StationarySearcher4::StationarySearcher4(const Searcher4 &searcher, const QVector<u8> &min, const QVector<u8> &max)
{
    this->searcher = searcher;
    this->min = min;
    this->max = max;
    cancel = false;
    progress = 0;

    connect(this, &StationarySearcher4::finished, this, &StationarySearcher4::deleteLater);
}

void StationarySearcher4::run()
{
    for (u8 a = min[0]; a <= max[0]; a++)
    {
        for (u8 b = min[1]; b <= max[1]; b++)
        {
            for (u8 c = min[2]; c <= max[2]; c++)
            {
                for (u8 d = min[3]; d <= max[3]; d++)
                {
                    for (u8 e = min[4]; e <= max[4]; e++)
                    {
                        for (u8 f = min[5]; f <= max[5]; f++)
                        {
                            if (cancel)
                            {
                                return;
                            }

                            progress++;

                            QMutexLocker locker(&mutex);
                            results.append(searcher.search(a, b, c, d, e, f));
                        }
                    }
                }
            }
        }
    }
}

int StationarySearcher4::currentProgress()
{
    return progress;
}

QVector<Frame4> StationarySearcher4::getResults()
{
    QMutexLocker locker(&mutex);
    auto data(results);
    results.clear();
    return data;
}

void StationarySearcher4::cancelSearch()
{
    cancel = true;
}
