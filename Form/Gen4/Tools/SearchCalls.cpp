/*
 * This file is part of PokéFinder
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

#include "SearchCalls.hpp"
#include "SearchChatot.hpp"
#include "ui_SearchCalls.h"
#include <Core/Gen4/SeedTime4.hpp>
#include <Core/Util/Utilities.hpp>
#include <Model/Gen4/SeedToTimeModel4.hpp>
#include <Model/IndexFilterProxyModel.hpp>
#include <QGridLayout>
#include <QSettings>
#include <QTabWidget>

SearchCalls::SearchCalls(SeedToTimeCalibrateModel4 *model, QWidget *parent) : QDialog(parent), ui(new Ui::SearchCalls), model(model)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    tabWidget = new QTabWidget(this);
    auto *callsPage = new QWidget(tabWidget);
    auto *callsLayout = new QGridLayout(callsPage);
    callsLayout->addWidget(ui->pushButtonK, 0, 0, 1, 2);
    callsLayout->addWidget(ui->pushButtonE, 0, 2, 1, 2);
    callsLayout->addWidget(ui->pushButtonP, 0, 4, 1, 2);
    callsLayout->addWidget(ui->lineEditCalls, 1, 0, 1, 4);
    callsLayout->addWidget(ui->pushButtonRemove, 1, 4);
    callsLayout->addWidget(ui->pushButtonClear, 1, 5);
    callsLayout->addWidget(ui->radioButtonElm, 2, 0);
    callsLayout->addWidget(ui->radioButtonIrwin, 2, 1);
    callsLayout->addWidget(ui->labelKResponse, 3, 0, 1, 6);
    callsLayout->addWidget(ui->labelEResponse, 4, 0, 1, 6);
    callsLayout->addWidget(ui->labelPResponse, 5, 0, 1, 6);
    tabWidget->addTab(callsPage, tr("Calls"));

    chatotSearch = new SearchChatot(model, [this](const QModelIndexList &results, int count) { setResults(results, count); }, tabWidget);
    tabWidget->addTab(chatotSearch, tr("Chatot"));

    ui->gridLayout->addWidget(tabWidget, 0, 0, 1, 6);
    ui->gridLayout->addWidget(ui->labelPossibleResults, 1, 0, 1, 2);
    ui->gridLayout->addWidget(ui->tableViewPreview, 2, 0, 1, 6);
    ui->gridLayout->addWidget(ui->pushButtonOkay, 3, 4);
    ui->gridLayout->addWidget(ui->pushButtonCancel, 3, 5);

    previewModel = new IndexFilterProxyModel(ui->tableViewPreview, model);
    ui->tableViewPreview->setModel(previewModel);

    ui->labelPossibleResults->setText(tr("Possible Results: %1").arg(model->rowCount()));

    connect(ui->pushButtonE, &QPushButton::clicked, this, &SearchCalls::e);
    connect(ui->pushButtonK, &QPushButton::clicked, this, &SearchCalls::k);
    connect(ui->pushButtonP, &QPushButton::clicked, this, &SearchCalls::p);
    connect(ui->lineEditCalls, &QLineEdit::textChanged, this, &SearchCalls::callsTextChanged);
    connect(ui->radioButtonElm, &QRadioButton::clicked, this, &SearchCalls::elm);
    connect(ui->radioButtonIrwin, &QRadioButton::clicked, this, &SearchCalls::irwin);
    connect(ui->pushButtonRemove, &QPushButton::clicked, this, &SearchCalls::remove);
    connect(ui->pushButtonClear, &QPushButton::clicked, this, &SearchCalls::clear);
    connect(ui->pushButtonOkay, &QPushButton::clicked, this, &SearchCalls::accept);
    connect(ui->pushButtonCancel, &QPushButton::clicked, this, &SearchCalls::reject);
    connect(tabWidget, &QTabWidget::currentChanged, this, [this](int index) {
        if (index == 0)
        {
            callsTextChanged(ui->lineEditCalls->text());
        }
        else
        {
            chatotSearch->search();
        }
    });

    QSettings setting;
    if (setting.contains("searchCalls/geometry"))
    {
        this->restoreGeometry(setting.value("searchCalls/geometry").toByteArray());
    }
}

SearchCalls::~SearchCalls()
{
    QSettings setting;
    setting.setValue("searchCalls/geometry", this->saveGeometry());

    delete ui;
}

QModelIndexList SearchCalls::getIndexes() const
{
    const QModelIndex current = ui->tableViewPreview->currentIndex();
    if (current.isValid() && ui->tableViewPreview->selectionModel()->isSelected(current))
    {
        const QModelIndex source = previewModel->mapToSource(current);
        return { model->index(source.row(), 0) };
    }

    return indexes;
}

void SearchCalls::updatePreview()
{
    if (!indexes.empty() && indexes.size() <= 5)
    {
        previewModel->setFilteredIndexes(indexes);
    }
    else
    {
        previewModel->setFilteredIndexes(QModelIndexList());
    }
}

void SearchCalls::setResults(const QModelIndexList &results, int count)
{
    indexes = results;
    ui->labelPossibleResults->setText(tr("Possible Results: %1").arg(count));
    updatePreview();
    ui->tableViewPreview->clearSelection();
    ui->tableViewPreview->setCurrentIndex(QModelIndex());
}

void SearchCalls::callsTextChanged(const QString &text)
{
    if (!text.isEmpty())
    {
        std::string result = text.toUpper().toStdString();
        std::erase_if(result, [](char c) { return c == ' ' || c == ','; });

        int num = 0;

        indexes.clear();
        for (int i = 0; i < model->rowCount(); i++)
        {
            std::string compare = model->getItem(i).getSequence();
            if (compare.find("skipped") != std::string::npos)
            {
                size_t index = compare.find(')');
                compare = compare.substr(index, compare.size() - index);
            }

            std::erase_if(compare, [](char c) { return c == ' ' || c == ','; });

            bool pass = compare.find(result) != std::string::npos;
            if (pass)
            {
                indexes.append(model->index(i, 0));
                num++;
            }
        }

        ui->labelPossibleResults->setText(tr("Possible Results: %1").arg(num));
        updatePreview();
    }
    else
    {
        indexes.clear();
        ui->labelPossibleResults->setText(tr("Possible Results: %1").arg(model->rowCount()));
        updatePreview();
    }
}

void SearchCalls::clear()
{
    if (tabWidget->currentIndex() == 0)
    {
        ui->lineEditCalls->clear();
    }
    else
    {
        chatotSearch->clear();
    }
}

void SearchCalls::e()
{
    QString string = ui->lineEditCalls->text();
    string += string.isEmpty() ? "E" : ", E";
    ui->lineEditCalls->setText(string);
}

void SearchCalls::elm()
{
    ui->labelKResponse->setText(tr("K - I expect there are some Pokémon in the Kanto region that I don't know. There are probably methods "
                                   "of evolution that I'm not familiar with yet. I should use that perspective and discover what I can!"));
    ui->labelEResponse->setText(tr("E - There are so many different ways that Pokémon evolve, aren't there?! Some "
                                   "Pokémon don't even evolve until they meet certain conditions first!"));
    ui->labelPResponse->setText(tr("P - It seems that Pokémon that have been infected with Pokérus level up "
                                   "better. We're not quite sure why..."));
}

void SearchCalls::irwin()
{
    ui->labelKResponse->setText(tr("K - I'm so glad you called! I was just about to call you, too! I guess we must be a good match!"));
    ui->labelEResponse->setText(tr("E - Hearing about your escapades rocks my soul! It sure does!"));
    ui->labelPResponse->setText(tr("P - How are you? What are you doing? Where are you? How many Badges do you have now? How much money "
                                   "have you saved? How's your mom? Have you got lots of Pokémon? Is it going to be sunny tomorrow? Arrgh, "
                                   "there's so much I want to chat about! This is going nowhere!"));
}

void SearchCalls::k()
{
    QString string = ui->lineEditCalls->text();
    string += string.isEmpty() ? "K" : ", K";
    ui->lineEditCalls->setText(string);
}

void SearchCalls::p()
{
    QString string = ui->lineEditCalls->text();
    string += string.isEmpty() ? "P" : ", P";
    ui->lineEditCalls->setText(string);
}

void SearchCalls::remove()
{
    if (tabWidget->currentIndex() != 0)
    {
        chatotSearch->remove();
        return;
    }
    QString string = ui->lineEditCalls->text();
    int index = string.lastIndexOf(',');
    ui->lineEditCalls->setText(index == -1 ? QString() : string.left(index));
}
