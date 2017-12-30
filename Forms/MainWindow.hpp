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
#include <Forms/QCheckList.h>
#include <QStandardItemModel>
#include <QStandardItem>
#include <libPokeFinder/Gen3/FrameGen3.hpp>
#include <libPokeFinder/RNG/MTRNG.hpp>
#include <libPokeFinder/Gen3/GeneratorGen3.hpp>
#include <libPokeFinder/RNG/LCRNG.hpp>
#include <libPokeFinder/Objects/FrameCompare.hpp>
#include <libPokeFinder/Objects/Nature.hpp>
#include <libPokeFinder/Objects/Power.hpp>
#include <QTranslator>
#include <vector>
#include <QDir>
#include <QDebug>
#include <QDomDocument>
#include <libPokeFinder/Gen3/ProfileGen3.hpp>
#include <Forms/ProfileManager/ProfileManagerGen3.hpp>
#include <Util/Validators/IDValidator.hpp>
#include <Util/Validators/SeedValidator.hpp>
#include <Util/Validators/FrameValidator.hpp>
#include <Forms/Researcher.hpp>

using namespace std;
typedef uint32_t u32;

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
    Researcher *r;

    void loadLanguage(const QString& rLanguage);
    void createLanguageMenu(void);
    void setupModels();
    void createProfileXml();

private slots:
    void on_generateStationary3_clicked();
    void slotLanguageChanged(QAction* action);
    void updateProfiles();
    void on_saveProfileStationary3_clicked();
    void on_comboBoxProfiles_currentIndexChanged(int index);
    void checkLineEdits(QString str);
    void on_anyNatureStationary3_clicked();
    void on_anyHiddenPowerStationary3_clicked();
    void on_checkBoxDelayStationary3_clicked();
    void on_actionResearcher_triggered();
    void on_generateWild3_clicked();
    void on_anyHiddenPowerWild3_clicked();
    void on_anyNatureWild3_clicked();

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    vector<ProfileGen3> profiles;
    QValidator *idVal;
    QValidator *seedVal;
    QValidator *frameVal;

};

#endif // MAINWINDOW_H
