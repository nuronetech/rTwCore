/***********************************************************************
*************Copyright (c) 2015-2017 The PIVX developers****************
******************Copyright (c) 2010-2019 Nur1Labs**********************
>Distributed under the MIT/X11 software license, see the accompanying
>file COPYING or http://www.opensource.org/licenses/mit-license.php.
************************************************************************/

#include "primitives/zerocoin.h"
#include "util.h"
#include "utilstrencodings.h"
 
bool CZerocoinMint::GetKeyPair(CKey &key) const
{
    if (version < STAKABLE_VERSION)
        return error("%s: version is %d", __func__, version);
     if (privkey.empty())
        return error("%s: empty privkey %s", __func__, privkey.data());
     return key.SetPrivKey(privkey, true);
}
 
std::string CZerocoinMint::ToString() const
{
    std::string str = strprintf("version=%d randomness: %s \n serial %s \n privkey %s\n", version, randomness.GetHex(), serialNumber.GetHex(), HexStr(privkey));
    return str;
}

void CZerocoinSpendReceipt::AddSpend(const CZerocoinSpend& spend)
{
    vSpends.emplace_back(spend);
}

std::vector<CZerocoinSpend> CZerocoinSpendReceipt::GetSpends()
{
    return vSpends;
}

void CZerocoinSpendReceipt::SetStatus(std::string strStatus, int nStatus, int nNeededSpends)
{
    strStatusMessage = strStatus;
    this->nStatus = nStatus;
    this->nNeededSpends = nNeededSpends;
}

std::string CZerocoinSpendReceipt::GetStatusMessage()
{
    return strStatusMessage;
}

int CZerocoinSpendReceipt::GetStatus()
{
    return nStatus;
}

int CZerocoinSpendReceipt::GetNeededSpends()
{
    return nNeededSpends;
}
