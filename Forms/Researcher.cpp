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

#include "Researcher.hpp"
#include "ui_Researcher.h"

Researcher::Researcher(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Researcher)
{
    ui->setupUi(this);

    keys["64Bit"] = 0;
    keys["32Bit"] = 1;
    keys["32Bit High"] = 2;
    keys["32Bit Low"] = 3;
    keys["16Bit High"] = 4;
    keys["16Bit Low"] = 5;
    keys["Custom1"] = 6;
    keys["Custom2"] = 7;
    keys["Custom3"] = 8;
    keys["Custom4"] = 9;
    keys["Custom5"] = 10;
    keys["Custom6"] = 11;
    keys["Custom7"] = 12;
    keys["Custom8"] = 13;
    keys["Custom9"] = 14;
    keys["Custom10"] = 15;
    keys["Previous1"] = 16;
    keys["Previous2"] = 17;
    keys["Previous3"] = 18;
    keys["Previous4"] = 19;
    keys["Previous5"] = 20;
    keys["Previous6"] = 21;
    keys["Previous7"] = 22;
    keys["Previous8"] = 23;
    keys["Previous9"] = 24;
    keys["Previous10"] = 25;

    ui->textBoxStartingFrame->SetValues("[^0-9]", 0xffffffffffffffff, 10);
    ui->textBoxMaxFrames->SetValues("[^0-9]", 0xffffffffffffffff, 10);
    ui->textBoxSeed->SetValues("[^0-9A-F]", 0xffffffffffffffff, 16);
    ui->textBoxSearch->SetValues("[^0-9A-F]", 0xffffffffffffffff, 16);

    ui->textBoxMult32Bit->SetValues("[^0-9A-F]", 0xffffffff, 16);
    ui->textBoxAdd32Bit->SetValues("[^0-9A-F]", 0xffffffff, 16);

    ui->textBoxMult64Bit->SetValues("[^0-9A-F]", 0xffffffffffffffff, 16);
    ui->textBoxAdd64Bit->SetValues("[^0-9A-F]", 0xffffffffffffffff, 16);

    ui->textBoxStatus3->SetValues("[^0-9A-F]", 0xffffffff, 16);
    ui->textBoxStatus2->SetValues("[^0-9A-F]", 0xffffffff, 16);
    ui->textBoxStatus1->SetValues("[^0-9A-F]", 0xffffffff, 16);
    ui->textBoxStatus0->SetValues("[^0-9A-F]", 0xffffffff, 16);

    ui->lineEditRValue1->SetValues("[^0-9A-F]", 0xffffffff, 16);
    ui->lineEditRValue2->SetValues("[^0-9A-F]", 0xffffffff, 16);
    ui->lineEditRValue3->SetValues("[^0-9A-F]", 0xffffffff, 16);
    ui->lineEditRValue4->SetValues("[^0-9A-F]", 0xffffffff, 16);
    ui->lineEditRValue5->SetValues("[^0-9A-F]", 0xffffffff, 16);
    ui->lineEditRValue6->SetValues("[^0-9A-F]", 0xffffffff, 16);
    ui->lineEditRValue7->SetValues("[^0-9A-F]", 0xffffffff, 16);
    ui->lineEditRValue8->SetValues("[^0-9A-F]", 0xffffffff, 16);
    ui->lineEditRValue9->SetValues("[^0-9A-F]", 0xffffffff, 16);
    ui->lineEditRValue10->SetValues("[^0-9A-F]", 0xffffffff, 16);

    ui->tableView->setModel(new ResearcherModel(this, false));
    ui->tableView->resizeColumnsToContents();

    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->verticalHeader()->setVisible(false);
}

Researcher::~Researcher()
{
    delete ui;
    if (model != NULL)
    {
        delete model;
        model = NULL;
    }
}

void Researcher::on_pushButtonGenerate32Bit_clicked()
{
    bool rng64Bit = ui->rngSelection->currentIndex() == 1;
    if (model != NULL)
        delete model;
    model = new ResearcherModel(this, rng64Bit);
    ui->tableView->setModel(model);

    u64 seed = ui->textBoxSeed->text().toULongLong(NULL, 16);
    u32 maxFrames = ui->textBoxMaxFrames->text().toUInt(NULL, 10);
    u32 startingFrame = ui->textBoxStartingFrame->text().toUInt(NULL, 10);

    IRNG *rng = NULL;
    IRNG64 *rng64 = NULL;

    if (ui->rngSelection->currentIndex() != 1 && (seed > 0xffffffff))
        seed >>= 32;

    if (ui->rngSelection->currentIndex() == 0)
    {
        if (ui->radioButtonCommon32Bit->isChecked())
        {
            switch (ui->comboBoxRNG32Bit->currentIndex())
            {
                case 0:
                    rng = new PokeRNG(seed);
                    break;
                case 1:
                    rng = new PokeRNGR(seed);
                    break;
                case 2:
                    rng = new XDRNG(seed);
                    break;
                case 3:
                    rng = new XDRNGR(seed);
                    break;
                case 4:
                    rng = new ARNG(seed);
                    break;
                case 5:
                    rng = new ARNGR(seed);
                    break;
                case 6:
                    rng = new MersenneTwister(seed);
                    break;
                case 7:
                    rng = new MersenneTwisterUntempered(seed);
                    break;
                case 8:
                    if (maxFrames > 227)
                    {
                        QMessageBox error;
                        error.setText(tr("Please enter a max frame lower then 228"));
                        error.exec();
                        return;
                    }
                    else
                        rng = new MersenneTwisterFast(seed, maxFrames);
                    break;
            }
        }
        else
        {
            u32 add = ui->textBoxAdd32Bit->text().toUInt(NULL, 16);
            u32 mult = ui->textBoxMult32Bit->text().toUInt(NULL, 16);
            rng = new LCRNG(add, mult, seed);
        }
    }
    else if (ui->rngSelection->currentIndex() == 1)
    {
        if (ui->radioButtonCommon64Bit->isChecked())
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
                        seed >>= 32;
                    rng64 = new SFMT(seed);
                    break;
            }
        }
        else
        {
            u64 add = ui->textBoxAdd64Bit->text().toUInt(NULL, 16);
            u64 mult = ui->textBoxMult64Bit->text().toUInt(NULL, 16);
            rng64 = new LCRNG64(add, mult, seed);
        }
    }
    else
    {
        u32 status[4] = { ui->textBoxStatus0->text().toUInt(NULL, 16), ui->textBoxStatus1->text().toUInt(NULL, 16),
                          ui->textBoxStatus2->text().toUInt(NULL, 16), ui->textBoxStatus3->text().toUInt(NULL, 16) };
        rng = new TinyMT(status);
    }

    Calculator calc;

    calc["/"] = &Researcher::Divide;
    calc["%"] = &Researcher::Modulo;
    calc[">>"] = &Researcher::ShiftRight;
    calc["<<"] = &Researcher::ShiftLeft;
    calc["&"] = &Researcher::And;
    calc["|"] = &Researcher::Or;
    calc["^"] = &Researcher::Xor;
    calc["+"] = &Researcher::Add;
    calc["-"] = &Researcher::Subtract;
    calc["*"] = &Researcher::Multiply;

    bool calcCustom[10];
    u64 customRValue[10];
    bool pass[10];
    u64(*Calculators[10])(u64, u64);

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

    customRValue[0] = ui->checkBoxHex1->isChecked()
                      ? ui->lineEditRValue1->text().toULongLong(&pass[0], 16)
                      : ui->lineEditRValue1->text().toULongLong(&pass[0], 10);
    customRValue[1] = ui->checkBoxHex2->isChecked()
                      ? ui->lineEditRValue2->text().toULongLong(&pass[1], 16)
                      : ui->lineEditRValue2->text().toULongLong(&pass[1], 10);
    customRValue[2] = ui->checkBoxHex3->isChecked()
                      ? ui->lineEditRValue3->text().toULongLong(&pass[2], 16)
                      : ui->lineEditRValue3->text().toULongLong(&pass[2], 10);
    customRValue[3] = ui->checkBoxHex4->isChecked()
                      ? ui->lineEditRValue4->text().toULongLong(&pass[3], 16)
                      : ui->lineEditRValue4->text().toULongLong(&pass[3], 10);
    customRValue[4] = ui->checkBoxHex5->isChecked()
                      ? ui->lineEditRValue5->text().toULongLong(&pass[4], 16)
                      : ui->lineEditRValue5->text().toULongLong(&pass[4], 10);
    customRValue[5] = ui->checkBoxHex6->isChecked()
                      ? ui->lineEditRValue6->text().toULongLong(&pass[5], 16)
                      : ui->lineEditRValue6->text().toULongLong(&pass[5], 10);
    customRValue[6] = ui->checkBoxHex7->isChecked()
                      ? ui->lineEditRValue7->text().toULongLong(&pass[6], 16)
                      : ui->lineEditRValue7->text().toULongLong(&pass[6], 10);
    customRValue[7] = ui->checkBoxHex8->isChecked()
                      ? ui->lineEditRValue8->text().toULongLong(&pass[7], 16)
                      : ui->lineEditRValue8->text().toULongLong(&pass[7], 10);
    customRValue[8] = ui->checkBoxHex9->isChecked()
                      ? ui->lineEditRValue9->text().toULongLong(&pass[8], 16)
                      : ui->lineEditRValue9->text().toULongLong(&pass[8], 10);
    customRValue[9] = ui->checkBoxHex10->isChecked()
                      ? ui->lineEditRValue10->text().toULongLong(&pass[9], 16)
                      : ui->lineEditRValue10->text().toULongLong(&pass[9], 10);

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

    Calculators[0] = calc[ui->comboBoxOperator1->currentText().toStdString()];
    Calculators[1] = calc[ui->comboBoxOperator2->currentText().toStdString()];
    Calculators[2] = calc[ui->comboBoxOperator3->currentText().toStdString()];
    Calculators[3] = calc[ui->comboBoxOperator4->currentText().toStdString()];
    Calculators[4] = calc[ui->comboBoxOperator5->currentText().toStdString()];
    Calculators[5] = calc[ui->comboBoxOperator6->currentText().toStdString()];
    Calculators[6] = calc[ui->comboBoxOperator7->currentText().toStdString()];
    Calculators[7] = calc[ui->comboBoxOperator8->currentText().toStdString()];
    Calculators[8] = calc[ui->comboBoxOperator9->currentText().toStdString()];
    Calculators[9] = calc[ui->comboBoxOperator10->currentText().toStdString()];

    vector<ResearcherFrame> frames;

    if (rng64Bit)
    {
        rng64->AdvanceFrames(startingFrame - 1);
    }
    else
    {
        rng->AdvanceFrames(startingFrame - 1);
    }

    string textL[10] = { ui->comboBoxLValue1->currentText().toStdString(), ui->comboBoxLValue2->currentText().toStdString(),
                          ui->comboBoxLValue3->currentText().toStdString(), ui->comboBoxLValue4->currentText().toStdString(),
                          ui->comboBoxLValue5->currentText().toStdString(), ui->comboBoxLValue6->currentText().toStdString(),
                          ui->comboBoxLValue7->currentText().toStdString(), ui->comboBoxLValue8->currentText().toStdString(),
                          ui->comboBoxLValue9->currentText().toStdString(), ui->comboBoxLValue10->currentText().toStdString() };

    string textR[10] = { "None", ui->comboBoxRValue2->currentText().toStdString(),
                          ui->comboBoxRValue3->currentText().toStdString(), ui->comboBoxRValue4->currentText().toStdString(),
                          ui->comboBoxRValue5->currentText().toStdString(), ui->comboBoxRValue6->currentText().toStdString(),
                          ui->comboBoxRValue7->currentText().toStdString(), ui->comboBoxRValue8->currentText().toStdString(),
                          ui->comboBoxRValue9->currentText().toStdString(), ui->comboBoxRValue10->currentText().toStdString() };

    for (u32 i = startingFrame; i < maxFrames + startingFrame; i++)
    {
        ResearcherFrame frame = ResearcherFrame(rng64Bit, i);
        if (rng64Bit)
        {
            frame.Full64 = rng64->Nextulong();
        }
        else
        {
            frame.Full32 = rng->Nextuint();
        }

        for (int j = 0; j < 10; j++)
        {
            if (calcCustom[j])
            {
                u64 temp = GetCustom(textL[j], frame, frames);

                if (textR[j] != "None")
                    customRValue[j] = GetCustom(textR[j], frame, frames);

                frame.Custom[j] = Calculators[j](temp, customRValue[j]);
            }
        }
        frames.push_back(frame);
    }

    model->SetModel(frames);
    ui->tableView->viewport()->update();
    ui->tableView->resizeColumnToContents(1);
    if (rng64Bit)
    {
        ui->tableView->resizeColumnToContents(2);
        ui->tableView->resizeColumnToContents(3);
    }

    delete rng;
    delete rng64;
    rng = NULL;
    rng64 = NULL;
}

void Researcher::on_pushButtonSearch_clicked()
{
    if (model->rowCount() == 0)
        return;

    QString string = ui->comboBoxSearch->currentText();
    u64 result = ui->textBoxSearch->text().toULongLong(NULL, 16);

    QModelIndex end = model->Search(string, result, 0);
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

void Researcher::on_pushButtonNext_clicked()
{
    if (model->rowCount() == 0)
        return;

    QString string = ui->comboBoxSearch->currentText();
    QModelIndex start = ui->tableView->currentIndex();
    u64 result = ui->textBoxSearch->text().toULongLong(NULL, 16);

    if (!start.isValid())
        return;

    QModelIndex end = model->Search(string, result, start.row() + 1);
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

u64 Researcher::GetCustom(string text, ResearcherFrame frame, vector<ResearcherFrame> frames)
{
    switch (keys[text])
    {
        case 0:
            return frame.Full64;
        case 1:
            return frame.Full32;
        case 2:
            return frame.High32();
        case 3:
            return frame.Low32();
        case 4:
            return frame.High16();
        case 5:
            return frame.Low16();
        case 6:
            return frame.Custom[0];
        case 7:
            return frame.Custom[1];
        case 8:
            return frame.Custom[2];
        case 9:
            return frame.Custom[3];
        case 10:
            return frame.Custom[4];
        case 11:
            return frame.Custom[5];
        case 12:
            return frame.Custom[6];
        case 13:
            return frame.Custom[7];
        case 14:
            return frame.Custom[8];
        case 15:
            return frame.Custom[9];
        case 16:
            return frames.size() == 0 ? 0 : frames[frames.size() - 1].Custom[0];
        case 17:
            return frames.size() == 0 ? 0 : frames[frames.size() - 1].Custom[1];
        case 18:
            return frames.size() == 0 ? 0 : frames[frames.size() - 1].Custom[2];
        case 19:
            return frames.size() == 0 ? 0 : frames[frames.size() - 1].Custom[3];
        case 20:
            return frames.size() == 0 ? 0 : frames[frames.size() - 1].Custom[4];
        case 21:
            return frames.size() == 0 ? 0 : frames[frames.size() - 1].Custom[5];
        case 22:
            return frames.size() == 0 ? 0 : frames[frames.size() - 1].Custom[6];
        case 23:
            return frames.size() == 0 ? 0 : frames[frames.size() - 1].Custom[7];
        case 24:
            return frames.size() == 0 ? 0 : frames[frames.size() - 1].Custom[8];
        case 25:
            return frames.size() == 0 ? 0 : frames[frames.size() - 1].Custom[9];
        default:
            return 0;
    }
}

void Researcher::on_rngSelection_currentChanged(int index)
{
    ui->textBoxSeed->setVisible(index != 2);
    ui->label_14->setVisible(index != 2);
    ui->comboBoxSearch->clear();
    QStringList items = index != 1 ? QStringList() << "32Bit" << "16Bit High" << "16Bit Low" :
                                     QStringList() << "64Bit" << "32Bit High" << "32Bit Low" << "16Bit High" << "16Bit Low" ;
    ui->comboBoxSearch->addItems(items);
}
