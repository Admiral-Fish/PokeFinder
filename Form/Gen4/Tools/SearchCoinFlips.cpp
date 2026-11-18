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

#include "SearchCoinFlips.hpp"
#include "SearchChatot.hpp"
#include "ui_SearchCoinFlips.h"
#include <Core/Gen4/SeedTime4.hpp>
#include <Core/Util/Utilities.hpp>
#include <Model/Gen4/SeedToTimeModel4.hpp>
#include <Model/IndexFilterProxyModel.hpp>
#include <QGridLayout>
#include <QSettings>
#include <QTabWidget>

SearchCoinFlips::SearchCoinFlips(SeedToTimeCalibrateModel4 *model, QWidget *parent) :
    QDialog(parent), ui(new Ui::SearchCoinFlips), model(model)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    tabWidget = new QTabWidget(this);
    auto *flipsPage = new QWidget(tabWidget);
    auto *flipsLayout = new QGridLayout(flipsPage);
    flipsLayout->addWidget(ui->pushButtonHeads, 0, 0, 1, 2);
    flipsLayout->addWidget(ui->pushButtonTails, 0, 2, 1, 2);
    flipsLayout->addWidget(ui->lineEditFlips, 1, 0, 1, 2);
    flipsLayout->addWidget(ui->pushButtonRemove, 1, 2);
    flipsLayout->addWidget(ui->pushButtonClear, 1, 3);
    tabWidget->addTab(flipsPage, tr("Coin Flips"));

    chatotSearch = new SearchChatot(model, [this](const QModelIndexList &results, int count) { setResults(results, count); }, tabWidget);
    tabWidget->addTab(chatotSearch, tr("Chatot"));

    ui->gridLayout->addWidget(tabWidget, 0, 0, 1, 4);
    ui->gridLayout->addWidget(ui->labelPossibleResults, 1, 0, 1, 2);
    ui->gridLayout->addWidget(ui->tableViewPreview, 2, 0, 1, 4);
    ui->gridLayout->addWidget(ui->pushButtonOkay, 3, 2);
    ui->gridLayout->addWidget(ui->pushButtonCancel, 3, 3);

    previewModel = new IndexFilterProxyModel(ui->tableViewPreview, model);
    ui->tableViewPreview->setModel(previewModel);

    ui->labelPossibleResults->setText(tr("Possible Results: %1").arg(model->rowCount()));

    connect(ui->pushButtonHeads, &QPushButton::clicked, this, &SearchCoinFlips::heads);
    connect(ui->pushButtonTails, &QPushButton::clicked, this, &SearchCoinFlips::tails);
    connect(ui->lineEditFlips, &QLineEdit::textChanged, this, &SearchCoinFlips::flipsTextChanged);
    connect(ui->pushButtonRemove, &QPushButton::clicked, this, &SearchCoinFlips::remove);
    connect(ui->pushButtonClear, &QPushButton::clicked, this, &SearchCoinFlips::clear);
    connect(ui->pushButtonOkay, &QPushButton::clicked, this, &SearchCoinFlips::accept);
    connect(ui->pushButtonCancel, &QPushButton::clicked, this, &SearchCoinFlips::reject);
    connect(tabWidget, &QTabWidget::currentChanged, this, [this](int index) {
        if (index == 0)
        {
            flipsTextChanged(ui->lineEditFlips->text());
        }
        else
        {
            chatotSearch->search();
        }
    });

    QSettings setting;
    if (setting.contains("searchCoinFlips/geometry"))
    {
        this->restoreGeometry(setting.value("searchCoinFlips/geometry").toByteArray());
    }
}

SearchCoinFlips::~SearchCoinFlips()
{
    QSettings setting;
    setting.setValue("searchCoinFlips/geometry", this->saveGeometry());

    delete ui;
}

QModelIndexList SearchCoinFlips::getIndexes() const
{
    const QModelIndex current = ui->tableViewPreview->currentIndex();
    if (current.isValid() && ui->tableViewPreview->selectionModel()->isSelected(current))
    {
        const QModelIndex source = previewModel->mapToSource(current);
        return { model->index(source.row(), 0) };
    }

    return indexes;
}

void SearchCoinFlips::updatePreview()
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

void SearchCoinFlips::setResults(const QModelIndexList &results, int count)
{
    indexes = results;
    ui->labelPossibleResults->setText(tr("Possible Results: %1").arg(count));
    updatePreview();
    ui->tableViewPreview->clearSelection();
    ui->tableViewPreview->setCurrentIndex(QModelIndex());
}

void SearchCoinFlips::clear()
{
    if (tabWidget->currentIndex() == 0)
    {
        ui->lineEditFlips->clear();
    }
    else
    {
        chatotSearch->clear();
    }
}

void SearchCoinFlips::flipsTextChanged(const QString &text)
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

void SearchCoinFlips::heads()
{
    QString string = ui->lineEditFlips->text();
    string += string.isEmpty() ? "H" : ", H";
    ui->lineEditFlips->setText(string);
}

void SearchCoinFlips::remove()
{
    if (tabWidget->currentIndex() != 0)
    {
        chatotSearch->remove();
        return;
    }
    QString string = ui->lineEditFlips->text();
    int index = string.lastIndexOf(',');
    ui->lineEditFlips->setText(index == -1 ? QString() : string.left(index));
}


void SearchCoinFlips::tails()
{
    QString string = ui->lineEditFlips->text();
    string += string.isEmpty() ? "T" : ", T";
    ui->lineEditFlips->setText(string);
}
