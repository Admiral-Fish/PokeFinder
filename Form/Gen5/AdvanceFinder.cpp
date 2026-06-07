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
#include <Model/Gen4/EggModel4.hpp>
#include <Model/Gen4/EventModel4.hpp>
#include <Model/Gen4/StaticModel4.hpp>
#include <Model/Gen4/WildModel4.hpp>
#include <Model/Gen5/EggModel5.hpp>
#include <Model/Gen5/EventModel5.hpp>
#include <Model/Gen5/HiddenGrottoModel.hpp>
#include <Model/Gen5/StaticModel5.hpp>
#include <Model/Gen5/WildModel5.hpp>
#include <QAbstractItemView>
#include <QAbstractItemModel>
#include <QGridLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QStringList>
#include <QTableView>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>
#include <QVariant>
#include <tuple>
#include <utility>

static QVector<u8> chatotRange(u8 min, u8 max)
{
    QVector<u8> values;
    values.reserve(max - min + 1);
    for (u8 value = min; value <= max; value++)
    {
        values.append(value);
    }
    return values;
}

static QString needleSymbol(u8 needle)
{
    static const char *needles[] = { "\342\206\221", "\342\206\227", "\342\206\222", "\342\206\230",
                                     "\342\206\223", "\342\206\231", "\342\206\220", "\342\206\226" };
    return QString::fromUtf8(needles[needle & 7]);
}

AdvanceFinder::AdvanceFinder(WildGeneratorModel5 *generatorModel, QWidget *parent) :
    AdvanceFinder(generatorModel, nullptr, parent)
{
}

AdvanceFinder::AdvanceFinder(WildGeneratorModel5 *generatorModel, QTableView *sourceTableView, QWidget *parent) :
    AdvanceFinder(generatorModel, [generatorModel] { return generatorModel->getModel().size(); },
                  [generatorModel](size_t row) { return generatorModel->getModel()[row].getChatot(); },
                  [generatorModel](size_t row) { return generatorModel->getModel()[row].getSaveNeedle(); }, sourceTableView, parent)
{
}

AdvanceFinder::AdvanceFinder(WildGeneratorModel4 *generatorModel, QWidget *parent) :
    AdvanceFinder(generatorModel, nullptr, parent)
{
}

AdvanceFinder::AdvanceFinder(WildGeneratorModel4 *generatorModel, QTableView *sourceTableView, QWidget *parent) :
    AdvanceFinder(generatorModel, [generatorModel] { return generatorModel->getModel().size(); },
                  [generatorModel](size_t row) { return generatorModel->getModel()[row].getChatot(); }, ValueGetter(), sourceTableView,
                  parent)
{
}

AdvanceFinder::AdvanceFinder(StaticGeneratorModel5 *generatorModel, QWidget *parent) :
    AdvanceFinder(generatorModel, nullptr, parent)
{
}

AdvanceFinder::AdvanceFinder(StaticGeneratorModel5 *generatorModel, QTableView *sourceTableView, QWidget *parent) :
    AdvanceFinder(generatorModel, [generatorModel] { return generatorModel->getModel().size(); },
                  [generatorModel](size_t row) { return generatorModel->getModel()[row].getChatot(); },
                  [generatorModel](size_t row) { return generatorModel->getModel()[row].getSaveNeedle(); }, sourceTableView, parent)
{
}

AdvanceFinder::AdvanceFinder(StaticGeneratorModel4 *generatorModel, QWidget *parent) :
    AdvanceFinder(generatorModel, nullptr, parent)
{
}

AdvanceFinder::AdvanceFinder(StaticGeneratorModel4 *generatorModel, QTableView *sourceTableView, QWidget *parent) :
    AdvanceFinder(generatorModel, [generatorModel] { return generatorModel->getModel().size(); },
                  [generatorModel](size_t row) { return generatorModel->getModel()[row].getChatot(); }, ValueGetter(), sourceTableView,
                  parent)
{
}

AdvanceFinder::AdvanceFinder(EventGeneratorModel5 *generatorModel, QWidget *parent) :
    AdvanceFinder(generatorModel, nullptr, parent)
{
}

AdvanceFinder::AdvanceFinder(EventGeneratorModel5 *generatorModel, QTableView *sourceTableView, QWidget *parent) :
    AdvanceFinder(generatorModel, [generatorModel] { return generatorModel->getModel().size(); },
                  [generatorModel](size_t row) { return generatorModel->getModel()[row].getChatot(); },
                  [generatorModel](size_t row) { return generatorModel->getModel()[row].getSaveNeedle(); }, sourceTableView, parent)
{
}

AdvanceFinder::AdvanceFinder(EventGeneratorModel4 *generatorModel, QWidget *parent) :
    AdvanceFinder(generatorModel, nullptr, parent)
{
}

AdvanceFinder::AdvanceFinder(EventGeneratorModel4 *generatorModel, QTableView *sourceTableView, QWidget *parent) :
    AdvanceFinder(generatorModel, [generatorModel] { return generatorModel->getModel().size(); },
                  [generatorModel](size_t row) { return generatorModel->getModel()[row].getChatot(); }, ValueGetter(), sourceTableView,
                  parent)
{
}

AdvanceFinder::AdvanceFinder(EggGeneratorModel5 *generatorModel, QWidget *parent) :
    AdvanceFinder(generatorModel, nullptr, parent)
{
}

AdvanceFinder::AdvanceFinder(EggGeneratorModel5 *generatorModel, QTableView *sourceTableView, QWidget *parent) :
    AdvanceFinder(generatorModel, [generatorModel] { return generatorModel->getModel().size(); },
                  [generatorModel](size_t row) { return generatorModel->getModel()[row].getChatot(); },
                  [generatorModel](size_t row) { return generatorModel->getModel()[row].getSaveNeedle(); }, sourceTableView, parent)
{
}

AdvanceFinder::AdvanceFinder(EggGeneratorModel4 *generatorModel, QWidget *parent) :
    AdvanceFinder(generatorModel, nullptr, parent)
{
}

AdvanceFinder::AdvanceFinder(EggGeneratorModel4 *generatorModel, QTableView *sourceTableView, QWidget *parent) :
    AdvanceFinder(generatorModel, [generatorModel] { return generatorModel->getModel().size(); },
                  [generatorModel](size_t row) { return generatorModel->getModel()[row].getChatot(); }, ValueGetter(), sourceTableView,
                  parent)
{
}

AdvanceFinder::AdvanceFinder(HiddenGrottoSlotGeneratorModel5 *generatorModel, QWidget *parent) :
    AdvanceFinder(generatorModel, nullptr, parent)
{
}

AdvanceFinder::AdvanceFinder(HiddenGrottoSlotGeneratorModel5 *generatorModel, QTableView *sourceTableView, QWidget *parent) :
    AdvanceFinder(generatorModel, [generatorModel] { return generatorModel->getModel().size(); },
                  [generatorModel](size_t row) { return generatorModel->getModel()[row].getChatot(); },
                  [generatorModel](size_t row) { return generatorModel->getModel()[row].getSaveNeedle(); }, sourceTableView, parent)
{
}

AdvanceFinder::AdvanceFinder(HiddenGrottoGeneratorModel5 *generatorModel, QWidget *parent) :
    AdvanceFinder(generatorModel, nullptr, parent)
{
}

AdvanceFinder::AdvanceFinder(HiddenGrottoGeneratorModel5 *generatorModel, QTableView *sourceTableView, QWidget *parent) :
    AdvanceFinder(generatorModel, [generatorModel] { return generatorModel->getModel().size(); },
                  [generatorModel](size_t row) { return generatorModel->getModel()[row].getChatot(); },
                  [generatorModel](size_t row) { return generatorModel->getModel()[row].getSaveNeedle(); }, sourceTableView, parent)
{
}

AdvanceFinder::AdvanceFinder(QAbstractItemModel *generatorModel, RowCountGetter rowCountGetter, ValueGetter chatotGetter,
                             ValueGetter needleGetter, QTableView *sourceTableView, QWidget *parent) :
    QDialog(parent),
    generatorModel(generatorModel),
    sourceTableView(sourceTableView),
    rowCountGetter(std::move(rowCountGetter)),
    chatotGetter(std::move(chatotGetter)),
    needleGetter(std::move(needleGetter)),
    switchingMode(false)
{
    setWindowTitle(tr("Advance Finder"));
    resize(680, 430);
    setAttribute(Qt::WA_QuitOnClose, false);
    bool hasNeedles = static_cast<bool>(this->needleGetter);

    auto *mainLayout = new QVBoxLayout(this);

    auto *modeLayout = new QHBoxLayout;
    radioButtonChatot = new QRadioButton(tr("Chatot Pitches"), this);
    radioButtonNeedle = new QRadioButton(tr("Save Needles"), this);
    radioButtonChatot->setChecked(true);
    radioButtonChatot->setVisible(hasNeedles);
    radioButtonNeedle->setVisible(hasNeedles);
    if (hasNeedles)
    {
        modeLayout->addWidget(radioButtonChatot);
        modeLayout->addWidget(radioButtonNeedle);
        modeLayout->addStretch();
        mainLayout->addLayout(modeLayout);
    }

    groupBoxChatot = new QGroupBox(tr("Chatot Pitches"), this);
    auto *chatotLayout = new QGridLayout(groupBoxChatot);
    const QStringList chatotOptions = { tr("High"),     tr("Mid-High"),     tr("Mid"),       tr("Mid-Low"),       tr("Low"),
                                        tr("High / Mid-High"), tr("Mid-High / Mid"), tr("Mid / Mid-Low"), tr("Mid-Low / Low"), tr("Any") };
    for (int i = 0; i < chatotOptions.size(); i++)
    {
        auto *button = new QPushButton(chatotOptions[i], this);
        connect(button, &QPushButton::clicked, this, [=] { appendToken(chatotOptions[i]); });
        chatotLayout->addWidget(button, i / 5, i % 5);
    }
    mainLayout->addWidget(groupBoxChatot);

    groupBoxNeedle = new QGroupBox(tr("Save Needles"), this);
    auto *needleLayout = new QGridLayout(groupBoxNeedle);
    const QStringList needleOptions = { needleSymbol(0), needleSymbol(1), needleSymbol(2), needleSymbol(3), needleSymbol(4),
                                        needleSymbol(5), needleSymbol(6), needleSymbol(7), tr("Any") };

    const QVector<std::tuple<int, int, int>> needlePositions = { { 7, 0, 0 }, { 0, 0, 1 }, { 1, 0, 2 },
                                                                 { 6, 1, 0 }, { 8, 1, 1 }, { 2, 1, 2 },
                                                                 { 5, 2, 0 }, { 4, 2, 1 }, { 3, 2, 2 } };
    for (const auto &[option, row, column] : needlePositions)
    {
        auto *button = new QPushButton(needleOptions[option], this);
        connect(button, &QPushButton::clicked, this, [=] { appendToken(needleOptions[option]); });
        needleLayout->addWidget(button, row, column);
    }
    groupBoxNeedle->setVisible(false);
    if (hasNeedles)
    {
        mainLayout->addWidget(groupBoxNeedle);
    }

    auto *inputLayout = new QHBoxLayout;
    lineEditSequence = new QLineEdit(this);
    buttonJump = new QPushButton(tr("Jump to Advance"), this);
    buttonJump->setEnabled(false);
    auto *buttonRemove = new QPushButton(tr("Remove"), this);
    auto *buttonClear = new QPushButton(tr("Clear"), this);
    inputLayout->addWidget(lineEditSequence);
    inputLayout->addWidget(buttonJump);
    inputLayout->addWidget(buttonRemove);
    inputLayout->addWidget(buttonClear);
    mainLayout->addLayout(inputLayout);

    labelPossibleResults = new QLabel(this);
    mainLayout->addWidget(labelPossibleResults);

    tableWidgetResults = new QTableWidget(this);
    tableWidgetResults->setColumnCount(generatorModel->columnCount());
    QStringList headers;
    for (int i = 0; i < generatorModel->columnCount(); i++)
    {
        headers << generatorModel->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString();
    }
    tableWidgetResults->setHorizontalHeaderLabels(headers);
    tableWidgetResults->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidgetResults->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidgetResults->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    mainLayout->addWidget(tableWidgetResults);

    connect(radioButtonChatot, &QRadioButton::toggled, this, &AdvanceFinder::onModeChanged);
    connect(lineEditSequence, &QLineEdit::textChanged, this, &AdvanceFinder::sequenceTextChanged);
    connect(buttonJump, &QPushButton::clicked, this, &AdvanceFinder::jumpToAdvance);
    connect(buttonRemove, &QPushButton::clicked, this, &AdvanceFinder::removeToken);
    connect(buttonClear, &QPushButton::clicked, lineEditSequence, &QLineEdit::clear);

    search();
}

AdvanceFinder::~AdvanceFinder() = default;

void AdvanceFinder::appendToken(const QString &token)
{
    QString text = lineEditSequence->text().trimmed();
    text += text.isEmpty() ? token : ", " + token;
    lineEditSequence->setText(text);
}

void AdvanceFinder::removeToken()
{
    QStringList tokens = lineEditSequence->text().split(',', Qt::SkipEmptyParts);
    if (!tokens.isEmpty())
    {
        tokens.removeLast();
    }

    for (QString &token : tokens)
    {
        token = token.trimmed();
    }
    lineEditSequence->setText(tokens.join(", "));
}

void AdvanceFinder::sequenceTextChanged(const QString &text)
{
    if (!switchingMode)
    {
        if (radioButtonChatot->isChecked())
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

std::vector<QVector<u8>> AdvanceFinder::getChatotSequence(bool &valid) const
{
    std::vector<QVector<u8>> sequence;
    valid = true;

    QStringList tokens = lineEditSequence->text().split(',', Qt::SkipEmptyParts);
    for (QString token : tokens)
    {
        token = token.trimmed().toUpper();
        token.remove(' ');
        token.remove('_');

        if (token == "ANY" || token == "A" || token == "*")
        {
            sequence.emplace_back(chatotRange(0, 99));
        }
        else if (token == "HIGH" || token == "H")
        {
            sequence.emplace_back(chatotRange(80, 99));
        }
        else if (token == "MID-HIGH" || token == "MIDHIGH" || token == "MH")
        {
            sequence.emplace_back(chatotRange(60, 79));
        }
        else if (token == "MID" || token == "M")
        {
            sequence.emplace_back(chatotRange(40, 59));
        }
        else if (token == "MID-LOW" || token == "MIDLOW" || token == "ML")
        {
            sequence.emplace_back(chatotRange(20, 39));
        }
        else if (token == "LOW" || token == "L")
        {
            sequence.emplace_back(chatotRange(0, 19));
        }
        else if (token == "HIGH/MID-HIGH" || token == "HIGH/MIDHIGH" || token == "H/MH")
        {
            sequence.emplace_back(chatotRange(60, 99));
        }
        else if (token == "MID-HIGH/MID" || token == "MIDHIGH/MID" || token == "MH/M")
        {
            sequence.emplace_back(chatotRange(40, 79));
        }
        else if (token == "MID/MID-LOW" || token == "MID/MIDLOW" || token == "M/ML")
        {
            sequence.emplace_back(chatotRange(20, 59));
        }
        else if (token == "MID-LOW/LOW" || token == "MIDLOW/LOW" || token == "ML/L")
        {
            sequence.emplace_back(chatotRange(0, 39));
        }
        else
        {
            valid = false;
            return {};
        }
    }

    return sequence;
}

std::vector<QVector<u8>> AdvanceFinder::getNeedleSequence(bool &valid) const
{
    std::vector<QVector<u8>> sequence;
    valid = true;

    QStringList tokens = lineEditSequence->text().split(',', Qt::SkipEmptyParts);
    for (QString token : tokens)
    {
        token = token.trimmed().toUpper();

        if (token == "ANY" || token == "A" || token == "*")
        {
            sequence.emplace_back(QVector<u8> { 0, 1, 2, 3, 4, 5, 6, 7 });
        }
        else
        {
            bool ok;
            uint needle = token.toUInt(&ok);
            if (ok && needle < 8)
            {
                sequence.emplace_back(QVector<u8> { static_cast<u8>(needle) });
            }
            else
            {
                bool found = false;
                for (u8 i = 0; i < 8; i++)
                {
                    if (token == needleSymbol(i))
                    {
                        sequence.emplace_back(QVector<u8> { i });
                        found = true;
                        break;
                    }
                }

                if (!found)
                {
                    valid = false;
                    return {};
                }
            }
        }
    }

    return sequence;
}

void AdvanceFinder::search()
{
    bool valid;
    std::vector<QVector<u8>> sequence = radioButtonChatot->isChecked() ? getChatotSequence(valid) : getNeedleSequence(valid);

    tableWidgetResults->setRowCount(0);
    previewRows.clear();
    buttonJump->setEnabled(false);

    if (!valid)
    {
        labelPossibleResults->setText(tr("Possible Results: Invalid input"));
        return;
    }

    if (sequence.empty())
    {
        labelPossibleResults->setText(tr("Possible Results: %1").arg(rowCountGetter()));
        return;
    }

    std::vector<size_t> matches;
    size_t rows = rowCountGetter();
    for (size_t i = 0; i + sequence.size() <= rows; i++)
    {
        bool match = true;
        for (size_t j = 0; j < sequence.size() && match; j++)
        {
            u8 value = radioButtonChatot->isChecked() ? chatotGetter(i + j) : needleGetter(i + j);
            match = sequence[j].contains(value);
        }

        if (match)
        {
            matches.emplace_back(i);
        }
    }

    labelPossibleResults->setText(tr("Possible Results: %1").arg(matches.size()));
    displayResults(matches, sequence.size());
}

void AdvanceFinder::displayResults(const std::vector<size_t> &matches, size_t sequenceSize)
{
    tableWidgetResults->setRowCount(0);
    previewRows.clear();
    buttonJump->setEnabled(false);
    if (matches.size() > 10)
    {
        return;
    }

    for (size_t i = 0; i < matches.size(); i++)
    {
        int resultRow = static_cast<int>(i);
        int sourceRow = static_cast<int>(matches[i] + sequenceSize - 1);
        previewRows.emplace_back(sourceRow);
        tableWidgetResults->insertRow(resultRow);

        for (int column = 0; column < generatorModel->columnCount(); column++)
        {
            QVariant value = generatorModel->data(generatorModel->index(sourceRow, column), Qt::DisplayRole);
            tableWidgetResults->setItem(resultRow, column, new QTableWidgetItem(value.toString()));
        }
    }
    buttonJump->setEnabled(sourceTableView != nullptr && !previewRows.empty());
    tableWidgetResults->resizeColumnsToContents();
}

void AdvanceFinder::jumpToAdvance()
{
    if (!sourceTableView || previewRows.empty())
    {
        return;
    }

    int resultRow = tableWidgetResults->currentRow();
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
    bool chatot = radioButtonChatot->isChecked();
    groupBoxChatot->setVisible(chatot);
    groupBoxNeedle->setVisible(!chatot);

    switchingMode = true;
    lineEditSequence->setText(chatot ? chatotSequence : needleSequence);
    switchingMode = false;
    search();
}
