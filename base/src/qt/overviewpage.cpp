/***********************************************************************
***********Copyright (c) 2009-2012 The Bitcoin developers***************
**********Copyright (c) 2014-2015 The Dash Core developers**************
*************Copyright (c) 2015-2017 The PIVX developers****************
******************Copyright (c) 2010-2019 Nur1Labs**********************
>Distributed under the MIT/X11 software license, see the accompanying
>file COPYING or http://www.opensource.org/licenses/mit-license.php.
************************************************************************/

#include "overviewpage.h"
#include "ui_overviewpage.h"

#include "mubdiunits.h"
#include "clientmodel.h"
#include "guiconstants.h"
#include "guiutil.h"
#include "init.h"
#include "obfuscation.h"
#include "obfuscationconfig.h"
#include "optionsmodel.h"
#include "transactionfilterproxy.h"
#include "transactionrecord.h"
#include "transactiontablemodel.h"
#include "walletmodel.h"

#include <QAbstractItemDelegate>
#include <QPainter>
#include <QSettings>
#include <QTimer>

#define DECORATION_SIZE 48
#define ICON_OFFSET 16
#define NUM_ITEMS 9

extern CWallet* pwalletMain;

class TxViewDelegate : public QAbstractItemDelegate
{
    Q_OBJECT
public:
    TxViewDelegate() : QAbstractItemDelegate(), unit(MuBdIUnits::MuBdI)
    {
    }

    inline void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
    {
        painter->save();

        QIcon icon = qvariant_cast<QIcon>(index.data(Qt::DecorationRole));
        QRect mainRect = option.rect;
        mainRect.moveLeft(ICON_OFFSET);
        QRect decorationRect(mainRect.topLeft(), QSize(DECORATION_SIZE, DECORATION_SIZE));
        int xspace = DECORATION_SIZE + 8;
        int ypad = 6;
        int halfheight = (mainRect.height() - 2 * ypad) / 2;
        QRect amountRect(mainRect.left() + xspace, mainRect.top() + ypad, mainRect.width() - xspace - ICON_OFFSET, halfheight);
        QRect addressRect(mainRect.left() + xspace, mainRect.top() + ypad + halfheight, mainRect.width() - xspace, halfheight);
        icon.paint(painter, decorationRect);

        QDateTime date = index.data(TransactionTableModel::DateRole).toDateTime();
        QString address = index.data(Qt::DisplayRole).toString();
        qint64 amount = index.data(TransactionTableModel::AmountRole).toLongLong();
        bool confirmed = index.data(TransactionTableModel::ConfirmedRole).toBool();

        // Check transaction status
        int nStatus = index.data(TransactionTableModel::StatusRole).toInt();
        bool fConflicted = false;
        if (nStatus == TransactionStatus::Conflicted || nStatus == TransactionStatus::NotAccepted) {
            fConflicted = true; // Most probably orphaned, but could have other reasons as well
        }
		bool fImmature = false;
        if (nStatus == TransactionStatus::Immature) {
            fImmature = true;
        }

        QVariant value = index.data(Qt::ForegroundRole);
        QColor foreground = COLOR_BLACK;
        if (value.canConvert<QBrush>()) {
            QBrush brush = qvariant_cast<QBrush>(value);
            foreground = brush.color();
        }

        painter->setPen(foreground);
        QRect boundingRect;
        painter->drawText(addressRect, Qt::AlignLeft | Qt::AlignVCenter, address, &boundingRect);

        if (index.data(TransactionTableModel::WatchonlyRole).toBool()) {
            QIcon iconWatchonly = qvariant_cast<QIcon>(index.data(TransactionTableModel::WatchonlyDecorationRole));
            QRect watchonlyRect(boundingRect.right() + 5, mainRect.top() + ypad + halfheight, 16, halfheight);
            iconWatchonly.paint(painter, watchonlyRect);
        }

        if(fConflicted) { // No need to check anything else for conflicted transactions
            foreground = COLOR_CONFLICTED;
		} else if (!confirmed || fImmature) {
            foreground = COLOR_UNCONFIRMED;
        } else if (amount < 0) {
            foreground = COLOR_NEGATIVE;
        } else {
            foreground = COLOR_BLACK;
        }
        painter->setPen(foreground);
        QString amountText = MuBdIUnits::formatWithUnit(unit, amount, true, MuBdIUnits::separatorAlways);
        if (!confirmed) {
            amountText = QString("[") + amountText + QString("]");
        }
        painter->drawText(amountRect, Qt::AlignRight | Qt::AlignVCenter, amountText);

        painter->setPen(COLOR_BLACK);
        painter->drawText(amountRect, Qt::AlignLeft | Qt::AlignVCenter, GUIUtil::dateTimeStr(date));

        painter->restore();
    }

    inline QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
    {
        return QSize(DECORATION_SIZE, DECORATION_SIZE);
    }

    int unit;
};
#include "overviewpage.moc"

OverviewPage::OverviewPage(QWidget* parent) : QWidget(parent),
                                              ui(new Ui::OverviewPage),
                                              clientModel(0),
                                              walletModel(0),
                                              currentBalance(-1),
                                              currentUnconfirmedBalance(-1),
                                              currentImmatureBalance(-1),
                                              currentZerocoinBalance(-1),
                                              currentUnconfirmedZerocoinBalance(-1),
                                              currentimmatureZerocoinBalance(-1),
                                              currentWatchOnlyBalance(-1),
                                              currentWatchUnconfBalance(-1),
                                              currentWatchImmatureBalance(-1),
                                              txdelegate(new TxViewDelegate()),
                                              filter(0)
{
    nDisplayUnit = 0; // just make sure it's not unitialized
    ui->setupUi(this);

    // Recent transactions
    ui->listTransactions->setItemDelegate(txdelegate);
    ui->listTransactions->setIconSize(QSize(DECORATION_SIZE, DECORATION_SIZE));
    ui->listTransactions->setMinimumHeight(NUM_ITEMS * (DECORATION_SIZE + 2));
    ui->listTransactions->setAttribute(Qt::WA_MacShowFocusRect, false);

    connect(ui->listTransactions, SIGNAL(clicked(QModelIndex)), this, SLOT(handleTransactionClicked(QModelIndex)));

    // init "out of sync" warning labels
    ui->labelWalletStatus->setText("(" + tr("out of sync") + ")");
    ui->labelTransactionsStatus->setText("(" + tr("out of sync") + ")");

    // start with displaying the "out of sync" warnings
    showOutOfSyncWarning(true);
}

void OverviewPage::handleTransactionClicked(const QModelIndex& index)
{
    if (filter)
        emit transactionClicked(filter->mapToSource(index));
}

OverviewPage::~OverviewPage()
{
    delete ui;
}

void OverviewPage::getPercentage(CAmount nUnlockedBalance, CAmount nZerocoinBalance, QString& sMuBdIPercentage, QString& szMuBdIPercentage)
{
    int nPrecision = 2;
    double dzPercentage = 0.0;

    if (nZerocoinBalance <= 0){
        dzPercentage = 0.0;
    }
    else{
        if (nUnlockedBalance <= 0){
            dzPercentage = 100.0;
        }
        else{
            dzPercentage = 100.0 * (double)(nZerocoinBalance / (double)(nZerocoinBalance + nUnlockedBalance));
        }
    }

    double dPercentage = 100.0 - dzPercentage;
    
    szMuBdIPercentage = "(" + QLocale(QLocale::system()).toString(dzPercentage, 'f', nPrecision) + " %)";
    sMuBdIPercentage = "(" + QLocale(QLocale::system()).toString(dPercentage, 'f', nPrecision) + " %)";
    
}

void OverviewPage::setBalance(const CAmount& balance, const CAmount& unconfirmedBalance, const CAmount& immatureBalance, 
                              const CAmount& zerocoinBalance, const CAmount& unconfirmedZerocoinBalance, const CAmount& immatureZerocoinBalance,
                              const CAmount& watchOnlyBalance, const CAmount& watchUnconfBalance, const CAmount& watchImmatureBalance)
{
    currentBalance = balance;
    currentUnconfirmedBalance = unconfirmedBalance;
    currentImmatureBalance = immatureBalance;
    currentZerocoinBalance = zerocoinBalance;
    currentUnconfirmedZerocoinBalance = unconfirmedZerocoinBalance;
    currentimmatureZerocoinBalance = immatureZerocoinBalance;
    currentWatchOnlyBalance = watchOnlyBalance;
    currentWatchUnconfBalance = watchUnconfBalance;
    currentWatchImmatureBalance = watchImmatureBalance;
	
	CAmount nLockedBalance = 0;
    CAmount nWatchOnlyLockedBalance = 0;
    if (pwalletMain) {
        nLockedBalance = pwalletMain->GetLockedCoins();
        nWatchOnlyLockedBalance = pwalletMain->GetLockedWatchOnlyBalance();
    }
    // AR Balance
    CAmount nTotalBalance = balance + unconfirmedBalance;
    CAmount MuBdIAvailableBalance = balance - immatureBalance - nLockedBalance;
    CAmount nTotalWatchBalance = watchOnlyBalance + watchUnconfBalance + watchImmatureBalance;    
    CAmount nUnlockedBalance = nTotalBalance - nLockedBalance;
    // zAR Balance
    CAmount matureZerocoinBalance = zerocoinBalance - immatureZerocoinBalance;
    // Percentages
    QString szPercentage = "";
    QString sPercentage = "";
    getPercentage(nUnlockedBalance, zerocoinBalance, sPercentage, szPercentage);
    // Combined balances
    CAmount availableTotalBalance = MuBdIAvailableBalance + matureZerocoinBalance;
    CAmount sumTotalBalance = nTotalBalance + zerocoinBalance;

    // MuBdI labels
    ui->labelBalance->setText(MuBdIUnits::floorHtmlWithUnit(nDisplayUnit, MuBdIAvailableBalance, false, MuBdIUnits::separatorAlways));
    ui->labelzBalance->setText(MuBdIUnits::floorHtmlWithUnit(nDisplayUnit, zerocoinBalance, false, MuBdIUnits::separatorAlways));
    ui->labelUnconfirmed->setText(MuBdIUnits::floorHtmlWithUnit(nDisplayUnit, unconfirmedBalance, false, MuBdIUnits::separatorAlways));
    ui->labelImmature->setText(MuBdIUnits::floorHtmlWithUnit(nDisplayUnit, immatureBalance, false, MuBdIUnits::separatorAlways));
    ui->labelLockedBalance->setText(MuBdIUnits::floorHtmlWithUnit(nDisplayUnit, nLockedBalance, false, MuBdIUnits::separatorAlways));
    ui->labelTotal->setText(MuBdIUnits::floorHtmlWithUnit(nDisplayUnit, nTotalBalance, false, MuBdIUnits::separatorAlways));

    // Watchonly labels
    ui->labelWatchAvailable->setText(MuBdIUnits::floorHtmlWithUnit(nDisplayUnit, watchOnlyBalance, false, MuBdIUnits::separatorAlways));
    ui->labelWatchPending->setText(MuBdIUnits::floorHtmlWithUnit(nDisplayUnit, watchUnconfBalance, false, MuBdIUnits::separatorAlways));
    ui->labelWatchImmature->setText(MuBdIUnits::floorHtmlWithUnit(nDisplayUnit, watchImmatureBalance, false, MuBdIUnits::separatorAlways));
    ui->labelWatchLocked->setText(MuBdIUnits::floorHtmlWithUnit(nDisplayUnit, nWatchOnlyLockedBalance, false, MuBdIUnits::separatorAlways));
    ui->labelWatchTotal->setText(MuBdIUnits::floorHtmlWithUnit(nDisplayUnit, nTotalWatchBalance, false, MuBdIUnits::separatorAlways));

    // zMuBdI labels
    ui->labelzBalance->setText(MuBdIUnits::floorHtmlWithUnit(nDisplayUnit, zerocoinBalance, false, MuBdIUnits::separatorAlways));
    ui->labelzBalanceUnconfirmed->setText(MuBdIUnits::floorHtmlWithUnit(nDisplayUnit, unconfirmedZerocoinBalance, false, MuBdIUnits::separatorAlways));
    ui->labelzBalanceMature->setText(MuBdIUnits::floorHtmlWithUnit(nDisplayUnit, matureZerocoinBalance, false, MuBdIUnits::separatorAlways));
    ui->labelzBalanceImmature->setText(MuBdIUnits::floorHtmlWithUnit(nDisplayUnit, immatureZerocoinBalance, false, MuBdIUnits::separatorAlways));
	
    // Combined labels
    ui->labelBalancez->setText(MuBdIUnits::floorHtmlWithUnit(nDisplayUnit, availableTotalBalance, false, MuBdIUnits::separatorAlways));
    ui->labelTotalz->setText(MuBdIUnits::floorHtmlWithUnit(nDisplayUnit, sumTotalBalance, false, MuBdIUnits::separatorAlways));
	
    // Percentage labels
    ui->labelMuBdIPercent->setText(sPercentage);
    ui->labelzMuBdIPercent->setText(szPercentage);

    // Adjust bubble-help according to AutoMint settings
    QString automintHelp = tr("Current percentage of zrTw.\nIf AutoMint is enabled this percentage will settle around the configured AutoMint percentage (default = 10%).\n");
    bool fEnableZeromint = GetBoolArg("-enablezeromint", true);
    int nZeromintPercentage = GetArg("-zeromintpercentage", 10);
    if (fEnableZeromint) {
        automintHelp += tr("AutoMint is currently enabled and set to ") + QString::number(nZeromintPercentage) + "%.\n";
        automintHelp += tr("To disable AutoMint add 'enablezeromint=0' in namecoin.conf.");
    }
    else {
        automintHelp += tr("AutoMint is currently disabled.\nTo enable AutoMint change 'enablezeromint=0' to 'enablezeromint=1' in namecoin.conf");
    }

        // Only show most balances if they are non-zero for the sake of simplicity
    QSettings settings;
    bool settingShowAllBalances = !settings.value("fHideZeroBalances").toBool();
    bool showSumAvailable = settingShowAllBalances || sumTotalBalance != availableTotalBalance;
    ui->labelBalanceTextz->setVisible(showSumAvailable);
    ui->labelBalancez->setVisible(showSumAvailable);
    bool showMuBdIAvailable = settingShowAllBalances || MuBdIAvailableBalance != nTotalBalance;
    bool showWatchOnlyMuBdIAvailable = watchOnlyBalance != nTotalWatchBalance;
    bool showMuBdIPending = settingShowAllBalances || unconfirmedBalance != 0;
    bool showWatchOnlyMuBdIPending = watchUnconfBalance != 0;
    bool showMuBdILocked = settingShowAllBalances || nLockedBalance != 0;
    bool showWatchOnlyMuBdILocked = nWatchOnlyLockedBalance != 0;
    bool showImmature = settingShowAllBalances || immatureBalance != 0;
    bool showWatchOnlyImmature = watchImmatureBalance != 0;

    bool showWatchOnly = nTotalWatchBalance != 0;
    ui->labelBalance->setVisible(showMuBdIAvailable || showWatchOnlyMuBdIAvailable);
    ui->labelBalanceText->setVisible(showMuBdIAvailable || showWatchOnlyMuBdIAvailable);
    ui->labelWatchAvailable->setVisible(showMuBdIAvailable && showWatchOnly);
    ui->labelUnconfirmed->setVisible(showMuBdIPending || showWatchOnlyMuBdIPending);
    ui->labelPendingText->setVisible(showMuBdIPending || showWatchOnlyMuBdIPending);
    ui->labelWatchPending->setVisible(showMuBdIPending && showWatchOnly);
    ui->labelLockedBalance->setVisible(showMuBdILocked || showWatchOnlyMuBdILocked);
    ui->labelLockedBalanceText->setVisible(showMuBdILocked || showWatchOnlyMuBdILocked);
    ui->labelWatchLocked->setVisible(showMuBdILocked && showWatchOnly);
    ui->labelImmature->setVisible(showImmature || showWatchOnlyImmature); // for symmetry reasons also show immature label when the watch-only one is shown
    ui->labelImmatureText->setVisible(showImmature || showWatchOnlyImmature);
    ui->labelWatchImmature->setVisible(showImmature && showWatchOnly); // show watch-only immature balance
    bool showzMuBdIAvailable = settingShowAllBalances || zerocoinBalance != matureZerocoinBalance;
    bool showzMuBdIUnconfirmed = settingShowAllBalances || unconfirmedZerocoinBalance != 0;
    bool showzMuBdIImmature = settingShowAllBalances || immatureZerocoinBalance != 0;
    ui->labelzBalanceMature->setVisible(showzMuBdIAvailable);
    ui->labelzBalanceMatureText->setVisible(showzMuBdIAvailable);
    ui->labelzBalanceUnconfirmed->setVisible(showzMuBdIUnconfirmed);
    ui->labelzBalanceUnconfirmedText->setVisible(showzMuBdIUnconfirmed);
    ui->labelzBalanceImmature->setVisible(showzMuBdIImmature);
    ui->labelzBalanceImmatureText->setVisible(showzMuBdIImmature);
    bool showPercentages = ! (zerocoinBalance == 0 && nTotalBalance == 0);
    ui->labelMuBdIPercent->setVisible(showPercentages);
    ui->labelzMuBdIPercent->setVisible(showPercentages);

    static int cachedTxLocks = 0;

    if (cachedTxLocks != nCompleteTXLocks) {
        cachedTxLocks = nCompleteTXLocks;
        ui->listTransactions->update();
    }
}

// show/hide watch-only labels
void OverviewPage::updateWatchOnlyLabels(bool showWatchOnly)
{
    ui->labelSpendable->setVisible(showWatchOnly);      // show spendable label (only when watch-only is active)
    ui->labelWatchonly->setVisible(showWatchOnly);      // show watch-only label
    ui->labelWatchAvailable->setVisible(showWatchOnly); // show watch-only available balance
    ui->labelWatchPending->setVisible(showWatchOnly);   // show watch-only pending balance
	ui->labelWatchLocked->setVisible(showWatchOnly);     // show watch-only total balance
    ui->labelWatchTotal->setVisible(showWatchOnly);     // show watch-only total balance

    if (!showWatchOnly) {
        ui->labelWatchImmature->hide();
    } else {
        ui->labelBalance->setIndent(20);
        ui->labelUnconfirmed->setIndent(20);
		ui->labelLockedBalance->setIndent(20);
        ui->labelImmature->setIndent(20);
        ui->labelTotal->setIndent(20);
    }
}

void OverviewPage::setClientModel(ClientModel* model)
{
    this->clientModel = model;
    if (model) {
        // Show warning if this is a prerelease version
        connect(model, SIGNAL(alertsChanged(QString)), this, SLOT(updateAlerts(QString)));
        updateAlerts(model->getStatusBarWarnings());
    }
}

void OverviewPage::setWalletModel(WalletModel* model)
{
    this->walletModel = model;
    if (model && model->getOptionsModel()) {
        // Set up transaction list
        filter = new TransactionFilterProxy();
        filter->setSourceModel(model->getTransactionTableModel());
        filter->setLimit(NUM_ITEMS);
        filter->setDynamicSortFilter(true);
        filter->setSortRole(Qt::EditRole);
        filter->setShowInactive(false);
        filter->sort(TransactionTableModel::Date, Qt::DescendingOrder);

        ui->listTransactions->setModel(filter);
        ui->listTransactions->setModelColumn(TransactionTableModel::ToAddress);

        // Keep up to date with wallet
        setBalance(model->getBalance(), model->getUnconfirmedBalance(), model->getImmatureBalance(),
                   model->getZerocoinBalance(), model->getUnconfirmedZerocoinBalance(), model->getImmatureZerocoinBalance(), 
                   model->getWatchBalance(), model->getWatchUnconfirmedBalance(), model->getWatchImmatureBalance());
        connect(model, SIGNAL(balanceChanged(CAmount, CAmount, CAmount, CAmount, CAmount, CAmount, CAmount, CAmount, CAmount)), this, 
                         SLOT(setBalance(CAmount, CAmount, CAmount, CAmount, CAmount, CAmount, CAmount, CAmount, CAmount)));

        connect(model->getOptionsModel(), SIGNAL(displayUnitChanged(int)), this, SLOT(updateDisplayUnit()));
		connect(model->getOptionsModel(), SIGNAL(hideZeroBalancesChanged(bool)), this, SLOT(updateDisplayUnit()));

        updateWatchOnlyLabels(model->haveWatchOnly());
        connect(model, SIGNAL(notifyWatchonlyChanged(bool)), this, SLOT(updateWatchOnlyLabels(bool)));
    }

    // update the display unit, to not use the default ("MuBdI")
    updateDisplayUnit();
}

void OverviewPage::updateDisplayUnit()
{
    if (walletModel && walletModel->getOptionsModel()) {
        nDisplayUnit = walletModel->getOptionsModel()->getDisplayUnit();
        if (currentBalance != -1)
            setBalance(currentBalance, currentUnconfirmedBalance, currentImmatureBalance, currentZerocoinBalance, currentUnconfirmedZerocoinBalance, currentimmatureZerocoinBalance,
                currentWatchOnlyBalance, currentWatchUnconfBalance, currentWatchImmatureBalance);

        // Update txdelegate->unit with the current unit
        txdelegate->unit = nDisplayUnit;

        ui->listTransactions->update();
    }
}

void OverviewPage::updateAlerts(const QString& warnings)
{
    this->ui->labelAlerts->setVisible(!warnings.isEmpty());
    this->ui->labelAlerts->setText(warnings);
}

void OverviewPage::showOutOfSyncWarning(bool fShow)
{
    ui->labelWalletStatus->setVisible(fShow);
    ui->labelTransactionsStatus->setVisible(fShow);
}