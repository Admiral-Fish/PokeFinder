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
#include <Core/Util/Translator.hpp>
#include <QAbstractItemView>
#include <QAbstractItemModel>
#include <QHeaderView>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QStringList>
#include <QTableView>
#include <QTableWidgetItem>
#include <QVariant>
#include <array>
#include <utility>

static int findColumn(QAbstractItemModel *model, const QString &header)
{
    for (int column = 0; column < model->columnCount(); column++)
    {
        if (model->headerData(column, Qt::Horizontal, Qt::DisplayRole).toString() == header)
        {
            return column;
        }
    }
    return -1;
}

static u8 readChatot(QAbstractItemModel *model, int row, int column)
{
    QString text = model->data(model->index(row, column), Qt::DisplayRole).toString();
    int end = text.size() - 1;
    while (end >= 0 && !text[end].isDigit())
    {
        end--;
    }

    int start = end;
    while (start >= 0 && text[start].isDigit())
    {
        start--;
    }

    return static_cast<u8>(text.mid(start + 1, end - start).toUInt());
}

static u8 readNeedle(QAbstractItemModel *model, int row, int column)
{
    QString text = model->data(model->index(row, column), Qt::DisplayRole).toString();
    for (u8 i = 0; i < 8; i++)
    {
        if (text == QString::fromStdString(Translator::getSaveNeedle(i)))
        {
            return i;
        }
    }
    return 0;
}

AdvanceFinder::AdvanceFinder(QAbstractItemModel *generatorModel, QWidget *parent) : AdvanceFinder(generatorModel, nullptr, parent)
{
}

AdvanceFinder::AdvanceFinder(QAbstractItemModel *generatorModel, QTableView *sourceTableView, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdvanceFinder),
    generatorModel(generatorModel),
    sourceTableView(sourceTableView),
    chatotColumn(findColumn(generatorModel, tr("Chatot"))),
    needleColumn(findColumn(generatorModel, tr("Needle"))),
    switchingMode(false)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    bool hasNeedles = needleColumn != -1;

    ui->widgetMode->setVisible(hasNeedles);
    ui->groupBoxNeedle->setVisible(false);

    using ChatotToken = AdvanceFinderLogic::ChatotToken;
    const std::array<std::pair<QPushButton *, ChatotToken>, 10> chatotButtons
        = { { { ui->pushButtonHigh, ChatotToken::High },
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
        connect(button, &QPushButton::clicked, this, [=] { appendChatotToken(button->text(), token); });
    }

    using NeedleToken = AdvanceFinderLogic::NeedleToken;
    const std::array<std::pair<QPushButton *, NeedleToken>, 9> needleButtons
        = { { { ui->pushButtonNeedle0, NeedleToken::Needle0 },
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
        connect(button, &QPushButton::clicked, this, [=] { appendNeedleToken(button->text(), token); });
    }

    ui->tableWidgetResults->setColumnCount(generatorModel->columnCount());
    QStringList headers;
    for (int i = 0; i < generatorModel->columnCount(); i++)
    {
        headers << generatorModel->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString();
    }
    ui->tableWidgetResults->setHorizontalHeaderLabels(headers);
    ui->tableWidgetResults->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);

    connect(ui->radioButtonChatot, &QRadioButton::toggled, this, &AdvanceFinder::onModeChanged);
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

void AdvanceFinder::appendChatotToken(const QString &text, AdvanceFinderLogic::ChatotToken token)
{
    chatotTokens.emplace_back(token);
    appendTokenText(text);
}

void AdvanceFinder::appendNeedleToken(const QString &text, AdvanceFinderLogic::NeedleToken token)
{
    needleTokens.emplace_back(token);
    appendTokenText(text);
}

void AdvanceFinder::appendTokenText(const QString &token)
{
    QString text = ui->lineEditSequence->text().trimmed();
    text += text.isEmpty() ? token : ", " + token;
    ui->lineEditSequence->setText(text);
}

void AdvanceFinder::removeToken()
{
    QStringList tokens = ui->lineEditSequence->text().split(',', Qt::SkipEmptyParts);
    if (!tokens.isEmpty())
    {
        tokens.removeLast();
        if (ui->radioButtonChatot->isChecked())
        {
            if (!chatotTokens.empty())
            {
                chatotTokens.pop_back();
            }
        }
        else
        {
            if (!needleTokens.empty())
            {
                needleTokens.pop_back();
            }
        }
    }

    for (QString &token : tokens)
    {
        token = token.trimmed();
    }
    ui->lineEditSequence->setText(tokens.join(", "));
}

void AdvanceFinder::clearTokens()
{
    if (ui->radioButtonChatot->isChecked())
    {
        chatotTokens.clear();
    }
    else
    {
        needleTokens.clear();
    }
    ui->lineEditSequence->clear();
}

void AdvanceFinder::sequenceTextChanged(const QString &text)
{
    if (!switchingMode)
    {
        if (ui->radioButtonChatot->isChecked())
        {
            chatotSequence = text;
        }
        else
        {
            needleSequence = text;
        }
    }

    search();
}

void AdvanceFinder::search()
{
    AdvanceFinderLogic::Sequence sequence = ui->radioButtonChatot->isChecked()
        ? AdvanceFinderLogic::getChatotSequence(chatotTokens)
        : AdvanceFinderLogic::getNeedleSequence(needleTokens);

    ui->tableWidgetResults->setRowCount(0);
    previewRows.clear();
    ui->pushButtonJump->setEnabled(false);

    if (sequence.empty())
    {
        ui->labelPossibleResults->setText(tr("Possible Results: %1").arg(generatorModel->rowCount()));
        return;
    }

    auto getter = [this](size_t row) {
        return ui->radioButtonChatot->isChecked() ? readChatot(generatorModel, static_cast<int>(row), chatotColumn)
                                                  : readNeedle(generatorModel, static_cast<int>(row), needleColumn);
    };
    std::vector<size_t> matches = AdvanceFinderLogic::findMatches(static_cast<size_t>(generatorModel->rowCount()), sequence, getter);

    ui->labelPossibleResults->setText(tr("Possible Results: %1").arg(matches.size()));
    displayResults(matches, sequence.size());
}

void AdvanceFinder::displayResults(const std::vector<size_t> &matches, size_t sequenceSize)
{
    ui->tableWidgetResults->setRowCount(0);
    previewRows.clear();
    ui->pushButtonJump->setEnabled(false);
    if (matches.size() > 10)
    {
        return;
    }

    for (size_t i = 0; i < matches.size(); i++)
    {
        int resultRow = static_cast<int>(i);
        int sourceRow = static_cast<int>(matches[i] + sequenceSize - 1);
        previewRows.emplace_back(sourceRow);
        ui->tableWidgetResults->insertRow(resultRow);

        for (int column = 0; column < generatorModel->columnCount(); column++)
        {
            QVariant value = generatorModel->data(generatorModel->index(sourceRow, column), Qt::DisplayRole);
            ui->tableWidgetResults->setItem(resultRow, column, new QTableWidgetItem(value.toString()));
        }
    }
    ui->pushButtonJump->setEnabled(sourceTableView != nullptr && !previewRows.empty());
    ui->tableWidgetResults->resizeColumnsToContents();
}

void AdvanceFinder::jumpToAdvance()
{
    if (!sourceTableView || previewRows.empty())
    {
        return;
    }

    int resultRow = ui->tableWidgetResults->currentRow();
    if (resultRow < 0 || resultRow >= static_cast<int>(previewRows.size()))
    {
        resultRow = 0;
    }

    int sourceRow = previewRows[resultRow];
    QModelIndex index = generatorModel->index(sourceRow, 0);
    sourceTableView->setCurrentIndex(index);
    sourceTableView->selectRow(sourceRow);
    sourceTableView->scrollTo(index, QAbstractItemView::PositionAtCenter);
    sourceTableView->setFocus();
}

void AdvanceFinder::onModeChanged()
{
    bool chatot = ui->radioButtonChatot->isChecked();
    ui->groupBoxChatot->setVisible(chatot);
    ui->groupBoxNeedle->setVisible(!chatot);

    switchingMode = true;
    ui->lineEditSequence->setText(chatot ? chatotSequence : needleSequence);
    switchingMode = false;
    search();
}
