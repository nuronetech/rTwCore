#!/bin/sh
# Copyright (c) 2013-2019 Nur1Labs
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.
./aria-cli stop
./ariad -daemon -pid=aria.pid
