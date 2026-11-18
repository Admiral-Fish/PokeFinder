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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "SearchChatot.hpp"
#include <Core/Gen4/HGSSRoamer.hpp>
#include <Core/Gen4/SeedTime4.hpp>
#include <Core/RNG/LCRNG.hpp>
#include <Core/Util/AdvanceSearcher.hpp>
#include <Model/Gen4/SeedToTimeModel4.hpp>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <array>
#include <tuple>
#include <utility>

static QString getChatotTokenText(AdvanceSearcher::ChatotToken token)
{
    using ChatotToken = AdvanceSearcher::ChatotToken;
    switch (token)
    {
    case ChatotToken::High:
        return QStringLiteral("H");
    case ChatotToken::MidHigh:
        return QStringLiteral("MH");
    case ChatotToken::Mid:
        return QStringLiteral("M");
    case ChatotToken::MidLow:
        return QStringLiteral("ML");
    case ChatotToken::Low:
        return QStringLiteral("L");
    case ChatotToken::HighMidHigh:
        return QStringLiteral("H/MH");
    case ChatotToken::MidHighMid:
        return QStringLiteral("MH/M");
    case ChatotToken::MidMidLow:
        return QStringLiteral("M/ML");
    case ChatotToken::MidLowLow:
        return QStringLiteral("ML/L");
    case ChatotToken::HighMidHighMid:
        return QStringLiteral("H/MH/M");
    case ChatotToken::MidMidLowLow:
        return QStringLiteral("M/ML/L");
    case ChatotToken::HigherThanLast:
        return QStringLiteral("H+");
    case ChatotToken::LowerThanLast:
        return QStringLiteral("L-");
    case ChatotToken::Any:
        return QStringLiteral("A");
    }
    return { };
}

SearchChatot::SearchChatot(SeedToTimeCalibrateModel4 *model, ResultsCallback callback, QWidget *parent) :
    QWidget(parent), model(model), callback(std::move(callback))
{
    auto *layout = new QGridLayout(this);

    auto *rangeLayout = new QHBoxLayout();
    rangeLayout->addWidget(new QLabel(tr("Starting Advance"), this));
    startingAdvance = new QSpinBox(this);
    startingAdvance->setRange(0, 999999999);
    rangeLayout->addWidget(startingAdvance);
    rangeLayout->addWidget(new QLabel(tr("Max Advance"), this));
    maxAdvance = new QSpinBox(this);
    maxAdvance->setRange(0, 999999999);
    maxAdvance->setValue(30);
    rangeLayout->addWidget(maxAdvance);
    rangeLayout->addStretch();
    layout->addLayout(rangeLayout, 6, 0, 1, 5);

    using ChatotToken = AdvanceSearcher::ChatotToken;
    const std::array<std::tuple<QString, QString, ChatotToken, int, int, int>, 14> buttons
        = { { { tr("High"), QStringLiteral("H"), ChatotToken::High, 1, 0, 1 },
              { tr("Mid-High"), QStringLiteral("MH"), ChatotToken::MidHigh, 1, 1, 1 },
              { tr("Mid"), QStringLiteral("M"), ChatotToken::Mid, 1, 2, 1 },
              { tr("Mid-Low"), QStringLiteral("ML"), ChatotToken::MidLow, 1, 3, 1 },
              { tr("Low"), QStringLiteral("L"), ChatotToken::Low, 1, 4, 1 },
              { tr("High / Mid-High"), QStringLiteral("H/MH"), ChatotToken::HighMidHigh, 2, 0, 1 },
              { tr("Mid-High / Mid"), QStringLiteral("MH/M"), ChatotToken::MidHighMid, 2, 1, 1 },
              { tr("Mid / Mid-Low"), QStringLiteral("M/ML"), ChatotToken::MidMidLow, 2, 2, 1 },
              { tr("Mid-Low / Low"), QStringLiteral("ML/L"), ChatotToken::MidLowLow, 2, 3, 1 },
              { tr("High / Mid-High / Mid"), QStringLiteral("H/MH/M"), ChatotToken::HighMidHighMid, 3, 0, 2 },
              { tr("Mid / Mid-Low / Low"), QStringLiteral("M/ML/L"), ChatotToken::MidMidLowLow, 3, 2, 2 },
              { tr("Higher than last"), QStringLiteral("H+"), ChatotToken::HigherThanLast, 4, 0, 2 },
              { tr("Lower than last"), QStringLiteral("L-"), ChatotToken::LowerThanLast, 4, 2, 2 },
              { tr("Any"), QStringLiteral("A"), ChatotToken::Any, 2, 4, 1 } } };

    for (const auto &[label, tokenText, token, row, column, columnSpan] : buttons)
    {
        auto *button = new QPushButton(label, this);
        if (token == ChatotToken::Any)
        {
            button->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
            layout->addWidget(button, row, column, 3, columnSpan);
        }
        else
        {
            layout->addWidget(button, row, column, 1, columnSpan);
        }
        connect(button, &QPushButton::clicked, this, [this, tokenText, token] { appendToken(tokenText, static_cast<u8>(token)); });
    }

    lineEdit = new QLineEdit(this);
    auto *inputLayout = new QHBoxLayout();
    inputLayout->addWidget(lineEdit, 1);
    auto *removeButton = new QPushButton(tr("Remove"), this);
    auto *clearButton = new QPushButton(tr("Clear"), this);
    inputLayout->addWidget(removeButton);
    inputLayout->addWidget(clearButton);
    layout->addLayout(inputLayout, 5, 0, 1, 5);

    connect(lineEdit, &QLineEdit::textChanged, this, [this](const QString &text) {
        sequenceValid = parseTokens(text);
        search();
    });
    connect(removeButton, &QPushButton::clicked, this, &SearchChatot::remove);
    connect(clearButton, &QPushButton::clicked, this, &SearchChatot::clear);
    connect(startingAdvance, &QSpinBox::valueChanged, this, [this] { search(); });
    connect(maxAdvance, &QSpinBox::valueChanged, this, [this] { search(); });
}

void SearchChatot::appendToken(const QString &text, u8 token)
{
    tokens.emplace_back(token);
    QString sequence = lineEdit->text();
    sequence += sequence.isEmpty() ? text : QStringLiteral(", ") + text;
    lineEdit->setText(sequence);
}

void SearchChatot::clear()
{
    lineEdit->clear();
}

bool SearchChatot::parseTokens(const QString &text)
{
    tokens.clear();
    if (text.trimmed().isEmpty())
    {
        return true;
    }

    using ChatotToken = AdvanceSearcher::ChatotToken;
    constexpr std::array<ChatotToken, 14> chatotTokens
        = { ChatotToken::High,           ChatotToken::MidHigh,      ChatotToken::Mid,
            ChatotToken::MidLow,         ChatotToken::Low,          ChatotToken::HighMidHigh,
            ChatotToken::MidHighMid,     ChatotToken::MidMidLow,    ChatotToken::MidLowLow,
            ChatotToken::HighMidHighMid, ChatotToken::MidMidLowLow, ChatotToken::HigherThanLast,
            ChatotToken::LowerThanLast,  ChatotToken::Any };

    const QStringList inputTokens = text.split(',', Qt::KeepEmptyParts);
    for (const QString &input : inputTokens)
    {
        const QString value = input.trimmed();
        bool found = false;
        for (ChatotToken token : chatotTokens)
        {
            if (!value.isEmpty() && value.compare(getChatotTokenText(token), Qt::CaseInsensitive) == 0)
            {
                tokens.emplace_back(static_cast<u8>(token));
                found = true;
                break;
            }
        }
        if (!found)
        {
            tokens.clear();
            return false;
        }
    }
    return true;
}

void SearchChatot::remove()
{
    QStringList sequence = lineEdit->text().split(',', Qt::SkipEmptyParts);
    if (!sequence.isEmpty())
    {
        sequence.removeLast();
    }
    for (QString &token : sequence)
    {
        token = token.trimmed();
    }
    lineEdit->setText(sequence.join(QStringLiteral(", ")));
}

void SearchChatot::search()
{
    QModelIndexList indexes;
    if (!sequenceValid)
    {
        callback(indexes, 0);
        return;
    }
    if (tokens.empty())
    {
        callback(indexes, model->rowCount());
        return;
    }

    const u32 firstAdvance = static_cast<u32>(startingAdvance->value());
    const u32 lastAdvance = static_cast<u32>(maxAdvance->value());
    if (firstAdvance > lastAdvance)
    {
        callback(indexes, 0);
        return;
    }

    using ChatotToken = AdvanceSearcher::ChatotToken;
    const ChatotToken firstToken = static_cast<ChatotToken>(tokens.front());
    const bool needsPrevious = firstToken == ChatotToken::HigherThanLast || firstToken == ChatotToken::LowerThanLast;

    for (int row = 0; row < model->rowCount(); row++)
    {
        const SeedTimeCalibrate4 &state = model->getItem(row);
        const u32 skips = state.getRoamer() ? state.getRoamer()->getSkips() : 0;
        const u32 firstSearchAdvance = firstAdvance > skips ? firstAdvance : skips;
        const u32 matchStartAdvance = needsPrevious && firstSearchAdvance == skips ? firstSearchAdvance + 1 : firstSearchAdvance;
        if (matchStartAdvance > lastAdvance)
        {
            continue;
        }

        const u32 rowDataStart = needsPrevious ? matchStartAdvance - 1 : matchStartAdvance;
        const int rowPitchCount = static_cast<int>(lastAdvance - rowDataStart + 1);
        PokeRNG rng(state.getSeed(), rowDataStart);
        std::vector<u8> pitches;
        pitches.reserve(rowPitchCount);
        for (int i = 0; i < rowPitchCount; i++)
        {
            pitches.emplace_back(static_cast<u8>(((rng.nextUShort() % 8192) * 100) >> 13));
        }

        const auto matches = AdvanceSearcher::findChatotMatches(rowPitchCount, tokens, [&pitches](int index) { return pitches[index]; });
        bool pass = false;
        for (int endingIndex : matches)
        {
            const u32 matchStart = rowDataStart + endingIndex + 1 - static_cast<u32>(tokens.size());
            if (matchStart >= matchStartAdvance)
            {
                pass = true;
                break;
            }
        }
        if (pass)
        {
            indexes.append(model->index(row, 0));
        }
    }

    callback(indexes, indexes.size());
}
