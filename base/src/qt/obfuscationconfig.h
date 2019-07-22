/***********************************************************************
**********Copyright (c) 2014-2015 The Dash Core developers**************
*************Copyright (c) 2015-2017 The PIVX developers****************
******************Copyright (c) 2010-2019 Nur1Labs**********************
>Distributed under the MIT/X11 software license, see the accompanying
>file COPYING or http://www.opensource.org/licenses/mit-license.php.
************************************************************************/

#ifndef MUBDI_OBFUSCATIONCONFIG_H
#define MUBDI_OBFUSCATIONCONFIG_H

#include <QDialog>

namespace Ui
{
class ObfuscationConfig;
}
class WalletModel;

/** Multifunctional dialog to ask for passphrases. Used for encryption, unlocking, and changing the passphrase.
 */
class ObfuscationConfig : public QDialog
{
    Q_OBJECT

public:
    ObfuscationConfig(QWidget* parent = 0);
    ~ObfuscationConfig();

    void setModel(WalletModel* model);


private:
    Ui::ObfuscationConfig* ui;
    WalletModel* model;
    void configure(bool enabled, int coins, int rounds);

private slots:

    void clickBasic();
    void clickHigh();
    void clickMax();
};

#endif /* MUBDI_OBFUSCATIONCONFIG_H */