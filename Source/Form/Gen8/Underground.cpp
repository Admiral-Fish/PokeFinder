#include "Underground.hpp"
#include "ui_Underground.h"
#include <Core/Enum/Encounter.hpp>
#include <Core/Enum/Lead.hpp>
#include <Core/Gen8/EncounterArea8.hpp>
#include <Core/Gen8/Encounters8.hpp>
#include <Core/Gen8/Profile8.hpp>
#include <Core/Parents/ProfileLoader.hpp>
#include <Core/Util/Translator.hpp>
#include <Form/Gen8/Profile/ProfileManager8.hpp>
#include <Model/Gen8/UndergroundModel.hpp>
#include <QMenu>
#include <QMessageBox>
#include <QSettings>

Underground::Underground(QWidget *parent) : QWidget(parent), ui(new Ui::Underground)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    model = new UndergroundModel(ui->tableView);
    ui->tableView->setModel(model);

    ui->textBoxSeed0->setValues(InputType::Seed64Bit);
    ui->textBoxSeed1->setValues(InputType::Seed64Bit);
    ui->textBoxInitialAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxMaxAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxDelay->setValues(InputType::Advance32Bit);

    ui->toolButtonLead->addAction(tr("None"), toInt(Lead::None));
    ui->toolButtonLead->addMenu(tr("Cute Charm"), { tr("♂ Lead"), tr("♀ Lead") }, { toInt(Lead::CuteCharmM), toInt(Lead::CuteCharmF) });
    ui->toolButtonLead->addAction(tr("Hustle"), toInt(Lead::Hustle));
    ui->toolButtonLead->addAction(tr("Pressure"), toInt(Lead::Pressure));
    ui->toolButtonLead->addMenu(tr("Synchronize"), *Translator::getNatures());
    ui->toolButtonLead->addAction(tr("Vital Spirit"), toInt(Lead::VitalSpirit));

    ui->comboBoxLocation->setup({ 196, 199, 189, 185, 198, 184, 191, 192, 186, 193, 183, 197, 194, 195, 188, 200, 187, 190 });

    connect(ui->comboBoxProfiles, &QComboBox::currentIndexChanged, this, &Underground::profileIndexChanged);
    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &Underground::generate);
    connect(ui->comboBoxStoryFlag, &QComboBox::currentIndexChanged, this, &Underground::storyFlagIndexChanged);
    connect(ui->comboBoxLocation, &QComboBox::currentIndexChanged, this, &Underground::locationIndexChanged);
    connect(ui->pushButtonProfileManager, &QPushButton::clicked, this, &Underground::profileManager);
    connect(ui->filter, &Filter::showStatsChanged, model, &UndergroundModel::setShowStats);

    updateProfiles();
    locationIndexChanged(0);

    QSettings setting;
    setting.beginGroup("underground");
    if (setting.contains("geometry"))
    {
        this->restoreGeometry(setting.value("geometry").toByteArray());
    }
    setting.endGroup();
}

Underground::~Underground()
{
    QSettings setting;
    setting.beginGroup("underground");
    setting.setValue("profile", ui->comboBoxProfiles->currentIndex());
    setting.setValue("geometry", this->saveGeometry());
    setting.endGroup();

    delete ui;
}

void Underground::updateProfiles()
{
    profiles.clear();
    auto completeProfiles = ProfileLoader8::getProfiles();
    std::copy_if(completeProfiles.begin(), completeProfiles.end(), std::back_inserter(profiles),
                 [](const Profile8 &profile) { return (profile.getVersion() & Game::BDSP) != Game::None; });
    profiles.insert(profiles.begin(), Profile8("-", Game::BD, 12345, 54321, false, false));

    ui->comboBoxProfiles->clear();

    for (const auto &profile : profiles)
    {
        ui->comboBoxProfiles->addItem(QString::fromStdString(profile.getName()));
    }

    QSettings setting;
    int val = setting.value("underground/profile", 0).toInt();
    if (val < ui->comboBoxProfiles->count())
    {
        ui->comboBoxProfiles->setCurrentIndex(val);
    }
}

void Underground::storyFlagIndexChanged(int index)
{
    encounters = Encounters8::getEncounters(Encounter::Underground, index + 1, false, false, currentProfile);
    locationIndexChanged(ui->comboBoxLocation->currentIndex());
}

void Underground::generate()
{
}

void Underground::locationIndexChanged(int index)
{
    if (index >= 0)
    {
        auto area = encounters[ui->comboBoxLocation->currentData().toInt() - 183];
        auto species = area.getUniqueSpecies();
        auto names = area.getSpecieNames();

        ui->comboBoxPokemon->clear();
        ui->comboBoxPokemon->addItem(QString("-"));
        for (size_t i = 0; i < species.size(); i++)
        {
            ui->comboBoxPokemon->addItem(QString::fromStdString(names[i]), species[i]);
        }
    }
}

void Underground::profileIndexChanged(int index)
{
    if (index >= 0)
    {
        currentProfile = &profiles[index];

        ui->labelProfileTIDValue->setText(QString::number(currentProfile->getTID()));
        ui->labelProfileSIDValue->setText(QString::number(currentProfile->getSID()));
        ui->labelProfileGameValue->setText(QString::fromStdString(*Translator::getGame(currentProfile->getVersion())));

        encounters = Encounters8::getEncounters(Encounter::Underground, index, false, false, currentProfile);
        locationIndexChanged(ui->comboBoxLocation->currentIndex());
    }
}

void Underground::profileManager()
{
    auto *manager = new ProfileManager8();
    connect(manager, &ProfileManager8::profilesModified, this, [=](int num) { emit profilesModified(num); });
    manager->show();
}
