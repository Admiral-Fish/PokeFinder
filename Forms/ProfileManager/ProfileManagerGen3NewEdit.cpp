#include "ProfileManagerGen3NewEdit.hpp"
#include "ui_ProfileManagerGen3NewEdit.h"

ProfileManagerGen3NewEdit::ProfileManagerGen3NewEdit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProfileManagerGen3NewEdit)
{
    ui->setupUi(this);
}

ProfileManagerGen3NewEdit::ProfileManagerGen3NewEdit(QString profileName, int version, int language, uint32_t tid, uint32_t sid, bool deadBattery, QWidget *parent) : QDialog(parent), ui(new Ui::ProfileManagerGen3NewEdit)
{
    ui->setupUi(this);
    ui->lineEditProfile->setText(profileName);
    ui->comboBoxVersion->setCurrentIndex(version);
    ui->comboBoxLanguage->setCurrentIndex(language);
    ui->lineEditTID->setText(QString::number(tid));
    ui->lineEditSID->setText(QString::number(sid));
    ui->checkBoxDeadBattery->setChecked(deadBattery);

}

ProfileManagerGen3NewEdit::~ProfileManagerGen3NewEdit()
{
    delete ui;
}

void ProfileManagerGen3NewEdit::on_pushButtonAccept_clicked()
{
    bool pass;
    QMessageBox error;
    QString input;

    input = ui->lineEditProfile->text().trimmed();
    if(input == "")
    {
        error.setText("Please enter a Profile Name.");
        error.exec();
        return;
    }

    input = ui->lineEditTID->text().trimmed();
    if (input != "")
    {
        uint32_t tid = input.toUInt(&pass, 10);
        if (!pass)
        {
            error.setText("Please enter Trainer ID in valid decimal format.");
            error.exec();
            return;
        }
        else if (tid > 0xffff)
        {
            error.setText("Please enter a value lower then 65536.");
            error.exec();
            return;
        }
    }

    input = ui->lineEditSID->text().trimmed();
    if (input != "")
    {
        uint32_t sid = input.toUInt(&pass, 10);
        if (!pass)
        {
            error.setText("Please enter Trainer SID in valid decimal format.");
            error.exec();
            return;
        }
        else if (sid > 0xffff)
        {
            error.setText("Please enter a value lower then 65536.");
            error.exec();
            return;
        }
    }

    emit newProfile(ui->lineEditProfile->text(), ui->comboBoxVersion->currentIndex(), ui->comboBoxLanguage->currentIndex(), ui->lineEditTID->text().toUInt(&pass, 10), ui->lineEditSID->text().toUInt(&pass, 10), ui->checkBoxDeadBattery->isChecked());

    done(QDialog::Accepted);
}


void ProfileManagerGen3NewEdit::on_pushButtonCancel_clicked()
{
    done(QDialog::Rejected);
}
