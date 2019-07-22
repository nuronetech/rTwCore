/***********************************************************************
**************Copyright (c) 2009-2010 Satoshi Nakamoto******************
***********Copyright (c) 2009-2012 The Bitcoin developers***************
******************Copyright (c) 2010-2019 Nur1Labs**********************
>Distributed under the MIT/X11 software license, see the accompanying
>file COPYING or http://www.opensource.org/licenses/mit-license.php.
************************************************************************/

#ifndef MUBDI_ECCRYPTOVERIFY_H
#define MUBDI_ECCRYPTOVERIFY_H

#include <cstdlib>
#include <vector>

class uint256;

namespace eccrypto
{
bool Check(const unsigned char* vch);
bool CheckSignatureElement(const unsigned char* vch, int len, bool half);

} // eccrypto namespace

#endif /* MUBDI_ECCRYPTOVERIFY_H */