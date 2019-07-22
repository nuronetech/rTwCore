/***********************************************************************
**********Copyright (c) 2014-2015 The Dash Core developers**************
*************Copyright (c) 2015-2017 The PIVX developers****************
******************Copyright (c) 2010-2019 Nur1Labs**********************
>Distributed under the MIT/X11 software license, see the accompanying
>file COPYING or http://www.opensource.org/licenses/mit-license.php.
************************************************************************/

#include "obfuscationconfig.h"
#include "ui_obfuscationconfig.h"

#include "mubdiunits.h"
#include "guiconstants.h"
#include "init.h"
#include "optionsmodel.h"
#include "walletmodel.h"

#include <QKeyEvent>
#include <QMessageBox>
#include <QPushButton>
#include <QSettings>

ObfuscationConfig::ObfuscationConfig(QWidget* parent) : QDialog(parent),
                                                        ui(new Ui::ObfuscationConfig),
                                                        model(0)
{
    ui->setupUi(this);

    connect(ui->buttonBasic, SIGNAL(clicked()), this, SLOT(clickBasic()));
    connect(ui->buttonHigh, SIGNAL(clicked()), this, SLOT(clickHigh()));
    connect(ui->buttonMax, SIGNAL(clicked()), this, SLOT(clickMax()));
}

ObfuscationConfig::~ObfuscationConfig()
{
    delete ui;
}

void ObfuscationConfig::setModel(WalletModel* model)
{
    this->model = model;
}

void ObfuscationConfig::clickBasic()
{
    configure(true, 1000, 2);

    QString strAmount(MuBdIUnits::formatWithUnit(
        model->getOptionsModel()->getDisplayUnit(), 1000 * COIN));
    QMessageBox::information(this, tr("Obfuscation Configuration"),
        tr(
            "Obfuscation was successfully set to basic (%1 and 2 rounds). You can change this at any time by opening rTw's configuration screen.")
            .arg(strAmount));

    close();
}

void ObfuscationConfig::clickHigh()
{
    configure(true, 1000, 8);

    QString strAmount(MuBdIUnits::formatWithUnit(
        model->getOptionsModel()->getDisplayUnit(), 1000 * COIN));
    QMessageBox::information(this, tr("Obfuscation Configuration"),
        tr(
            "Obfuscation was successfully set to high (%1 and 8 rounds). You can change this at any time by opening rTw's configuration screen.")
            .arg(strAmount));

    close();
}

void ObfuscationConfig::clickMax()
{
    configure(true, 1000, 16);

    QString strAmount(MuBdIUnits::formatWithUnit(
        model->getOptionsModel()->getDisplayUnit(), 1000 * COIN));
    QMessageBox::information(this, tr("Obfuscation Configuration"),
        tr(
            "Obfuscation was successfully set to maximum (%1 and 16 rounds). You can change this at any time by opening rTw's configuration screen.")
            .arg(strAmount));

    close();
}

void ObfuscationConfig::configure(bool enabled, int coins, int rounds)
{
    QSettings settings;

    settings.setValue("nObfuscationRounds", rounds);
    settings.setValue("nAnonymizeMuBdIAmount", coins);

    nZeromintPercentage = rounds;
    nAnonymizeMuBdIAmount = coins;
}
