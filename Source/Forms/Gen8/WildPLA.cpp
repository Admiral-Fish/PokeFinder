#include "WildPLA.hpp"
#include "ui_WildPLA.h"

#include <Core/Gen8/Generators/WildGeneratorPLA.hpp>
#include <Core/Gen8/Profile8.hpp>
#include <Core/Parents/ProfileLoader.hpp>
#include <Forms/Controls/Controls.hpp>
#include <Forms/Gen8/Profile/ProfileManager8.hpp>
#include <Forms/Models/Gen8/RaidModel.hpp>
#include <QMenu>
#include <QSettings>

WildPLA::WildPLA(QWidget *parent) : QWidget(parent), ui(new Ui::WildPLA)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_QuitOnClose, false);

    updateProfiles();
    setupModels();
}

WildPLA::~WildPLA()
{
    QSettings setting;
    setting.beginGroup("WildPLA");
    setting.setValue("profile", ui->comboBoxProfiles->currentIndex());
    setting.setValue("geometry", this->saveGeometry());
    setting.endGroup();

    delete ui;
}

bool WildPLA::hasProfiles() const
{
    return !profiles.empty();
}

void WildPLA::updateProfiles()
{
    profiles.clear();
    auto completeProfiles = ProfileLoader8::getProfiles();
    std::copy_if(completeProfiles.begin(), completeProfiles.end(), std::back_inserter(profiles),
                 [](const Profile &profile) { return profile.getVersion() == Game::PAL; });
    profiles.insert(profiles.begin(), Profile8(Game::PAL));

    ui->comboBoxProfiles->clear();

    for (const auto &profile : profiles)
    {
        ui->comboBoxProfiles->addItem(QString::fromStdString(profile.getName()));
    }

    QSettings setting;
    int val = setting.value("WildPLA/profile", 0).toInt();
    if (val < ui->comboBoxProfiles->count())
    {
        ui->comboBoxProfiles->setCurrentIndex(val);
    }

    profilesIndexChanged(0);
}

void WildPLA::setupModels()
{
    model = new RaidModel(ui->tableView);

    menu = new QMenu(ui->tableView);

    ui->tableView->setModel(model);

    ui->textBoxSpawner0Seed->setValues(InputType::Seed64Bit);
    ui->textBoxInitialAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxMaxAdvances->setValues(InputType::Advance32Bit);

    QAction *outputTXTGenerator = menu->addAction(tr("Output Results to TXT"));
    QAction *outputCSVGenerator = menu->addAction(tr("Output Results to CSV"));
    connect(outputTXTGenerator, &QAction::triggered, [=] { ui->tableView->outputModel(); });
    connect(outputCSVGenerator, &QAction::triggered, [=] { ui->tableView->outputModel(true); });

    ui->filter->disableControls(Controls::UseDelay);

    connect(ui->comboBoxProfiles, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &WildPLA::profilesIndexChanged);
    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &WildPLA::generate);
    connect(ui->tableView, &QTableView::customContextMenuRequested, this, &WildPLA::tableViewContextMenu);
    connect(ui->pushButtonProfileManager, &QPushButton::clicked, this, &WildPLA::profileManager);

    QSettings setting;
    setting.beginGroup("WildPLA");
    if (setting.contains("geometry"))
    {
        this->restoreGeometry(setting.value("geometry").toByteArray());
    }
    setting.endGroup();
}

void WildPLA::generate()
{
    model->clearModel();

    u64 spawner0Seed = ui->textBoxSpawner0Seed->getULong();
    u32 initialAdvances = ui->textBoxInitialAdvances->getUInt();
    u32 maxAdvances = ui->textBoxMaxAdvances->getUInt();
    u16 tid = currentProfile->getTID();
    u16 sid = currentProfile->getSID();
    u8 rolls = 1;
    if (ui->comboBoxDexPage->currentIndex() > 0)
    {
        rolls += 1;
        if (ui->comboBoxDexPage->currentIndex() == 2)
        {
            rolls += 2;
        }
    }

    if (currentProfile->getShinyCharm())
    {
        rolls += 4;
    }

    StateFilter filter(ui->filter->getGender(), ui->filter->getAbility(), ui->filter->getShiny(), ui->filter->getDisableFilters(),
                       ui->filter->getMinIVs(), ui->filter->getMaxIVs(), ui->filter->getNatures(), {}, {});

    WildGeneratorPLA generator(initialAdvances, maxAdvances, tid, sid, ui->filter->getGenderRatio(), filter, rolls,
                               ui->checkBoxAlpha->isChecked());

    auto states = generator.generate(spawner0Seed);
    model->addItems(states);
}

void WildPLA::profilesIndexChanged(int index)
{
    if (index >= 0)
    {
        currentProfile = &profiles[index];

        ui->labelProfileTIDValue->setText(QString::number(currentProfile->getTID()));
        ui->labelProfileSIDValue->setText(QString::number(currentProfile->getSID()));
        ui->labelProfileGameValue->setText(QString::fromStdString(currentProfile->getVersionString()));
    }
}

void WildPLA::tableViewContextMenu(QPoint pos)
{
    if (model->rowCount() > 0)
    {
        menu->popup(ui->tableView->viewport()->mapToGlobal(pos));
    }
}

void WildPLA::profileManager()
{
    auto *manager = new ProfileManager8();
    connect(manager, &ProfileManager8::updateProfiles, this, [=] { emit alertProfiles(8); });
    manager->show();
}
