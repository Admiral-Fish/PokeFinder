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
#include <Core/RNG/MTRNG.hpp>
#include <Core/RNG/SFMT.hpp>
#include <Core/RNG/TinyMT.hpp>
#include <Models/Util/ResearcherModel.hpp>
#include <QSettings>
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

    ui->textBoxInitialFrame->setValues(InputType::Frame64Bit);
    ui->textBoxMaxResults->setValues(InputType::Frame64Bit);
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

    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &Researcher::generate);
    connect(ui->rngSelection, &QTabWidget::currentChanged, this, &Researcher::selectionIndexChanged);
    connect(ui->pushButtonSearch, &QPushButton::clicked, this, &Researcher::search);
    connect(ui->pushButtonNext, &QPushButton::clicked, this, &Researcher::next);

    QSettings setting;
    if (setting.contains("researcher/geometry"))
    {
        this->restoreGeometry(setting.value("researcher/geometry").toByteArray());
    }
}

u64 Researcher::getCustom(const QString &text, const ResearcherFrame &frame, const QVector<ResearcherFrame> &frames)
{
    switch (keys[text])
    {
    case 0:
        return frame.getFull64();
    case 1:
        return frame.getFull32();
    case 2:
        return frame.getHigh32();
    case 3:
        return frame.getLow32();
    case 4:
        return frame.getHigh16();
    case 5:
        return frame.getLow16();
    case 6:
        return frame.getCustom(0);
    case 7:
        return frame.getCustom(1);
    case 8:
        return frame.getCustom(2);
    case 9:
        return frame.getCustom(3);
    case 10:
        return frame.getCustom(4);
    case 11:
        return frame.getCustom(5);
    case 12:
        return frame.getCustom(6);
    case 13:
        return frame.getCustom(7);
    case 14:
        return frame.getCustom(8);
    case 15:
        return frames.isEmpty() ? 0 : frames[frames.size() - 1].getCustom(0);
    case 16:
        return frames.isEmpty() ? 0 : frames[frames.size() - 1].getCustom(1);
    case 17:
        return frames.isEmpty() ? 0 : frames[frames.size() - 1].getCustom(2);
    case 18:
        return frames.isEmpty() ? 0 : frames[frames.size() - 1].getCustom(3);
    case 19:
        return frames.isEmpty() ? 0 : frames[frames.size() - 1].getCustom(4);
    case 20:
        return frames.isEmpty() ? 0 : frames[frames.size() - 1].getCustom(5);
    case 21:
        return frames.isEmpty() ? 0 : frames[frames.size() - 1].getCustom(6);
    case 22:
        return frames.isEmpty() ? 0 : frames[frames.size() - 1].getCustom(7);
    case 23:
        return frames.isEmpty() ? 0 : frames[frames.size() - 1].getCustom(8);
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

QVector<bool> Researcher::getHexCheck()
{
    QVector<bool> hex;

    hex.append(ui->checkBoxHex1->isChecked());
    hex.append(ui->checkBoxHex2->isChecked());
    hex.append(ui->checkBoxHex3->isChecked());
    hex.append(ui->checkBoxHex4->isChecked());
    hex.append(ui->checkBoxHex5->isChecked());
    hex.append(ui->checkBoxHex6->isChecked());
    hex.append(ui->checkBoxHex7->isChecked());
    hex.append(ui->checkBoxHex8->isChecked());
    hex.append(ui->checkBoxHex9->isChecked());
    hex.append(ui->checkBoxHex10->isChecked());

    return hex;
}

void Researcher::generate()
{
    bool rng64Bit = ui->rngSelection->currentIndex() == 1;

    model->clearModel();
    model->setFlag(rng64Bit);

    u64 seed = ui->textBoxSeed->text().toULongLong(nullptr, 16);
    u32 initialFrame = ui->textBoxInitialFrame->getUInt();
    u32 maxResults = ui->textBoxMaxResults->getUInt();

    IRNG<u32> *rng = nullptr;
    IRNG<u64> *rng64 = nullptr;

    if (ui->rngSelection->currentIndex() != 1 && (seed > 0xffffffff))
    {
        seed >>= 32;
    }

    if (ui->rngSelection->currentIndex() == 0)
    {
        switch (ui->comboBoxRNG32Bit->currentIndex())
        {
        case 0:
            rng = new PokeRNG(static_cast<u32>(seed));
            break;
        case 1:
            rng = new PokeRNGR(static_cast<u32>(seed));
            break;
        case 2:
            rng = new XDRNG(static_cast<u32>(seed));
            break;
        case 3:
            rng = new XDRNGR(static_cast<u32>(seed));
            break;
        case 4:
            rng = new ARNG(static_cast<u32>(seed));
            break;
        case 5:
            rng = new ARNGR(static_cast<u32>(seed));
            break;
        case 6:
            rng = new MersenneTwister(static_cast<u32>(seed));
            break;
        case 7:
            rng = new MersenneTwisterUntempered(static_cast<u32>(seed));
            break;
        case 8:
            try
            {
                rng = new MersenneTwisterFast(initialFrame + maxResults, static_cast<u32>(seed));
            }
            catch (const std::runtime_error &e)
            {
                (void)e;
                QMessageBox error;
                error.setText(tr("Please enter a search range lower then 228"));
                error.exec();
                return;
            }
            break;
        }
        rng->advanceFrames(initialFrame - 1);
    }
    else if (ui->rngSelection->currentIndex() == 1)
    {
        switch (ui->comboBoxRNG64Bit->currentIndex())
        {
        case 0:
            rng64 = new BWRNG(seed);
            break;
        case 1:
            rng64 = new BWRNGR(seed);
            break;
        case 2:
            if (seed > 0xffffffff)
            {
                seed >>= 32;
            }
            rng64 = new SFMT(static_cast<u32>(seed));
            break;
        }
        rng64->advanceFrames(initialFrame - 1);
    }
    else
    {
        u32 status[4] = { ui->textBoxStatus0->getUInt(), ui->textBoxStatus1->getUInt(), ui->textBoxStatus2->getUInt(),
                          ui->textBoxStatus3->getUInt() };
        rng = new TinyMT(status);
        rng->advanceFrames(initialFrame - 1);
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

    QVector<bool> calcCustom(10);
    QVector<u64> customRValue(10);
    QVector<bool> pass(10);
    QVector<std::function<u64(u64, u64)>> calculators(10);

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
        if (calcCustom.at(i) && !pass.at(i))
        {
            QMessageBox error;
            error.setText(tr("You must check the Hex box in order to use Hex values."));
            error.exec();
            delete rng;
            delete rng64;
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

    QVector<ResearcherFrame> frames;
    for (u32 i = initialFrame; i < maxResults + initialFrame; i++)
    {
        ResearcherFrame frame(rng64Bit, i);
        if (rng64Bit)
        {
            frame.setFull64(rng64->next());
        }
        else
        {
            frame.setFull32(rng->next());
        }

        for (u8 j = 0; j < 10; j++)
        {
            if (calcCustom.at(j))
            {
                u64 temp = getCustom(textL.at(j), frame, frames);

                if (textR[j] != tr("None"))
                {
                    customRValue[j] = getCustom(textR.at(j), frame, frames);
                }

                frame.setCustom(j, calculators.at(j)(temp, customRValue.at(j)));
            }
        }
        frames.append(frame);
    }

    model->setHex(getHexCheck());
    model->addItems(frames);

    for (int i = 1; i < (rng64Bit ? 4 : 2); i++)
    {
        ui->tableView->horizontalHeader()->setSectionResizeMode(i, QHeaderView::ResizeToContents);
        int width = ui->tableView->horizontalHeader()->sectionSize(i);
        ui->tableView->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Interactive);
        ui->tableView->horizontalHeader()->resizeSection(i, width);
    }

    delete rng;
    delete rng64;
}

void Researcher::selectionIndexChanged(int index)
{
    if (index >= 0)
    {
        ui->textBoxSeed->setVisible(index != 2);
        ui->labelSeed->setVisible(index != 2);
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
