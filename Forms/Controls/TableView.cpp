/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2019 by Admiral_Fish, bumba, and EzPzStreamz
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

#include <QApplication>
#include <QClipboard>
#include <QFileDialog>
#include <QHeaderView>
#include <QKeyEvent>
#include <QTextStream>
#include "TableView.hpp"

TableView::TableView(QWidget *parent) :
    QTableView(parent)
{
}

void TableView::resizeEvent(QResizeEvent *event)
{
    QTableView::resizeEvent(event);

    QHeaderView *header = this->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::Stretch);

    for (int column = 0; column < header->count(); column++)
    {
        int width = header->sectionSize(column);
        header->setSectionResizeMode(column, QHeaderView::Interactive);
        header->resizeSection(column, width);
    }
}

void TableView::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->type() == QMouseEvent::MouseButtonDblClick)
    {
        QModelIndex index = this->currentIndex();
        if (index.isValid())
        {
            QString str = this->model()->data(index).toString();
            QApplication::clipboard()->setText(str);
        }
    }
}

void TableView::keyPressEvent(QKeyEvent *event)
{
    QTableView::keyPressEvent(event);

    if (event)
    {
        if ((event->key() == Qt::Key_C) && (event->modifiers() == Qt::ControlModifier))
        {
            QModelIndex index = this->currentIndex();
            if (index.isValid())
            {
                QString str = this->model()->data(index).toString();
                QApplication::clipboard()->setText(str);
            }
        }
    }
}

void TableView::outputModelTXT()
{
    QString fileName = QFileDialog::getSaveFileName(nullptr, QObject::tr("Save Output to TXT"), QDir::currentPath(), QObject::tr("Text File (*.txt);;All Files (*)"));

    if (fileName.isEmpty())
    {
        return;
    }

    QFile file(fileName);
    QAbstractItemModel *model = this->model();
    if (file.open(QIODevice::WriteOnly))
    {
        QTextStream ts(&file);
        int rows = model->rowCount();
        int columns = model->columnCount();

        QString header = "";
        for (int i = 0; i < columns; i++)
        {
            header += model->headerData(i, Qt::Horizontal, 0).toString();
            if (i != columns - 1)
            {
                header += "\t";
            }
        }
        header += "\n";

        for (int i = 0; i < rows; i++)
        {
            QString body = "";
            for (int j = 0; j < columns; j++)
            {
                QString entry = model->data(model->index(i, j)).toString();
                body += (entry.isEmpty() ? "-" : entry);
                if (i != columns - 1)
                {
                    body += "\t";
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

void TableView::outputModelCSV()
{
    QString fileName = QFileDialog::getSaveFileName(nullptr, QObject::tr("Save Output to CSV"), QDir::currentPath(), QObject::tr("CSV File (*.csv);;All Files (*)"));

    if (fileName.isEmpty())
    {
        return;
    }

    QFile file(fileName);
    QAbstractItemModel *model = this->model();
    if (file.open(QIODevice::WriteOnly))
    {
        QTextStream ts(&file);
        int rows = model->rowCount();
        int columns = model->columnCount();

        QString header = "";
        for (int i = 0; i < columns; i++)
        {
            header += model->headerData(i, Qt::Horizontal, 0).toString();
            if (i != columns - 1)
            {
                header += ",";
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
                    body += ",";
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
