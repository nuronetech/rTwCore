/***********************************************************************
***********Copyright (c) 2009-2012 The Bitcoin developers***************
******************Copyright (c) 2010-2019 Nur1Labs**********************
>Distributed under the MIT/X11 software license, see the accompanying
>file COPYING or http://www.opensource.org/licenses/mit-license.php.
************************************************************************/

#ifndef MUBDI_QT_MUBDIAMOUNTFIELD_H
#define MUBDI_QT_MUBDIAMOUNTFIELD_H

#include "amount.h"

#include <QWidget>

class AmountSpinBox;

QT_BEGIN_NAMESPACE
class QValueComboBox;
QT_END_NAMESPACE

/** Widget for entering mubdi amounts.
  */
class MuBdIAmountField : public QWidget
{
    Q_OBJECT

    // ugly hack: for some unknown reason CAmount (instead of qint64) does not work here as expected
    // discussion: https://github.com/mubdi/mubdi/pull/5117
    Q_PROPERTY(qint64 value READ value WRITE setValue NOTIFY valueChanged USER true)

public:
    explicit MuBdIAmountField(QWidget* parent = 0);

    CAmount value(bool* value = 0) const;
    void setValue(const CAmount& value);

    /** Set single step in satoshis **/
    void setSingleStep(const CAmount& step);

    /** Make read-only **/
    void setReadOnly(bool fReadOnly);

    /** Mark current value as invalid in UI. */
    void setValid(bool valid);
    /** Perform input validation, mark field as invalid if entered value is not valid. */
    bool validate();

    /** Change unit used to display amount. */
    void setDisplayUnit(int unit);

    /** Make field empty and ready for new input. */
    void clear();

    /** Enable/Disable. */
    void setEnabled(bool fEnabled);

    /** Qt messes up the tab chain by default in some cases (issue https://bugreports.qt-project.org/browse/QTBUG-10907),
        in these cases we have to set it up manually.
    */
    QWidget* setupTabChain(QWidget* prev);

signals:
    void valueChanged();

protected:
    /** Intercept focus-in event and ',' key presses */
    bool eventFilter(QObject* object, QEvent* event);

private:
    AmountSpinBox* amount;
    QValueComboBox* unit;

private slots:
    void unitChanged(int idx);
};

#endif /* MUBDI_QT_MUBDIAMOUNTFIELD_H */