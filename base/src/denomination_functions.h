/**
 * @file       denominations_functions.h
 *
 * @brief      Denomination functions for the Zerocoin library.
 *
 * @copyright  Copyright 2017 PIVX Developers
 * @license    This project is released under the MIT license.
 **/

/***********************************************************************
*************Copyright (c) 2015-2017 The PIVX developers****************
******************Copyright (c) 2010-2019 Nur1Labs**********************
>Distributed under the MIT/X11 software license, see the accompanying
>file COPYING or http://www.opensource.org/licenses/mit-license.php.
************************************************************************/

#ifndef MUBDI_DENOM_H
#define MUBDI_DENOM_H

#include "reverse_iterate.h"
#include "util.h"
#include "libzerocoin/Denominations.h"
#include "primitives/zerocoin.h"
#include <list>
#include <map>
std::vector<CZerocoinMint> SelectMintsFromList(const CAmount nValueTarget, CAmount& nSelectedValue,
                                               int nMaxNumberOfSpends,
                                               bool fMinimizeChange,
                                               int& nCoinsReturned,
                                               const std::list<CZerocoinMint>& listMints,
                                               const std::map<libzerocoin::CoinDenomination, CAmount> mapDenomsHeld,
                                               int& nNeededSpends
                                               );

int calculateChange(
    int nMaxNumberOfSpends,
    bool fMinimizeChange,
    const CAmount nValueTarget,
    const std::map<libzerocoin::CoinDenomination, CAmount>& mapOfDenomsHeld,
    std::map<libzerocoin::CoinDenomination, CAmount>& mapOfDenomsUsed);

void listSpends(const std::vector<CZerocoinMint>& vSelectedMints);

#endif /* MUBDI_DENOM_H */