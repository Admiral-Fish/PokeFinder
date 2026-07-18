/*
 * This file is part of PokeFinder
 * Copyright (C) 2017-2024 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "AdvanceFinder.hpp"
#include "ui_AdvanceFinder.h"
#include <Core/Util/AdvanceFinderLogic.hpp>
#include <Core/Util/Translator.hpp>
#include <Model/Gen4/IRNGProvider4.hpp>
#include <Model/Gen5/IRNGProvider5.hpp>
#include <Model/IndexFilterProxyModel.hpp>
#include <array>

AdvanceFinder::AdvanceFinder(QAbstractItemModel *generatorModel, QTableView *sourceTableView, QWidget *parent) :
    QDialog(parent), ui(new Ui::AdvanceFinder), sourceTableView(sourceTableView)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    model = new IndexFilterProxyModel(ui->tableView, generatorModel);
    ui->tableView->setModel(model);

    callChatot = dynamic_cast<IRNGProvider4 *>(generatorModel);
    chatotNeedle = dynamic_cast<IRNGProvider5 *>(generatorModel);

    if (callChatot == nullptr)
    {
        ui->radioButtonCalls->hide();
        ui->groupBoxCalls->hide();
        ui->radioButtonChatot->setChecked(true);
    }
    else if (chatotNeedle == nullptr)
    {
        ui->radioButtonNeedle->hide();
        ui->groupBoxNeedle->hide();
    }

    onModeChanged();

    using ChatotToken = AdvanceFinderLogic::ChatotToken;
    const std::array<std::pair<QPushButton *, ChatotToken>, 10> chatotButtons = { { { ui->pushButtonHigh, ChatotToken::High },
                                                                                    { ui->pushButtonMidHigh, ChatotToken::MidHigh },
                                                                                    { ui->pushButtonMid, ChatotToken::Mid },
                                                                                    { ui->pushButtonMidLow, ChatotToken::MidLow },
                                                                                    { ui->pushButtonLow, ChatotToken::Low },
                                                                                    { ui->pushButtonHighMidHigh, ChatotToken::HighMidHigh },
                                                                                    { ui->pushButtonMidHighMid, ChatotToken::MidHighMid },
                                                                                    { ui->pushButtonMidMidLow, ChatotToken::MidMidLow },
                                                                                    { ui->pushButtonMidLowLow, ChatotToken::MidLowLow },
                                                                                    { ui->pushButtonChatotAny, ChatotToken::Any } } };
    for (const auto &[button, token] : chatotButtons)
    {
        connect(button, &QPushButton::clicked, this, [=] { appendToken(button, static_cast<u8>(token)); });
    }

    using NeedleToken = AdvanceFinderLogic::NeedleToken;
    const std::array<std::pair<QPushButton *, NeedleToken>, 9> needleButtons = { { { ui->pushButtonNeedle0, NeedleToken::Needle0 },
                                                                                   { ui->pushButtonNeedle1, NeedleToken::Needle1 },
                                                                                   { ui->pushButtonNeedle2, NeedleToken::Needle2 },
                                                                                   { ui->pushButtonNeedle3, NeedleToken::Needle3 },
                                                                                   { ui->pushButtonNeedle4, NeedleToken::Needle4 },
                                                                                   { ui->pushButtonNeedle5, NeedleToken::Needle5 },
                                                                                   { ui->pushButtonNeedle6, NeedleToken::Needle6 },
                                                                                   { ui->pushButtonNeedle7, NeedleToken::Needle7 },
                                                                                   { ui->pushButtonNeedleAny, NeedleToken::Any } } };
    for (const auto &[button, token] : needleButtons)
    {
        connect(button, &QPushButton::clicked, this, [=] { appendToken(button, static_cast<u8>(token)); });
    }

    using CallToken = AdvanceFinderLogic::CallToken;
    connect(ui->pushButtonCallE, &QPushButton::clicked, this, [=] { appendToken(ui->pushButtonCallE, static_cast<u8>(CallToken::E)); });
    connect(ui->pushButtonCallK, &QPushButton::clicked, this, [=] { appendToken(ui->pushButtonCallK, static_cast<u8>(CallToken::K)); });
    connect(ui->pushButtonCallP, &QPushButton::clicked, this, [=] { appendToken(ui->pushButtonCallP, static_cast<u8>(CallToken::P)); });
    connect(ui->radioButtonElm, &QRadioButton::clicked, this, &AdvanceFinder::elm);
    connect(ui->radioButtonIrwin, &QRadioButton::clicked, this, &AdvanceFinder::irwin);
    irwin();

    connect(ui->buttonGroupType, &QButtonGroup::buttonClicked, this, &AdvanceFinder::onModeChanged);
    connect(ui->lineEditSequence, &QLineEdit::textChanged, this, &AdvanceFinder::sequenceTextChanged);
    connect(ui->pushButtonJump, &QPushButton::clicked, this, &AdvanceFinder::jumpToAdvance);
    connect(ui->pushButtonRemove, &QPushButton::clicked, this, &AdvanceFinder::removeToken);
    connect(ui->pushButtonClear, &QPushButton::clicked, this, &AdvanceFinder::clearTokens);

    search();
}

AdvanceFinder::~AdvanceFinder()
{
    delete ui;
}

void AdvanceFinder::appendToken(const QPushButton *button, u8 token)
{
    tokens.emplace_back(token);

    QString text = ui->lineEditSequence->text();
    text += text.isEmpty() ? button->text() : ", " + button->text();
    ui->lineEditSequence->setText(text);
}

void AdvanceFinder::removeToken()
{
    if (!tokens.empty())
    {
        tokens.pop_back();
    }

    QStringList text = ui->lineEditSequence->text().split(',', Qt::SkipEmptyParts);
    if (!text.isEmpty())
    {
        text.removeLast();
    }
    ui->lineEditSequence->setText(text.join(", "));
}

void AdvanceFinder::clearTokens()
{
    tokens.clear();
    ui->lineEditSequence->clear();
}

void AdvanceFinder::elm()
{
    ui->labelKResponse->setText(tr("K - I expect there are some Pokémon in the Kanto region that I don't know. There are probably methods "
                                   "of evolution that I'm not familiar with yet. I should use that perspective and discover what I can!"));
    ui->labelEResponse->setText(tr("E - There are so many different ways that Pokémon evolve, aren't there?! Some "
                                   "Pokémon don't even evolve until they meet certain conditions first!"));
    ui->labelPResponse->setText(tr("P - It seems that Pokémon that have been infected with Pokérus level up "
                                   "better. We're not quite sure why..."));
}

void AdvanceFinder::irwin()
{
    ui->labelKResponse->setText(tr("K - I'm so glad you called! I was just about to call you, too! I guess we must be a good match!"));
    ui->labelEResponse->setText(tr("E - Hearing about your escapades rocks my soul! It sure does!"));
    ui->labelPResponse->setText(tr("P - How are you? What are you doing? Where are you? How many Badges do you have now? How much money "
                                   "have you saved? How's your mom? Have you got lots of Pokémon? Is it going to be sunny tomorrow? Arrgh, "
                                   "there's so much I want to chat about! This is going nowhere!"));
}

void AdvanceFinder::sequenceTextChanged(const QString &text)
{
    search();
}

void AdvanceFinder::search()
{
    AdvanceFinderLogic::Sequence sequence;
    std::function<u8(int)> getter;
    if (ui->radioButtonCalls->isChecked())
    {
        sequence = AdvanceFinderLogic::getCallSequence(tokens);

        getter = [this](int row) { return callChatot->getCall(row); };
    }
    else if (ui->radioButtonChatot->isChecked())
    {
        sequence = AdvanceFinderLogic::getChatotSequence(tokens);

        if (callChatot)
        {
            getter = [this](int row) { return callChatot->getChatot(row); };
        }
        else
        {
            getter = [this](int row) { return chatotNeedle->getChatot(row); };
        }
    }
    else
    {
        sequence = AdvanceFinderLogic::getNeedleSequence(tokens);

        getter = [this](int row) { return chatotNeedle->getNeedle(row); };
    }

    if (sequence.empty())
    {
        model->setFilteredIndexes(QModelIndexList());
        ui->labelPossibleResults->setText(tr("Possible Results: %1").arg(model->sourceModel()->rowCount()));
        return;
    }

    std::vector<size_t> matches = AdvanceFinderLogic::findMatches(model->sourceModel()->rowCount(), sequence, getter);

    model->setFilteredIndexes(matches);
    ui->labelPossibleResults->setText(tr("Possible Results: %1").arg(matches.size()));
}

void AdvanceFinder::jumpToAdvance()
{
    int row = ui->tableView->currentIndex().row();
    if (row < 0)
    {
        return;
    }

    QModelIndex proxyIndex = model->index(row, 0);
    QModelIndex sourceIndex = model->mapToSource(proxyIndex);

    sourceTableView->setCurrentIndex(sourceIndex);
    sourceTableView->selectRow(sourceIndex.row());
    sourceTableView->scrollTo(sourceIndex, QAbstractItemView::PositionAtCenter);
    sourceTableView->setFocus();
}

void AdvanceFinder::onModeChanged()
{
    bool calls = ui->radioButtonCalls->isChecked();
    bool chatot = ui->radioButtonChatot->isChecked();
    bool needle = ui->radioButtonNeedle->isChecked();

    ui->groupBoxCalls->setVisible(calls);
    ui->groupBoxChatot->setVisible(chatot);
    ui->groupBoxNeedle->setVisible(needle);

    tokens.clear();
    ui->lineEditSequence->clear();
    search();
}
