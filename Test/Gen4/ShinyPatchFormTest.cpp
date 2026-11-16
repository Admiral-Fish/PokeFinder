/*
 * This file is part of PokéFinder
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include <Form/Controls/TableView.hpp>
#include <Form/Controls/TextBox.hpp>
#include <Form/Gen4/Tools/ShinyPatch.hpp>
#include <QCheckBox>
#include <QLabel>
#include <QPushButton>
#include <QSettings>
#include <QSpinBox>
#include <QTemporaryDir>
#include <QTest>
#include <QTimer>

class ShinyPatchFormTest : public QObject
{
    Q_OBJECT
private:
    QTemporaryDir settings;

    static void setRange(ShinyPatch &form, const QString &seed, const QString &start, const QString &end)
    {
        form.findChild<TextBox *>("textBoxSeed")->setText(seed);
        form.findChild<TextBox *>("textBoxStart")->setText(start);
        form.findChild<TextBox *>("textBoxEnd")->setText(end);
    }

private slots:
    void initTestCase()
    {
        QVERIFY(settings.isValid());
        QCoreApplication::setOrganizationName("PokeFinder Tests");
        QCoreApplication::setApplicationName("Shiny Patch");
        QSettings::setDefaultFormat(QSettings::IniFormat);
        QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, settings.path());
    }

    void candidateAndTerrain()
    {
        ShinyPatch form;
        setRange(form, "A01513D6", "466", "466");
        auto *generate = form.findChild<QPushButton *>("pushButtonGenerate");
        auto *model = form.findChild<TableView *>("tableView")->model();
        generate->click();
        QTRY_VERIFY_WITH_TIMEOUT(generate->isEnabled(), 10000);
        QCOMPARE(model->rowCount(), 1);
        QCOMPARE(model->data(model->index(0, 0)).toUInt(), 466u);
        QCOMPARE(model->data(model->index(0, 1)).toString(), "11BAE100");
        QCOMPARE(model->data(model->index(0, 3)).toInt(), 3);
        QCOMPARE(model->data(model->index(0, 4)).toInt(), 4);
        QCOMPARE(model->data(model->index(0, 7)).toULongLong(), 472ull);

        form.findChild<QPushButton *>("tile_7_8")->click();
        generate->click();
        QTRY_VERIFY_WITH_TIMEOUT(generate->isEnabled(), 10000);
        QCOMPARE(model->rowCount(), 0);
    }

    void rejectedAndDetailed()
    {
        ShinyPatch form;
        setRange(form, "1D0B3634", "312", "312");
        auto *generate = form.findChild<QPushButton *>("pushButtonGenerate");
        auto *model = form.findChild<TableView *>("tableView")->model();
        generate->click();
        QTRY_VERIFY_WITH_TIMEOUT(generate->isEnabled(), 10000);
        QCOMPARE(model->rowCount(), 0);

        form.findChild<QCheckBox *>("checkBoxShinyOnly")->setChecked(false);
        generate->click();
        QTRY_VERIFY_WITH_TIMEOUT(generate->isEnabled(), 10000);
        QCOMPARE(model->rowCount(), 4);
        QCOMPARE(model->data(model->index(0, 5)).toInt(), 98);
        QCOMPARE(model->data(model->index(0, 6)).toString(), "-");
        QCOMPARE(model->data(model->index(0, 8)).toString(), "Chain breaks");
    }

    void maximumAdvanceAndCancel()
    {
        ShinyPatch form;
        setRange(form, "00000000", "4294967295", "4294967295");
        form.findChild<QCheckBox *>("checkBoxShinyOnly")->setChecked(false);
        auto *generate = form.findChild<QPushButton *>("pushButtonGenerate");
        auto *cancel = form.findChild<QPushButton *>("pushButtonCancel");
        auto *model = form.findChild<TableView *>("tableView")->model();
        generate->click();
        QTRY_VERIFY_WITH_TIMEOUT(generate->isEnabled(), 10000);
        QCOMPARE(model->rowCount(), 4);
        QCOMPARE(model->data(model->index(0, 0)).toUInt(), 0xffffffffu);
        QCOMPARE(model->data(model->index(0, 7)).toULongLong(), 4294967301ull);

        setRange(form, "00000000", "0", "4294967295");
        form.findChild<QCheckBox *>("checkBoxShinyOnly")->setChecked(true);
        form.findChild<QSpinBox *>("spinBoxChain")->setValue(0);
        generate->click();
        QVERIFY(cancel->isEnabled());
        QTimer::singleShot(0, cancel, &QPushButton::click);
        QTRY_VERIFY_WITH_TIMEOUT(generate->isEnabled(), 10000);
        QVERIFY(!cancel->isEnabled());
        QVERIFY(form.findChild<QLabel *>("labelStatus")->text().startsWith("Cancelled"));
    }

    void rowLimit()
    {
        ShinyPatch form;
        setRange(form, "00000000", "0", "30000");
        form.findChild<QCheckBox *>("checkBoxShinyOnly")->setChecked(false);
        auto *generate = form.findChild<QPushButton *>("pushButtonGenerate");
        generate->click();
        QTRY_VERIFY_WITH_TIMEOUT(generate->isEnabled(), 30000);
        QCOMPARE(form.findChild<TableView *>("tableView")->model()->rowCount(), 100000);
        QVERIFY(form.findChild<QLabel *>("labelStatus")->text().startsWith("Stopped at 100000"));
    }
};

QTEST_MAIN(ShinyPatchFormTest)
#include "ShinyPatchFormTest.moc"
