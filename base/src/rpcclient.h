/***********************************************************************
**************Copyright (c) 2009-2010 Satoshi Nakamoto******************
***********Copyright (c) 2009-2012 The Bitcoin developers***************
******************Copyright (c) 2010-2019 Nur1Labs**********************
>Distributed under the MIT/X11 software license, see the accompanying
>file COPYING or http://www.opensource.org/licenses/mit-license.php.
************************************************************************/

#ifndef MUBDI_RPCCLIENT_H
#define MUBDI_RPCCLIENT_H

#include <univalue.h>

UniValue RPCConvertValues(const std::string& strMethod, const std::vector<std::string>& strParams);
/** Non-RFC4627 JSON parser, accepts internal values (such as numbers, true, false, null)
 * as well as objects and arrays.
 */
UniValue ParseNonRFCJSONValue(const std::string& strVal);

#endif /* MUBDI_RPCCLIENT_H */