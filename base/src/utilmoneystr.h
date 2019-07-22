/***********************************************************************
**************Copyright (c) 2009-2010 Satoshi Nakamoto******************
***********Copyright (c) 2009-2012 The Bitcoin developers***************
******************Copyright (c) 2010-2019 Nur1Labs**********************
>Distributed under the MIT/X11 software license, see the accompanying
>file COPYING or http://www.opensource.org/licenses/mit-license.php.
************************************************************************/

/**
 * Money parsing/formatting utilities.
 */
 
#ifndef MUBDI_UTILMONEYSTR_H
#define MUBDI_UTILMONEYSTR_H

#include <stdint.h>
#include <string>

#include "amount.h"

std::string FormatMoney(const CAmount& n, bool fPlus = false);
bool ParseMoney(const std::string& str, CAmount& nRet);
bool ParseMoney(const char* pszIn, CAmount& nRet);

#endif /* MUBDI_UTILMONEYSTR_H */