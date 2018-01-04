#include "ProfileManager3NewEdit.hpp"
#include "ui_ProfileManager3NewEdit.h"

ProfileManager3NewEdit::ProfileManager3NewEdit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProfileManager3NewEdit)
{
    ui->setupUi(this);
}

ProfileManager3NewEdit::ProfileManager3NewEdit(QString profileName, int version, int language, u32 tid, u32 sid, bool deadBattery, QWidget *parent) : QDialog(parent), ui(new Ui::ProfileManager3NewEdit)
{
    ui->setupUi(this);
    ui->lineEditProfile->setText(profileName);
    ui->comboBoxVersion->setCurrentIndex(version);
    ui->comboBoxLanguage->setCurrentIndex(language);
    ui->lineEditTID->setText(QString::number(tid));
    ui->lineEditSID->setText(QString::number(sid));
    ui->checkBoxDeadBattery->setChecked(deadBattery);
}

ProfileManager3NewEdit::~ProfileManager3NewEdit()
{
    delete ui;
}

void ProfileManager3NewEdit::on_pushButtonAccept_clicked()
{
    bool pass;
    QMessageBox error;
    QString input;

    input = ui->lineEditProfile->text().trimmed();
    if(input == "")
    {
        error.setText(tr("Please enter a Profile Name."));
        error.exec();
        return;
    }

    input = ui->lineEditTID->text().trimmed();
    if (input != "")
    {
        u32 tid = input.toUInt(&pass, 10);
        if (!pass)
        {
            error.setText(tr("Please enter Trainer ID in valid decimal format."));
            error.exec();
            return;
        }
        else if (tid > 0xffff)
        {
            error.setText(tr("Please enter a value lower then 65536."));
            error.exec();
            return;
        }
    }

    input = ui->lineEditSID->text().trimmed();
    if (input != "")
    {
        u32 sid = input.toUInt(&pass, 10);
        if (!pass)
        {
            error.setText(tr("Please enter Trainer SID in valid decimal format."));
            error.exec();
            return;
        }
        else if (sid > 0xffff)
        {
            error.setText(tr("Please enter a value lower then 65536."));
            error.exec();
            return;
        }
    }

    emit newProfile(ui->lineEditProfile->text(), ui->comboBoxVersion->currentIndex(), ui->comboBoxLanguage->currentIndex(), ui->lineEditTID->text().toUInt(&pass, 10), ui->lineEditSID->text().toUInt(&pass, 10), ui->checkBoxDeadBattery->isChecked());

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
