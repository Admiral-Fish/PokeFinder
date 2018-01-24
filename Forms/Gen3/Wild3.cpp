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

#include "Wild3.hpp"
#include "ui_Wild3.h"

Wild3::Wild3(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Wild3)
{
    ui->setupUi(this);

    SetupModels();
    UpdateProfiles();

    QFile file(QApplication::applicationDirPath() + "/profiles.xml");

    if(!file.exists())
        CreateProfileXml();

    qRegisterMetaType<vector<Frame3>>("vector<Frame3>");
    connect(this, SIGNAL(UpdateView(vector<Frame3>)), this, SLOT(UpdateViewSearcher(vector<Frame3>)));
}

Wild3::~Wild3()
{
    delete ui;
    if (s != NULL)
    {
        delete s;
        s = NULL;
    }
    if (g != NULL)
    {
        delete g;
        g = NULL;
    }
}

void Wild3::on_checkBoxDelayGenerator_clicked()
{
    if (ui->checkBoxDelayGenerator->isChecked())
    {
        ui->delayGenerator->setEnabled(true);
    }
    else
    {
        ui->delayGenerator->setEnabled(false);
        ui->delayGenerator->setText("");
    }
}

void Wild3::on_checkBoxDelaySearcher_clicked()
{
    if (ui->checkBoxDelaySearcher->isChecked())
    {
        ui->delaySearcher->setEnabled(true);
    }
    else
    {
        ui->delaySearcher->setEnabled(false);
        ui->delaySearcher->setText("");
    }
}

void Wild3::UpdateViewSearcher(vector<Frame3> frames)
{
    s->AddItems(frames);
    ui->tableViewSearcher->viewport()->update();
}

void Wild3::UpdateProfiles()
{
    profiles = Profile3::LoadProfileList();

    QStandardItemModel *profile = new QStandardItemModel((int)profiles.size() + 1, 1, this);
    QStandardItem* firstProfile = new QStandardItem(tr("None"));
    profile->setItem(0, firstProfile);
    for(int i = 0; i < (int)profiles.size(); i++)
    {
        QStandardItem* item = new QStandardItem(profiles.at(i).profileName);
        profile->setItem(i + 1, item);
    }
    ui->comboBoxProfiles->setModel(profile);
}

void Wild3::SetupModels()
{
    vector<QString> natureList = Nature::GetNatures();
    ui->comboBoxNatureGenerator->AddCheckItems(natureList, QVariant(), Qt::Unchecked);
    ui->comboBoxNatureSearcher->AddCheckItems(natureList, QVariant(), Qt::Unchecked);

    vector<QString> powerList = Power::GetPowers();
    ui->comboBoxHiddenPowerGenerator->AddCheckItems(powerList, QVariant(), Qt::Unchecked);
    ui->comboBoxHiddenPowerSearcher->AddCheckItems(powerList, QVariant(), Qt::Unchecked);

    if (g != NULL)
        delete g;
    g = new Wild3Model(this);
    ui->tableViewGenerator->setModel(g);
    ui->tableViewGenerator->verticalHeader()->setVisible(false);
    ui->tableViewGenerator->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    if (s != NULL)
        delete s;
    s = new Searcher3Model(this, MethodH1);
    ui->tableViewSearcher->setModel(s);
    ui->tableViewSearcher->verticalHeader()->setVisible(false);
    ui->tableViewSearcher->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->comboBoxMethodGenerator->clear();
    ui->comboBoxMethodGenerator->addItem(tr("Method H1"), MethodH1);
    ui->comboBoxMethodGenerator->addItem(tr("Method H2"), MethodH2);
    ui->comboBoxMethodGenerator->addItem(tr("Method H4"), MethodH4);

    ui->comboBoxMethodSearcher->clear();
    ui->comboBoxMethodSearcher->addItem(tr("Method H1"), MethodH1);
    ui->comboBoxMethodSearcher->addItem(tr("Method H2"), MethodH2);
    ui->comboBoxMethodSearcher->addItem(tr("Method H4"), MethodH4);

    ui->initialSeedGenerator->SetValues("[^0-9A-F]", 0, 0xffffffff, 16);
    ui->idGenerator->SetValues("[^0-9]", 0, 0xffff, 10);
    ui->sidGenerator->SetValues("[^0-9]", 0, 0xffff, 10);
    ui->startingFrameGenerator->SetValues("[^0-9]", 1, 0xffffffff, 10);
    ui->maxResultsGenerator->SetValues("[^0-9]", 1, 0xffffffff, 10);
    ui->delayGenerator->SetValues("[^0-9]", 0, 0xffffffff, 10);

    ui->idSearcher->SetValues("[^0-9]", 0, 0xffff, 10);
    ui->sidSearcher->SetValues("[^0-9]", 0, 0xffff, 10);
}

void Wild3::CreateProfileXml()
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

void Wild3::on_saveProfileGenerator_clicked()
{
    ProfileManager3* manager = new ProfileManager3();
    manager->setAttribute(Qt::WA_QuitOnClose, false);
    connect(manager, SIGNAL(UpdateProfiles()), this, SLOT(UpdateProfiles()));
    manager->show();
}


void Wild3::on_saveSearcher_clicked()
{
    ProfileManager3* manager = new ProfileManager3();
    manager->setAttribute(Qt::WA_QuitOnClose, false);
    connect(manager, SIGNAL(UpdateProfiles()), this, SLOT(UpdateProfiles()));
    manager->show();
}

void Wild3::on_anyNatureGenerator_clicked()
{
    ui->comboBoxNatureGenerator->UncheckAll();
}

void Wild3::on_anyHiddenPowerGenerator_clicked()
{
    ui->comboBoxHiddenPowerGenerator->UncheckAll();
}


void Wild3::on_anyNatureSearcher_clicked()
{
    ui->comboBoxNatureSearcher->UncheckAll();
}

void Wild3::on_anyHiddenPowerSearcher_clicked()
{
    ui->comboBoxHiddenPowerSearcher->UncheckAll();
}

void Wild3::on_generate_clicked()
{
    if (g != NULL)
        delete g;
    g = new Wild3Model(this);
    ui->tableViewGenerator->setModel(g);

    u32 seed = ui->initialSeedGenerator->text().toUInt(NULL, 16);
    u32 startingFrame = ui->startingFrameGenerator->text().toUInt(NULL, 10);
    u32 maxResults = ui->maxResultsGenerator->text().toUInt(NULL, 10);
    u32 tid = ui->idGenerator->text().toUInt(NULL, 10);
    u32 sid = ui->sidGenerator->text().toUInt(NULL, 10);
    u32 offset = ui->delayGenerator->text().toUInt(NULL, 10);

    int genderRatioIndex = ui->comboBoxGenderRatioGenerator->currentIndex();
    Generator3 generator = Generator3(maxResults, startingFrame, seed, tid, sid, offset);
    FrameCompare compare = FrameCompare(ui->ivFilterGenerator->GetEvals(), ui->ivFilterGenerator->GetValues(),
                                        ui->comboBoxGenderGenerator->currentIndex(), genderRatioIndex, ui->comboBoxAbilityGenerator->currentIndex(),
                                        ui->comboBoxNatureGenerator->GetChecked(), ui->comboBoxHiddenPowerGenerator->GetChecked(),
                                        ui->checkBoxShinyGenerator->isChecked(), ui->checkBoxDisableGenerator->isChecked());

    generator.frameType = (Method)ui->comboBoxMethodGenerator->currentData().toInt(NULL);

    vector<Frame3> frames = generator.Generate(compare);
    g->SetModel(frames);
    ui->tableViewGenerator->viewport()->update();
}

void Wild3::on_search_clicked()
{
    if (search == true)
        return;
    if (s != NULL)
        delete s;
    s = new Searcher3Model(this, (Method)ui->comboBoxMethodSearcher->currentData().toInt(NULL));
    ui->tableViewSearcher->setModel(s);
    std::thread job(&Wild3::Search, this);
    job.detach();
}

void Wild3::Search()
{
    search = true;
    u32 tid = ui->idSearcher->text().toUInt(NULL, 10);
    u32 sid = ui->sidSearcher->text().toUInt(NULL, 10);

    vector<u32> eval = ui->ivFilterSearcher->GetEvals();
    vector<u32> ivs = ui->ivFilterSearcher->GetValues();

    int genderRatioIndex = ui->comboBoxGenderRatioSearcher->currentIndex();
    Searcher3 searcher = Searcher3(tid, sid);
    FrameCompare compare = FrameCompare(eval, ivs, ui->comboBoxGenderSearcher->currentIndex(), genderRatioIndex,
                                        ui->comboBoxAbilitySearcher->currentIndex(), ui->comboBoxNatureSearcher->GetChecked(),
                                        ui->comboBoxHiddenPowerSearcher->GetChecked(), ui->checkBoxShinySearcher->isChecked(), false);

    searcher.frameType = (Method)ui->comboBoxMethodSearcher->currentData().toInt(NULL);

    u32 min[6], max[6];

    for (int i = 0; i < 6; i++)
    {
        switch (eval[i])
        {
            case 0:
                min[i] = 0;
                max[i] = 31;
                break;
            case 1:
                min[i] = max[i] = ivs[i];
                break;
            case 2:
                min[i] = ivs[i];
                max[i] = 31;
                break;
            case 3:
                min[i] = 0;
                max[i] = ivs[i];
                break;
        }
    }

    for (u32 a = min[0]; a <= max[0]; a++)
    {
        for (u32 b = min[1]; b <= max[1]; b++)
        {
            for (u32 c = min[2]; c <= max[2]; c++)
            {
                for (u32 d = min[3]; d <= max[3]; d++)
                {
                    for (u32 e = min[4]; e <= max[4]; e++)
                    {
                        for (u32 f = min[5]; f <= max[5]; f++)
                        {
                            vector<Frame3> frames = searcher.Search(a, b, c, d, e, f, compare);

                            if (!frames.empty())
                                emit UpdateView(frames);
                        }
                    }
                }
            }
        }
    }
    search = false;
}
