/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2020 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "Researcher.hpp"
#include "ui_Researcher.h"
#include <Core/RNG/LCRNG.hpp>
#include <Core/RNG/LCRNG64.hpp>
#include <Core/RNG/MT.hpp>
#include <Core/RNG/SFMT.hpp>
#include <Core/RNG/TinyMT.hpp>
#include <Forms/Models/Util/ResearcherModel.hpp>
#include <QSettings>
#include <array>
#include <functional>

Researcher::Researcher(QWidget *parent) : QWidget(parent), ui(new Ui::Researcher)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    setupModels();
}

Researcher::~Researcher()
{
    QSettings setting;
    setting.setValue("researcher/geometry", this->saveGeometry());

    delete ui;
}

void Researcher::setupModels()
{
    model = new ResearcherModel(ui->tableView, false);
    ui->tableView->setModel(model);
    resizeHeader();

    menu = new QMenu(ui->tableView);

    ui->textBoxInitialAdvances->setValues(InputType::Advance64Bit);
    ui->textBoxMaxAdvances->setValues(InputType::Advance64Bit);
    ui->textBoxSeed->setValues(InputType::Seed64Bit);
    ui->textBoxSearch->setValues(InputType::Seed64Bit);

    ui->textBoxStatus3->setValues(InputType::Seed32Bit);
    ui->textBoxStatus2->setValues(InputType::Seed32Bit);
    ui->textBoxStatus1->setValues(InputType::Seed32Bit);
    ui->textBoxStatus0->setValues(InputType::Seed32Bit);

    ui->lineEditRValue1->setValues(1, 0xffffffff, 10, 16);
    ui->lineEditRValue2->setValues(1, 0xffffffff, 10, 16);
    ui->lineEditRValue3->setValues(1, 0xffffffff, 10, 16);
    ui->lineEditRValue4->setValues(1, 0xffffffff, 10, 16);
    ui->lineEditRValue5->setValues(1, 0xffffffff, 10, 16);
    ui->lineEditRValue6->setValues(1, 0xffffffff, 10, 16);
    ui->lineEditRValue7->setValues(1, 0xffffffff, 10, 16);
    ui->lineEditRValue8->setValues(1, 0xffffffff, 10, 16);
    ui->lineEditRValue9->setValues(1, 0xffffffff, 10, 16);
    ui->lineEditRValue10->setValues(1, 0xffffffff, 10, 16);

    keys[tr("64Bit")] = 0;
    keys[tr("32Bit")] = 1;
    keys[tr("32Bit High")] = 2;
    keys[tr("32Bit Low")] = 3;
    keys[tr("16Bit High")] = 4;
    keys[tr("16Bit Low")] = 5;
    keys[tr("Custom 1")] = 6;
    keys[tr("Custom 2")] = 7;
    keys[tr("Custom 3")] = 8;
    keys[tr("Custom 4")] = 9;
    keys[tr("Custom 5")] = 10;
    keys[tr("Custom 6")] = 11;
    keys[tr("Custom 7")] = 12;
    keys[tr("Custom 8")] = 13;
    keys[tr("Custom 9")] = 14;
    keys[tr("Previous 1")] = 15;
    keys[tr("Previous 2")] = 16;
    keys[tr("Previous 3")] = 17;
    keys[tr("Previous 4")] = 18;
    keys[tr("Previous 5")] = 19;
    keys[tr("Previous 6")] = 20;
    keys[tr("Previous 7")] = 21;
    keys[tr("Previous 8")] = 22;
    keys[tr("Previous 9")] = 23;

    QAction *outputTXT = menu->addAction(tr("Output Results to TXT"));
    QAction *outputCSV = menu->addAction(tr("Output Results to CSV"));
    connect(outputTXT, &QAction::triggered, this, [=] { ui->tableView->outputModel(); });
    connect(outputCSV, &QAction::triggered, this, [=] { ui->tableView->outputModel(true); });

    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &Researcher::generate);
    connect(ui->rngSelection, &QTabWidget::currentChanged, this, &Researcher::selectionIndexChanged);
    connect(ui->pushButtonSearch, &QPushButton::clicked, this, &Researcher::search);
    connect(ui->pushButtonNext, &QPushButton::clicked, this, &Researcher::next);
    connect(ui->tableView, &QTableView::customContextMenuRequested, this, &Researcher::tableViewContextMenu);

    QSettings setting;
    if (setting.contains("researcher/geometry"))
    {
        this->restoreGeometry(setting.value("researcher/geometry").toByteArray());
    }
}

u64 Researcher::getCustom(const QString &text, const ResearcherState &state, const std::vector<ResearcherState> &states)
{
    switch (keys[text])
    {
    case 0:
    case 1:
        return state.getState();
    case 2:
        return state.getHigh32();
    case 3:
        return state.getLow32();
    case 4:
        return state.getHigh16();
    case 5:
        return state.getLow16();
    case 6:
        return state.getCustom(0);
    case 7:
        return state.getCustom(1);
    case 8:
        return state.getCustom(2);
    case 9:
        return state.getCustom(3);
    case 10:
        return state.getCustom(4);
    case 11:
        return state.getCustom(5);
    case 12:
        return state.getCustom(6);
    case 13:
        return state.getCustom(7);
    case 14:
        return state.getCustom(8);
    case 15:
        return states.empty() ? 0 : states[states.size() - 1].getCustom(0);
    case 16:
        return states.empty() ? 0 : states[states.size() - 1].getCustom(1);
    case 17:
        return states.empty() ? 0 : states[states.size() - 1].getCustom(2);
    case 18:
        return states.empty() ? 0 : states[states.size() - 1].getCustom(3);
    case 19:
        return states.empty() ? 0 : states[states.size() - 1].getCustom(4);
    case 20:
        return states.empty() ? 0 : states[states.size() - 1].getCustom(5);
    case 21:
        return states.empty() ? 0 : states[states.size() - 1].getCustom(6);
    case 22:
        return states.empty() ? 0 : states[states.size() - 1].getCustom(7);
    case 23:
        return states.empty() ? 0 : states[states.size() - 1].getCustom(8);
    default:
        return 0;
    }
}

void Researcher::resizeHeader()
{
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    for (int column = 0; column < model->columnCount(); column++)
    {
        int width = ui->tableView->horizontalHeader()->sectionSize(column);
        ui->tableView->horizontalHeader()->setSectionResizeMode(column, QHeaderView::Interactive);
        ui->tableView->horizontalHeader()->resizeSection(column, width);
    }
}

std::array<bool, 10> Researcher::getHexCheck()
{
    std::array<bool, 10> hex;

    hex[0] = ui->checkBoxHex1->isChecked();
    hex[1] = ui->checkBoxHex2->isChecked();
    hex[2] = ui->checkBoxHex3->isChecked();
    hex[3] = ui->checkBoxHex4->isChecked();
    hex[4] = ui->checkBoxHex5->isChecked();
    hex[5] = ui->checkBoxHex6->isChecked();
    hex[6] = ui->checkBoxHex7->isChecked();
    hex[7] = ui->checkBoxHex8->isChecked();
    hex[8] = ui->checkBoxHex9->isChecked();
    hex[9] = ui->checkBoxHex10->isChecked();

    return hex;
}

template <class RNGType>
std::vector<u64> getStates(RNGType rng, u32 initial, u32 max)
{
    std::vector<u64> states;

    rng.advance(initial);
    for (u32 i = 0; i < max; i++)
    {
        states.emplace_back(rng.next());
    }

    return states;
}

void Researcher::generate()
{
    bool rng64Bit = ui->rngSelection->currentIndex() == 1;

    model->clearModel();
    model->setFlag(rng64Bit);

    u64 seed = ui->textBoxSeed->text().toULongLong(nullptr, 16);
    u32 initialAdvances = ui->textBoxInitialAdvances->getUInt();
    u32 maxAdvances = ui->textBoxMaxAdvances->getUInt();

    if (ui->rngSelection->currentIndex() != 1 && (seed > 0xffffffff))
    {
        seed >>= 32;
    }

    std::vector<u64> rngStates;
    if (ui->rngSelection->currentIndex() == 0)
    {
        switch (ui->comboBoxRNG32Bit->currentIndex())
        {
        case 0:
            rngStates = getStates(PokeRNG(seed), initialAdvances, maxAdvances);
            rngStates.insert(rngStates.begin(), seed);
            break;
        case 1:
            rngStates = getStates(PokeRNGR(seed), initialAdvances, maxAdvances);
            rngStates.insert(rngStates.begin(), seed);
            break;
        case 2:
            rngStates = getStates(XDRNG(seed), initialAdvances, maxAdvances);
            rngStates.insert(rngStates.begin(), seed);
            break;
        case 3:
            rngStates = getStates(XDRNGR(seed), initialAdvances, maxAdvances);
            rngStates.insert(rngStates.begin(), seed);
            break;
        case 4:
            rngStates = getStates(ARNG(seed), initialAdvances, maxAdvances);
            rngStates.insert(rngStates.begin(), seed);
            break;
        case 5:
            rngStates = getStates(ARNGR(seed), initialAdvances, maxAdvances);
            rngStates.insert(rngStates.begin(), seed);
            break;
        case 6:
            rngStates = getStates(MT(seed), initialAdvances, maxAdvances);
            break;
        }
    }
    else if (ui->rngSelection->currentIndex() == 1)
    {
        switch (ui->comboBoxRNG64Bit->currentIndex())
        {
        case 0:
            rngStates = getStates(BWRNG(seed), initialAdvances, maxAdvances);
            rngStates.insert(rngStates.begin(), seed);
            break;
        case 1:
            rngStates = getStates(BWRNGR(seed), initialAdvances, maxAdvances);
            rngStates.insert(rngStates.begin(), seed);
            break;
        case 2:
            if (seed > 0xffffffff)
            {
                seed >>= 32;
            }
            rngStates = getStates(SFMT(seed), initialAdvances, maxAdvances);
            break;
        }
    }
    else
    {
        u32 status[4] = { ui->textBoxStatus0->getUInt(), ui->textBoxStatus1->getUInt(), ui->textBoxStatus2->getUInt(),
                          ui->textBoxStatus3->getUInt() };

        if (std::all_of(std::begin(status), std::end(status), [](u32 x) { return x == 0; }))
        {
            rngStates = getStates(TinyMT(seed), initialAdvances, maxAdvances);
        }
        else
        {
            rngStates = getStates(TinyMT(status), initialAdvances, maxAdvances);
        }
    }

    QHash<QString, std::function<u64(u64, u64)>> calc;
    calc["/"] = [](u64 x, u64 y) { return y == 0 ? 0 : x / y; };
    calc["%"] = [](u64 x, u64 y) { return x % y; };
    calc[">>"] = [](u64 x, u64 y) { return x >> y; };
    calc["<<"] = [](u64 x, u64 y) { return x << y; };
    calc["&"] = [](u64 x, u64 y) { return x & y; };
    calc["|"] = [](u64 x, u64 y) { return x | y; };
    calc["^"] = [](u64 x, u64 y) { return x ^ y; };
    calc["+"] = [](u64 x, u64 y) { return x + y; };
    calc["-"] = [](u64 x, u64 y) { return x - y; };
    calc["*"] = [](u64 x, u64 y) { return x * y; };

    std::array<bool, 10> calcCustom;
    std::array<u64, 10> customRValue;
    std::array<bool, 10> pass;
    std::array<std::function<u64(u64, u64)>, 10> calculators;

    calcCustom[0] = ui->lineEditRValue1->text() != "";
    calcCustom[1] = ui->lineEditRValue2->text() != "" || ui->comboBoxRValue2->currentIndex() != 0;
    calcCustom[2] = ui->lineEditRValue3->text() != "" || ui->comboBoxRValue3->currentIndex() != 0;
    calcCustom[3] = ui->lineEditRValue4->text() != "" || ui->comboBoxRValue4->currentIndex() != 0;
    calcCustom[4] = ui->lineEditRValue5->text() != "" || ui->comboBoxRValue5->currentIndex() != 0;
    calcCustom[5] = ui->lineEditRValue6->text() != "" || ui->comboBoxRValue6->currentIndex() != 0;
    calcCustom[6] = ui->lineEditRValue7->text() != "" || ui->comboBoxRValue7->currentIndex() != 0;
    calcCustom[7] = ui->lineEditRValue8->text() != "" || ui->comboBoxRValue8->currentIndex() != 0;
    calcCustom[8] = ui->lineEditRValue9->text() != "" || ui->comboBoxRValue9->currentIndex() != 0;
    calcCustom[9] = ui->lineEditRValue10->text() != "" || ui->comboBoxRValue10->currentIndex() != 0;

    customRValue[0] = ui->checkBoxHex1->isChecked() ? ui->lineEditRValue1->text().toULongLong(&pass[0], 16)
                                                    : ui->lineEditRValue1->text().toULongLong(&pass[0]);
    customRValue[1] = ui->checkBoxHex2->isChecked() ? ui->lineEditRValue2->text().toULongLong(&pass[1], 16)
                                                    : ui->lineEditRValue2->text().toULongLong(&pass[1]);
    customRValue[2] = ui->checkBoxHex3->isChecked() ? ui->lineEditRValue3->text().toULongLong(&pass[2], 16)
                                                    : ui->lineEditRValue3->text().toULongLong(&pass[2]);
    customRValue[3] = ui->checkBoxHex4->isChecked() ? ui->lineEditRValue4->text().toULongLong(&pass[3], 16)
                                                    : ui->lineEditRValue4->text().toULongLong(&pass[3]);
    customRValue[4] = ui->checkBoxHex5->isChecked() ? ui->lineEditRValue5->text().toULongLong(&pass[4], 16)
                                                    : ui->lineEditRValue5->text().toULongLong(&pass[4]);
    customRValue[5] = ui->checkBoxHex6->isChecked() ? ui->lineEditRValue6->text().toULongLong(&pass[5], 16)
                                                    : ui->lineEditRValue6->text().toULongLong(&pass[5]);
    customRValue[6] = ui->checkBoxHex7->isChecked() ? ui->lineEditRValue7->text().toULongLong(&pass[6], 16)
                                                    : ui->lineEditRValue7->text().toULongLong(&pass[6]);
    customRValue[7] = ui->checkBoxHex8->isChecked() ? ui->lineEditRValue8->text().toULongLong(&pass[7], 16)
                                                    : ui->lineEditRValue8->text().toULongLong(&pass[7]);
    customRValue[8] = ui->checkBoxHex9->isChecked() ? ui->lineEditRValue9->text().toULongLong(&pass[8], 16)
                                                    : ui->lineEditRValue9->text().toULongLong(&pass[8]);
    customRValue[9] = ui->checkBoxHex10->isChecked() ? ui->lineEditRValue10->text().toULongLong(&pass[9], 16)
                                                     : ui->lineEditRValue10->text().toULongLong(&pass[9]);

    for (int i = 0; i < 10; i++)
    {
        if (calcCustom[i] && !pass[i])
        {
            QMessageBox error;
            error.setText(tr("You must check the Hex box in order to use Hex values."));
            error.exec();
            return;
        }
    }

    calculators[0] = calc[ui->comboBoxOperator1->currentText()];
    calculators[1] = calc[ui->comboBoxOperator2->currentText()];
    calculators[2] = calc[ui->comboBoxOperator3->currentText()];
    calculators[3] = calc[ui->comboBoxOperator4->currentText()];
    calculators[4] = calc[ui->comboBoxOperator5->currentText()];
    calculators[5] = calc[ui->comboBoxOperator6->currentText()];
    calculators[6] = calc[ui->comboBoxOperator7->currentText()];
    calculators[7] = calc[ui->comboBoxOperator8->currentText()];
    calculators[8] = calc[ui->comboBoxOperator9->currentText()];
    calculators[9] = calc[ui->comboBoxOperator10->currentText()];

    QStringList textL = { ui->comboBoxLValue1->currentText(), ui->comboBoxLValue2->currentText(), ui->comboBoxLValue3->currentText(),
                          ui->comboBoxLValue4->currentText(), ui->comboBoxLValue5->currentText(), ui->comboBoxLValue6->currentText(),
                          ui->comboBoxLValue7->currentText(), ui->comboBoxLValue8->currentText(), ui->comboBoxLValue9->currentText(),
                          ui->comboBoxLValue10->currentText() };

    QStringList textR = { tr("None"),
                          ui->comboBoxRValue2->currentText(),
                          ui->comboBoxRValue3->currentText(),
                          ui->comboBoxRValue4->currentText(),
                          ui->comboBoxRValue5->currentText(),
                          ui->comboBoxRValue6->currentText(),
                          ui->comboBoxRValue7->currentText(),
                          ui->comboBoxRValue8->currentText(),
                          ui->comboBoxRValue9->currentText(),
                          ui->comboBoxRValue10->currentText() };

    std::vector<ResearcherState> states;
    for (u32 cnt = 0; cnt < maxAdvances; cnt++)
    {
        ResearcherState state(rng64Bit, cnt + initialAdvances);

        state.setState(rngStates[cnt]);

        for (u8 j = 0; j < 10; j++)
        {
            if (calcCustom[j])
            {
                u64 temp = getCustom(textL[j], state, states);

                if (textR[j] != tr("None"))
                {
                    customRValue[j] = getCustom(textR[j], state, states);
                }

                state.setCustom(j, calculators[j](temp, customRValue[j]));
            }
        }
        states.emplace_back(state);
    }

    model->setHex(getHexCheck());
    model->addItems(states);

    for (int i = 1; i < (rng64Bit ? 4 : 2); i++)
    {
        ui->tableView->horizontalHeader()->setSectionResizeMode(i, QHeaderView::ResizeToContents);
        int width = ui->tableView->horizontalHeader()->sectionSize(i);
        ui->tableView->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Interactive);
        ui->tableView->horizontalHeader()->resizeSection(i, width);
    }
}

void Researcher::selectionIndexChanged(int index)
{
    if (index >= 0)
    {
        ui->comboBoxSearch->clear();
        QStringList items = index != 1
            ? QStringList() << tr("32Bit") << tr("16Bit High") << tr("16Bit Low")
            : QStringList() << tr("64Bit") << tr("32Bit High") << tr("32Bit Low") << tr("16Bit High") << tr("16Bit Low");
        ui->comboBoxSearch->addItems(items);
    }
}

void Researcher::search()
{
    if (model->rowCount() == 0)
    {
        return;
    }

    QString string = ui->comboBoxSearch->currentText();
    u64 result = ui->textBoxSearch->text().toULongLong(nullptr, 16);

    QModelIndex end = model->search(string, result, 0);
    if (end.isValid())
    {
        ui->tableView->setCurrentIndex(end);
        ui->tableView->scrollTo(end);
        ui->tableView->setFocus();
    }
    else
    {
        QMessageBox error;
        error.setText(tr("No result found"));
        error.exec();
        return;
    }
}

void Researcher::next()
{
    if (model->rowCount() == 0)
    {
        return;
    }

    QString string = ui->comboBoxSearch->currentText();
    QModelIndex start = ui->tableView->currentIndex();
    u64 result = ui->textBoxSearch->text().toULongLong(nullptr, 16);

    if (!start.isValid())
    {
        return;
    }

    QModelIndex end = model->search(string, result, start.row() + 1);
    if (end.isValid())
    {
        ui->tableView->setCurrentIndex(end);
        ui->tableView->scrollTo(end);
        ui->tableView->setFocus();
    }
    else
    {
        QMessageBox error;
        error.setText(tr("No result found"));
        error.exec();
        return;
    }
}

void Researcher::tableViewContextMenu(QPoint pos)
{
    if (model->rowCount() > 0)
    {
        menu->popup(ui->tableView->viewport()->mapToGlobal(pos));
    }
}
