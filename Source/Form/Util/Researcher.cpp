/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2023 by Admiral_Fish, bumba, and EzPzStreamz
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
#include <Core/RNG/Xoroshiro.hpp>
#include <Core/RNG/Xorshift.hpp>
#include <Model/Util/ResearcherModel.hpp>
#include <QMessageBox>
#include <QSettings>

/**
 * @brief Computes the PRNG states for \p rng
 *
 * @tparam RNGType Type of RNG
 * @tparam lcrng Whether the RNGType if LCRNG/LCRNG64 or not
 * @param rng RNG object to generate numbers
 * @param initial Initial advances
 * @param max Maximum advances
 *
 * @return Vector of PRNG states
 */
template <class RNGType>
static std::vector<u64> getStates(RNGType rng, u32 initial, u32 max)
{
    std::vector<u64> states;

    if constexpr (std::is_same<RNGType, ARNG>::value || std::is_same<RNGType, ARNGR>::value || std::is_same<RNGType, BWRNG>::value
                  || std::is_same<RNGType, BWRNGR>::value || std::is_same<RNGType, PokeRNG>::value || std::is_same<RNGType, PokeRNGR>::value
                  || std::is_same<RNGType, TinyMT>::value || std::is_same<RNGType, XDRNG>::value || std::is_same<RNGType, XDRNGR>::value
                  || std::is_same<RNGType, Xorshift>::value || std::is_same<RNGType, Xoroshiro>::value
                  || std::is_same<RNGType, XoroshiroBDSP>::value)
    {
        rng.jump(initial);
    }
    else
    {
        rng.advance(initial);
    }

    for (u32 i = 0; i < max; i++)
    {
        states.emplace_back(rng.next());
    }

    return states;
}

Researcher::Researcher(QWidget *parent) : QWidget(parent), ui(new Ui::Researcher)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    model = new ResearcherModel(ui->tableView, false);
    ui->tableView->setModel(model);

    ui->textBoxInitialAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxMaxAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxSearch->setValues(InputType::Seed64Bit);

    ui->textBox32BitSeed->setValues(InputType::Seed32Bit);

    ui->textBox64BitSeed->setValues(InputType::Seed64Bit);

    ui->textBoxTinyMTSeed0->setValues(InputType::Seed32Bit);
    ui->textBoxTinyMTSeed1->setValues(InputType::Seed32Bit);
    ui->textBoxTinyMTSeed2->setValues(InputType::Seed32Bit);
    ui->textBoxTinyMTSeed3->setValues(InputType::Seed32Bit);

    ui->textBoxXorshiftSeed0->setValues(InputType::Seed64Bit);
    ui->textBoxXorshiftSeed1->setValues(InputType::Seed64Bit);

    ui->comboBoxSearch->setup({ toInt(Custom::Full64Bit), toInt(Custom::Full32Bit), toInt(Custom::High32Bit), toInt(Custom::Low32Bit),
                                toInt(Custom::High16Bit), toInt(Custom::Low16Bit), toInt(Custom::Custom1), toInt(Custom::Custom2),
                                toInt(Custom::Custom3), toInt(Custom::Custom4), toInt(Custom::Custom5), toInt(Custom::Custom6),
                                toInt(Custom::Custom7), toInt(Custom::Custom8), toInt(Custom::Custom9), toInt(Custom::Custom10) });

    std::vector<QVariant> customs = { toInt(Custom::Full64Bit),         toInt(Custom::Full32Bit),         toInt(Custom::High32Bit),
                                      toInt(Custom::Low32Bit),          toInt(Custom::High16Bit),         toInt(Custom::Low16Bit),
                                      toInt(Custom::PreviousFull64Bit), toInt(Custom::PreviousFull32Bit), toInt(Custom::PreviousHigh32Bit),
                                      toInt(Custom::PreviousLow32Bit),  toInt(Custom::PreviousHigh16Bit), toInt(Custom::PreviousLow16Bit) };
    QStringList operands = { "*", "/", "%", "+", "-", "<<", ">>", "<", "<=", ">", ">=", "&", "^", "|" };
    for (int i = 1; i <= 10; i++)
    {
        auto *box = ui->groupBoxCustoms->findChild<ComboBox *>(QString("comboBoxOperator%1").arg(i));
        box->addItems(operands);

        auto *text = ui->groupBoxCustoms->findChild<TextBox *>(QString("textBoxRValue%1").arg(i));
        text->setValues(1, 0xffffffff, 10, 16);

        auto *left = ui->groupBoxCustoms->findChild<ComboBox *>(QString("comboBoxLValue%1").arg(i));
        left->setup(customs);

        if (i != 1)
        {
            // Add/remove None from the container. Only needed for right side combo boxes
            customs.insert(customs.begin(), toInt(Custom::None));
            auto *right = ui->groupBoxCustoms->findChild<ComboBox *>(QString("comboBoxRValue%1").arg(i));
            right->setup(customs);
            customs.erase(customs.begin());
        }

        customs.emplace_back(toInt(Custom::Custom1) + i - 1);
        customs.emplace_back(toInt(Custom::Previous1) + i - 1);
    }

    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &Researcher::generate);
    connect(ui->rngSelection, &QTabWidget::currentChanged, this, &Researcher::rngSelectionIndexChanged);
    connect(ui->pushButtonSearch, &QPushButton::clicked, this, &Researcher::search);
    connect(ui->pushButtonNext, &QPushButton::clicked, this, &Researcher::next);

    rngSelectionIndexChanged(0);

    QSettings setting;
    if (setting.contains("researcher/geometry"))
    {
        this->restoreGeometry(setting.value("researcher/geometry").toByteArray());
    }
}

Researcher::~Researcher()
{
    QSettings setting;
    setting.setValue("researcher/geometry", this->saveGeometry());

    delete ui;
}

u64 Researcher::getCustom(Custom custom, const ResearcherState &state, const std::vector<ResearcherState> &states)
{
    switch (custom)
    {
    case Custom::Full64Bit:
    case Custom::Full32Bit:
        return state.getPRNG();
    case Custom::High32Bit:
        return state.getHigh32();
    case Custom::Low32Bit:
        return state.getLow32();
    case Custom::High16Bit:
        return state.getHigh16();
    case Custom::Low16Bit:
        return state.getLow16();
    case Custom::PreviousFull64Bit:
    case Custom::PreviousFull32Bit:
        return states.empty() ? 0 : states.back().getPRNG();
    case Custom::PreviousHigh32Bit:
        return states.empty() ? 0 : states.back().getHigh32();
    case Custom::PreviousLow32Bit:
        return states.empty() ? 0 : states.back().getLow32();
    case Custom::PreviousHigh16Bit:
        return states.empty() ? 0 : states.back().getHigh16();
    case Custom::PreviousLow16Bit:
        return states.empty() ? 0 : states.back().getLow16();
    case Custom::Custom1:
    case Custom::Custom2:
    case Custom::Custom3:
    case Custom::Custom4:
    case Custom::Custom5:
    case Custom::Custom6:
    case Custom::Custom7:
    case Custom::Custom8:
    case Custom::Custom9:
        return state.getCustom(toInt(custom) - toInt(Custom::Custom1));
    case Custom::Previous1:
    case Custom::Previous2:
    case Custom::Previous3:
    case Custom::Previous4:
    case Custom::Previous5:
    case Custom::Previous6:
    case Custom::Previous7:
    case Custom::Previous8:
    case Custom::Previous9:
        return states.empty() ? 0 : states.back().getCustom(toInt(custom) - toInt(Custom::Previous1));
    default:
        return 0;
    }
}

std::array<bool, 10> Researcher::getHexCheck()
{
    std::array<bool, 10> checked;
    for (int i = 0; i < 10; i++)
    {
        auto *hex = ui->groupBoxCustoms->findChild<QCheckBox *>(QString("checkBoxHex%1").arg(i + 1));
        checked[i] = hex->isChecked();
    }
    return checked;
}

void Researcher::generate()
{
    bool rng64Bit = ui->rngSelection->currentIndex() == 1;

    model->clearModel();
    model->setFlag(rng64Bit);

    u32 initialAdvances = ui->textBoxInitialAdvances->getUInt();
    u32 maxAdvances = ui->textBoxMaxAdvances->getUInt();

    std::vector<u64> rngStates;
    if (ui->rngSelection->currentIndex() == 0)
    {
        u32 seed = ui->textBox32BitSeed->getUInt();
        switch (ui->comboBoxRNG32Bit->currentIndex())
        {
        case 0:
            rngStates = getStates(PokeRNG(seed), initialAdvances, maxAdvances);
            break;
        case 1:
            rngStates = getStates(PokeRNGR(seed), initialAdvances, maxAdvances);
            break;
        case 2:
            rngStates = getStates(XDRNG(seed), initialAdvances, maxAdvances);
            break;
        case 3:
            rngStates = getStates(XDRNGR(seed), initialAdvances, maxAdvances);
            break;
        case 4:
            rngStates = getStates(ARNG(seed), initialAdvances, maxAdvances);
            break;
        case 5:
            rngStates = getStates(ARNGR(seed), initialAdvances, maxAdvances);
            break;
        case 6:
            rngStates = getStates<MT>(MT(seed), initialAdvances, maxAdvances);
            break;
        }
    }
    else if (ui->rngSelection->currentIndex() == 1)
    {
        u64 seed = ui->textBox64BitSeed->getULong();
        switch (ui->comboBoxRNG64Bit->currentIndex())
        {
        case 0:
            rngStates = getStates(BWRNG(seed), initialAdvances, maxAdvances);
            break;
        case 1:
            rngStates = getStates(BWRNGR(seed), initialAdvances, maxAdvances);
            break;
        case 2:
            if (seed > 0xffffffff)
            {
                seed >>= 32;
            }
            rngStates = getStates<SFMT>(SFMT(seed), initialAdvances, maxAdvances);
            break;
        case 3:
            rngStates = getStates<Xoroshiro>(Xoroshiro(seed), initialAdvances, maxAdvances);
            break;
        case 4:
            rngStates = getStates<XoroshiroBDSP>(XoroshiroBDSP(seed), initialAdvances, maxAdvances);
            break;
        }
    }
    else if (ui->rngSelection->currentIndex() == 2)
    {
        rngStates = getStates<TinyMT>(TinyMT(ui->textBoxTinyMTSeed0->getUInt(), ui->textBoxTinyMTSeed1->getUInt(),
                                             ui->textBoxTinyMTSeed2->getUInt(), ui->textBoxTinyMTSeed3->getUInt()),
                                      initialAdvances, maxAdvances);
    }
    else
    {
        u64 seed0 = ui->textBoxXorshiftSeed0->getULong();
        u64 seed1 = ui->textBoxXorshiftSeed1->getULong();
        rngStates = getStates<Xorshift>(Xorshift(seed0, seed1), initialAdvances, maxAdvances);
    }

    QHash<QString, u64 (*)(u64, u64)> calc;
    calc["*"] = [](u64 x, u64 y) { return x * y; };
    calc["/"] = [](u64 x, u64 y) { return y == 0 ? 0 : x / y; };
    calc["%"] = [](u64 x, u64 y) { return x % y; };
    calc["+"] = [](u64 x, u64 y) { return x + y; };
    calc["-"] = [](u64 x, u64 y) { return x - y; };
    calc[">>"] = [](u64 x, u64 y) { return x >> y; };
    calc["<<"] = [](u64 x, u64 y) { return x << y; };
    calc["<"] = [](u64 x, u64 y) { return static_cast<u64>(x < y); };
    calc["<="] = [](u64 x, u64 y) { return static_cast<u64>(x <= y); };
    calc[">"] = [](u64 x, u64 y) { return static_cast<u64>(x > y); };
    calc[">="] = [](u64 x, u64 y) { return static_cast<u64>(x >= y); };
    calc["&"] = [](u64 x, u64 y) { return x & y; };
    calc["^"] = [](u64 x, u64 y) { return x ^ y; };
    calc["|"] = [](u64 x, u64 y) { return x | y; };

    std::array<bool, 10> calcCustom;
    std::array<u64, 10> customRValue;
    std::array<u64 (*)(u64, u64), 10> calculators;
    Custom customL[10];
    Custom customR[10];
    for (int i = 0; i < 10; i++)
    {
        auto *boxL = ui->groupBoxCustoms->findChild<ComboBox *>(QString("comboBoxLValue%1").arg(i + 1));
        auto *operand = ui->groupBoxCustoms->findChild<QComboBox *>(QString("comboBoxOperator%1").arg(i + 1));
        auto *textR = ui->groupBoxCustoms->findChild<TextBox *>(QString("textBoxRValue%1").arg(i + 1));
        auto *hex = ui->groupBoxCustoms->findChild<QCheckBox *>(QString("checkBoxHex%1").arg(i + 1));
        auto *boxR = ui->groupBoxCustoms->findChild<ComboBox *>(QString("comboBoxRValue%1").arg(i + 1));

        calcCustom[i] = !textR->text().isEmpty() || (boxR && boxR->getEnum<Custom>() != Custom::None);

        calculators[i] = calc[operand->currentText()];

        customL[i] = boxL->getEnum<Custom>();
        customR[i] = boxR ? boxR->getEnum<Custom>() : Custom::None;

        bool pass;
        customRValue[i] = textR->text().toULongLong(&pass, hex->isChecked() ? 16 : 10);

        if (calcCustom[i] && (!pass && customR[i] == Custom::None))
        {
            QMessageBox msg(QMessageBox::Warning, tr("Missing setting"),
                            tr("You must check the Hex box in order to use hexadecimal values for Custom %1").arg(i + 1));
            msg.exec();
            return;
        }
    }

    std::vector<ResearcherState> states;
    for (u32 cnt = 0; cnt < maxAdvances; cnt++)
    {
        ResearcherState state(initialAdvances + cnt, rngStates[cnt], rng64Bit);
        for (u8 j = 0; j < 10; j++)
        {
            u64 custom = 0;
            if (calcCustom[j])
            {
                u64 temp = getCustom(customL[j], state, states);
                if (customR[j] != Custom::None)
                {
                    customRValue[j] = getCustom(customR[j], state, states);
                }
                custom = calculators[j](temp, customRValue[j]);
            }
            state.setCustom(j, custom);
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

void Researcher::next()
{
    if (model->rowCount() == 0)
    {
        return;
    }

    auto custom = ui->comboBoxSearch->getEnum<Custom>();
    QModelIndex start = ui->tableView->currentIndex();
    u64 result = ui->textBoxSearch->text().toULongLong(nullptr, 16);

    if (!start.isValid())
    {
        return;
    }

    QModelIndex end = model->search(custom, result, start.row() + 1);
    if (end.isValid())
    {
        ui->tableView->setCurrentIndex(end);
        ui->tableView->scrollTo(end);
        ui->tableView->setFocus();
    }
    else
    {
        QMessageBox msg(QMessageBox::Information, tr("No result"), tr("Unable to find a result"));
        msg.exec();
    }
}

void Researcher::search()
{
    if (model->rowCount() == 0)
    {
        return;
    }

    auto custom = ui->comboBoxSearch->getEnum<Custom>();
    u64 result = ui->textBoxSearch->text().toULongLong(nullptr, ui->checkBoxSearch->isChecked() ? 16 : 10);

    QModelIndex end = model->search(custom, result, 0);
    if (end.isValid())
    {
        ui->tableView->setCurrentIndex(end);
        ui->tableView->scrollTo(end);
        ui->tableView->setFocus();
    }
    else
    {
        QMessageBox msg(QMessageBox::Information, tr("No result"), tr("Unable to find a result"));
        msg.exec();
    }
}

void Researcher::rngSelectionIndexChanged(int index)
{
    if (index >= 0)
    {
        bool flag = index != 1;

        ui->comboBoxSearch->setItemHidden(0, flag);
        ui->comboBoxSearch->setItemHidden(1, !flag);
        ui->comboBoxSearch->setItemHidden(2, flag);
        ui->comboBoxSearch->setItemHidden(3, flag);

        for (int i = 1; i <= 10; i++)
        {
            auto *boxL = ui->groupBoxCustoms->findChild<ComboBox *>(QString("comboBoxLValue%1").arg(i));

            boxL->setItemHidden(0, flag);
            boxL->setItemHidden(1, !flag);
            boxL->setItemHidden(2, flag);
            boxL->setItemHidden(3, flag);

            if (i != 1)
            {
                auto *boxR = ui->groupBoxCustoms->findChild<ComboBox *>(QString("comboBoxRValue%1").arg(i));

                boxR->setItemHidden(1, flag);
                boxR->setItemHidden(2, !flag);
                boxR->setItemHidden(3, flag);
                boxR->setItemHidden(4, flag);
            }
        }
    }
}
