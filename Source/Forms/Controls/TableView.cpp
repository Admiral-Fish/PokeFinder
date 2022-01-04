/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2022 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "TableView.hpp"
#include <QApplication>
#include <QClipboard>
#include <QFileDialog>
#include <QHeaderView>
#include <QKeyEvent>
#include <QTextStream>
#include <QTimer>

TableView::TableView(QWidget *parent) : QTableView(parent)
{
    QHeaderView *header = this->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::Interactive);

    QTimer::singleShot(500, [header] { header->resizeSections(QHeaderView::Stretch); });
}

void TableView::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event && event->type() == QMouseEvent::MouseButtonDblClick)
    {
        setSelectionToClipBoard();
    }
}

void TableView::keyPressEvent(QKeyEvent *event)
{
    QTableView::keyPressEvent(event);

    if (event && (event->key() == Qt::Key_C) && (event->modifiers() == Qt::ControlModifier))
    {
        setSelectionToClipBoard();
    }
}

void TableView::outputModel(bool csv) const
{
    QString caption = tr(csv ? "Save Output to CSV" : "Save Output to TXT");
    QString filter = tr(csv ? "CSV File (*.csv);;All Files (*)" : "Text File (*.txt);;All Files (*)");

    QString fileName = QFileDialog::getSaveFileName(nullptr, caption, QDir::currentPath(), filter);

    if (fileName.isEmpty())
    {
        return;
    }

    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly))
    {
        QAbstractItemModel *model = this->model();

        QTextStream ts(&file);
        int rows = model->rowCount();
        int columns = model->columnCount();

        QString header = "";
        for (int i = 0; i < columns; i++)
        {
            header += model->headerData(i, Qt::Horizontal, 0).toString();
            if (i != columns - 1)
            {
                header += csv ? "," : "\t";
            }
        }
        header += "\n";
        ts << header;

        for (int i = 0; i < rows; i++)
        {
            QString body = "";
            for (int j = 0; j < columns; j++)
            {
                QString entry = model->data(model->index(i, j)).toString();
                body += (entry.isEmpty() ? "-" : entry);
                if (j != columns - 1)
                {
                    body += csv ? "," : "\t";
                }
            }
            if (i != rows - 1)
            {
                body += "\n";
            }
            ts << body;
        }

        file.close();
    }
}

void TableView::setSelectionToClipBoard()
{
    QModelIndexList indexes = this->selectionModel()->selectedIndexes();
    if (!indexes.isEmpty())
    {
        QString selectedText;

        for (auto i = 0; i < indexes.size(); i++)
        {
            QModelIndex current = indexes[i];
            QString text = current.data().toString();

            if (i + 1 < selectedIndexes().count())
            {
                QModelIndex next = indexes[i + 1];

                if (next.row() != current.row())
                {
                    text += "\n";
                }
                else
                {
                    text += "\t";
                }
            }
            selectedText += text;
        }

        QApplication::clipboard()->setText(selectedText);
    }
}
