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

#include "TableView.hpp"

TableView::TableView(QWidget *parent) : QTableView(parent)
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
    QAbstractItemModel *model = this->model();
    QString fileName = QFileDialog::getSaveFileName(nullptr, QObject::tr("Save Output to TXT"), QDir::currentPath(), QObject::tr("Text File (*.txt);;All Files (*)"));

    if (fileName.isEmpty())
    {
        return;
    }

    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly))
    {
        QString textData = "";
        int rows = model->rowCount();
        int columns = model->columnCount();

        for (int i = 0; i < columns; i++)
        {
            textData += model->headerData(i, Qt::Horizontal, 0).toString();
            if (i != columns - 1)
            {
                textData += "\t";
            }
        }
        textData += "\n";

        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < columns; j++)
            {
                QString entry = model->data(model->index(i, j)).toString();
                textData += (entry.isEmpty() ? "-" : entry);
                if (i != columns - 1)
                {
                    textData += "\t";
                }
            }
            textData += "\n";
        }

        QTextStream out(&file);
        out << textData;
        file.close();
    }
}

void TableView::outputModelCSV()
{
    QAbstractItemModel *model = this->model();
    QString fileName = QFileDialog::getSaveFileName(nullptr, QObject::tr("Save Output to CSV"), QDir::currentPath(), QObject::tr("CSV File (*.csv);;All Files (*)"));

    if (fileName.isEmpty())
    {
        return;
    }

    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly))
    {
        QString textData = "";
        int rows = model->rowCount();
        int columns = model->columnCount();

        for (int i = 0; i < columns; i++)
        {
            textData += model->headerData(i, Qt::Horizontal, 0).toString();
            if (i != columns - 1)
            {
                textData += ",";
            }
        }

        textData += "\n";

        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < columns; j++)
            {
                QString entry = model->data(model->index(i, j)).toString();
                textData += (entry.isEmpty() ? "-" : entry);
                if (j != columns - 1)
                {
                    textData += ",";
                }
            }
            textData += "\n";
        }

        QTextStream out(&file);
        out << textData;
        file.close();
    }
}
