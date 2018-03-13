#include "ProfileManager3NewEdit.hpp"
#include "ui_ProfileManager3NewEdit.h"

ProfileManager3NewEdit::ProfileManager3NewEdit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProfileManager3NewEdit)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    setupModels();
}

ProfileManager3NewEdit::ProfileManager3NewEdit(Profile3 profile, QWidget *parent) : QDialog(parent), ui(new Ui::ProfileManager3NewEdit)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    setupModels();

    ui->lineEditProfile->setText(profile.profileName);
    ui->comboBoxVersion->setCurrentIndex(profile.version);
    ui->comboBoxLanguage->setCurrentIndex(profile.language);
    ui->textBoxTID->setText(QString::number(profile.tid));
    ui->textBoxSID->setText(QString::number(profile.sid));
    ui->checkBoxDeadBattery->setChecked(profile.deadBattery);
    isEditing = true;
    original = profile;
}

ProfileManager3NewEdit::~ProfileManager3NewEdit()
{
    delete ui;
}

void ProfileManager3NewEdit::changeEvent(QEvent *event)
{
    if (event != NULL)
    {
        switch (event->type())
        {
            case QEvent::LanguageChange:
                ui->retranslateUi(this);
                break;
            default:
                break;
        }
    }
}

void ProfileManager3NewEdit::setupModels()
{
    ui->textBoxTID->setValues(0, 48, true);
    ui->textBoxSID->setValues(0, 48, true);
}

void ProfileManager3NewEdit::on_pushButtonAccept_clicked()
{
    QString input = ui->lineEditProfile->text().trimmed();
    if (input == "")
    {
        QMessageBox error;
        error.setText(tr("Enter a Profile Name."));
        error.exec();
        return;
    }

    Profile3 profile(ui->lineEditProfile->text(), ui->comboBoxVersion->currentIndex(), ui->textBoxTID->text().toUInt(NULL, 10),
                     ui->textBoxSID->text().toUInt(NULL, 10), ui->comboBoxLanguage->currentIndex(), ui->checkBoxDeadBattery->isChecked());

    if (isEditing)
    {
        emit editProfile(profile, original);
    }
    else
    {
        emit newProfile(profile);
    }

    done(QDialog::Accepted);
}


void ProfileManager3NewEdit::on_pushButtonCancel_clicked()
{
    done(QDialog::Rejected);
}

void ProfileManager3NewEdit::on_comboBoxVersion_currentIndexChanged(int index)
{
    if(index > 1)
    {
        ui->checkBoxDeadBattery->setEnabled(false);
        ui->checkBoxDeadBattery->setChecked(false);
    }
    else
    {
        ui->checkBoxDeadBattery->setEnabled(true);
    }
}
