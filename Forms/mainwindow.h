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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QHeaderView>
#include <QMainWindow>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QStandardItem>
#include <Core/Gen3/FrameGen3.hpp>
#include <Core/RNG/MTRNG.hpp>
#include <Core/Gen3/GeneratorGen3.hpp>
#include <Core/RNG/LCRNG.hpp>
#include <Core/Objects/FrameCompare.hpp>
#include <cstdint>
#include <QTranslator>
#include <vector>
#include <QDir>
#include <QDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

protected:
    void changeEvent(QEvent*);


private:
    Ui::MainWindow *ui;
    QTranslator m_translator;
    QTranslator m_translatorQt;
    QString m_currLang;
    QString m_langPath;

    void loadLanguage(const QString& rLanguage);

    void createLanguageMenu(void);

    void setupModels();

private slots:
    void on_generate_clicked();
    void slotLanguageChanged(QAction* action);
    void natureItemCheck(QModelIndex a, QModelIndex b);
    void hiddenItemCheck(QModelIndex a, QModelIndex b);

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

};

#endif // MAINWINDOW_H
