# Copyright (c) 2010-2019 Nur1Labs
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

DIST_SUBDIRS = secp256k1 univalue

AM_LDFLAGS = $(PTHREAD_CFLAGS) $(LIBTOOL_LDFLAGS) $(HARDENED_LDFLAGS)
AM_CXXFLAGS = $(HARDENED_CXXFLAGS) $(ERROR_CXXFLAGS)
AM_CPPFLAGS = $(HARDENED_CPPFLAGS)
EXTRA_LIBRARIES =

if EMBEDDED_UNIVALUE
LIBUNIVALUE = univalue/libunivalue.la

$(LIBUNIVALUE): $(wildcard univalue/lib/*) $(wildcard univalue/include/*)
	$(AM_V_at)$(MAKE) $(AM_MAKEFLAGS) -C $(@D) $(@F)
else
LIBUNIVALUE = $(UNIVALUE_LIBS)
endif

if EMBEDDED_LEVELDB
LEVELDB_CPPFLAGS += -I$(srcdir)/leveldb/include
LEVELDB_CPPFLAGS += -I$(srcdir)/leveldb/helpers/memenv
LIBLEVELDB += $(builddir)/leveldb/libleveldb.a
LIBMEMENV  += $(builddir)/leveldb/libmemenv.a

# NOTE: This dependency is not strictly necessary, but without it make may try to build both in parallel, which breaks the LevelDB build system in a race
$(LIBLEVELDB): $(LIBMEMENV)

$(LIBLEVELDB) $(LIBMEMENV):
	@echo "Building LevelDB ..." && $(MAKE) -C $(@D) $(@F) CXX="$(CXX)" \
	  CC="$(CC)" PLATFORM=$(TARGET_OS) AR="$(AR)" $(LEVELDB_TARGET_FLAGS) \
          OPT="$(AM_CXXFLAGS) $(PIE_FLAGS) $(CXXFLAGS) $(AM_CPPFLAGS) $(CPPFLAGS) -D__STDC_LIMIT_MACROS"
endif

MUBDI_CONFIG_INCLUDES=-I$(builddir)/config
MUBDI_INCLUDES=-I$(builddir) -I$(builddir)/obj $(BOOST_CPPFLAGS) $(LEVELDB_CPPFLAGS) $(CRYPTO_CFLAGS) $(SSL_CFLAGS)

MUBDI_INCLUDES += -I$(srcdir)/secp256k1/include
MUBDI_INCLUDES += $(UNIVALUE_CFLAGS)

LIBMUBDI_SERVER=libmubdi_server.a
LIBMUBDI_COMMON=libmubdi_common.a
LIBMUBDI_CLI=libmubdi_cli.a
LIBMUBDI_UTIL=libmubdi_util.a
LIBMUBDI_CRYPTO=crypto/libmubdi_crypto.a
LIBMUBDI_ZEROCOIN=libzerocoin/libmubdi_zerocoin.a
LIBMUBDIQT=qt/libmubdiqt.a
LIBSECP256K1=secp256k1/libsecp256k1.la

if ENABLE_ZMQ
LIBMUBDI_ZMQ=libmubdi_zmq.a
endif
if BUILD_MUBDI_LIBS
LIBMUBDICONSENSUS=libmubdiconsensus.la
endif
if ENABLE_WALLET
LIBMUBDI_WALLET=libmubdi_wallet.a
endif

$(LIBSECP256K1): $(wildcard secp256k1/src/*) $(wildcard secp256k1/include/*)
	$(AM_V_at)$(MAKE) $(AM_MAKEFLAGS) -C $(@D) $(@F)

# Make is not made aware of per-object dependencies to avoid limiting building parallelization
# But to build the less dependent modules first, we manually select their order here:
EXTRA_LIBRARIES += \
  $(LIBMUBDI_CRYPTO) \
  $(LIBMUBDI_UTIL) \
  $(LIBMUBDI_COMMON) \
  $(LIBMUBDI_ZEROCOIN) \
  $(LIBMUBDI_SERVER) \
  $(LIBMUBDI_CLI) \
  $(LIBMUBDI_WALLET) \
  $(LIBMUBDI_ZMQ)

lib_LTLIBRARIES = $(LIBMUBDICONSENSUS)

bin_PROGRAMS =
noinst_PROGRAMS =
TESTS =

if BUILD_MUBDID
  bin_PROGRAMS += rtwd
endif

if BUILD_MUBDI_UTILS
  bin_PROGRAMS += rtw-cli mubdi-tx
endif

.PHONY: FORCE
# mubdi core #
MUBDI_CORE_H = \
  activemasternode.h \
  accumulators.h \
  accumulatormap.h \
  addrman.h \
  alert.h \
  allocators.h \
  amount.h \
  base58.h \
  bip38.h \
  bloom.h \
  blocksignature.h \
  chain.h \
  chainparams.h \
  chainparamsbase.h \
  chainparamsseeds.h \
  checkpoints.h \
  checkqueue.h \
  clientversion.h \
  coincontrol.h \
  coins.h \
  compat.h \
  compat/sanity.h \
  compressor.h \
  primitives/block.h \
  primitives/transaction.h \
  primitives/zerocoin.h \
  core_io.h \
  crypter.h \
  denomination_functions.h \
  obfuscation.h \
  obfuscation-relay.h \
  db.h \
  eccryptoverify.h \
  ecwrapper.h \
  hash.h \
  httprpc.h \
  httpserver.h \
  init.h \
  kernel.h \
  swifttx.h \
  key.h \
  keystore.h \
  leveldbwrapper.h \
  limitedmap.h \
  main.h \
  masternode.h \
  masternode-payments.h \
  masternode-budget.h \
  masternode-sync.h \
  masternodeman.h \
  masternodeconfig.h \
  merkleblock.h \
  miner.h \
  mruset.h \
  netbase.h \
  net.h \
  noui.h \
  pow.h \
  protocol.h \
  pubkey.h \
  random.h \
  reverselock.h \
  reverse_iterate.h \
  rpcclient.h \
  rpcprotocol.h \
  rpcserver.h \
  scheduler.h \
  script/interpreter.h \
  script/script.h \
  script/sigcache.h \
  script/sign.h \
  script/standard.h \
  script/script_error.h \
  serialize.h \
  spork.h \
  sporkdb.h \
  stakeinput.h \
  streams.h \
  sync.h \
  threadsafety.h \
  timedata.h \
  tinyformat.h \
  torcontrol.h \
  txdb.h \
  txmempool.h \
  ui_interface.h \
  uint256.h \
  undo.h \
  util.h \
  utilstrencodings.h \
  utilmoneystr.h \
  utiltime.h \
  validationinterface.h \
  version.h \
  wallet.h \
  wallet_ismine.h \
  walletdb.h \
  zmq/zmqabstractnotifier.h \
  zmq/zmqconfig.h \
  zmq/zmqnotificationinterface.h \
  zmq/zmqpublishnotifier.h

obj/build.h: FORCE
	@$(MKDIR_P) $(builddir)/obj
	@$(top_srcdir)/share/genbuild.sh "$(abs_top_builddir)/src/obj/build.h" \
	  "$(abs_top_srcdir)"
libmubdi_util_a-clientversion.$(OBJEXT): obj/build.h

# server: shared between mubdid and mubdi-qt
libmubdi_server_a_CPPFLAGS = $(AM_CPPFLAGS) $(MUBDI_INCLUDES) $(MINIUPNPC_CPPFLAGS) $(EVENT_CFLAGS) $(EVENT_PTHREADS_CFLAGS)
libmubdi_server_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libmubdi_server_a_SOURCES = \
  addrman.cpp \
  alert.cpp \
  bloom.cpp \
  blocksignature.cpp \
  chain.cpp \
  checkpoints.cpp \
  httprpc.cpp \
  httpserver.cpp \
  init.cpp \
  leveldbwrapper.cpp \
  main.cpp \
  merkleblock.cpp \
  miner.cpp \
  net.cpp \
  noui.cpp \
  pow.cpp \
  rest.cpp \
  rpcblockchain.cpp \
  rpcmasternode.cpp \
  rpcmasternode-budget.cpp \
  rpcmining.cpp \
  rpcmisc.cpp \
  rpcnet.cpp \
  rpcrawtransaction.cpp \
  rpcserver.cpp \
  script/sigcache.cpp \
  sporkdb.cpp \
  timedata.cpp \
  torcontrol.cpp \
  txdb.cpp \
  txmempool.cpp \
  validationinterface.cpp \
  $(MUBDI_CORE_H)

if ENABLE_ZMQ
libmubdi_zmq_a_CPPFLAGS = $(MUBDI_INCLUDES) $(ZMQ_CFLAGS)
libmubdi_zmq_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libmubdi_zmq_a_SOURCES = \
  zmq/zmqabstractnotifier.cpp \
  zmq/zmqnotificationinterface.cpp \
  zmq/zmqpublishnotifier.cpp
endif

# wallet: shared between mubdid and mubdi-qt, but only linked
# when wallet enabled
libmubdi_wallet_a_CPPFLAGS = $(AM_CPPFLAGS) $(MUBDI_INCLUDES)
libmubdi_wallet_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libmubdi_wallet_a_SOURCES = \
  activemasternode.cpp \
  bip38.cpp \
  denomination_functions.cpp \
  obfuscation.cpp \
  obfuscation-relay.cpp \
  db.cpp \
  crypter.cpp \
  swifttx.cpp \
  masternode.cpp \
  masternode-budget.cpp \
  masternode-payments.cpp \
  masternode-sync.cpp \
  masternodeconfig.cpp \
  masternodeman.cpp \
  rpcdump.cpp \
  primitives/zerocoin.cpp \
  rpcwallet.cpp \
  kernel.cpp \
  wallet.cpp \
  wallet_ismine.cpp \
  walletdb.cpp \
  stakeinput.cpp \
  $(MUBDI_CORE_H)

# crypto primitives library
crypto_libmubdi_crypto_a_CPPFLAGS = $(AM_CPPFLAGS)
crypto_libmubdi_crypto_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
crypto_libmubdi_crypto_a_SOURCES = \
  crypto/sha1.cpp \
  crypto/sha256.cpp \
  crypto/sha512.cpp \
  crypto/hmac_sha256.cpp \
  crypto/rfc6979_hmac_sha256.cpp \
  crypto/hmac_sha512.cpp \
  crypto/scrypt.cpp \
  crypto/ripemd160.cpp \
  crypto/sph_md_helper.c \
  crypto/sph_sha2big.c \
  crypto/aes_helper.c \
  crypto/blake.c \
  crypto/bmw.c \
  crypto/groestl.c \
  crypto/jh.c \
  crypto/luffa.c \
  crypto/keccak.c \
  crypto/skein.c \
  crypto/cubehash.c \
  crypto/shavite.c \
  crypto/shabal.c \
  crypto/simd.c \
  crypto/echo.c \
  crypto/whirlpool.c \
  crypto/haval.c \
  crypto/hamsi.c \
  crypto/fugue.c \
  crypto/sha2.c \
  crypto/common.h \
  crypto/sha256.h \
  crypto/sha512.h \
  crypto/hmac_sha256.h \
  crypto/rfc6979_hmac_sha256.h \
  crypto/hmac_sha512.h \
  crypto/scrypt.h \
  crypto/sha1.h \
  crypto/ripemd160.h \
  crypto/sph_blake.h \
  crypto/sph_bmw.h \
  crypto/sph_groestl.h \
  crypto/sph_jh.h \
  crypto/sph_keccak.h \
  crypto/sph_luffa.h \
  crypto/sph_shavite.h \
  crypto/sph_hamsi.h \
  crypto/sph_cubehash.h \
  crypto/sph_skein.h \
  crypto/sph_simd.h \
  crypto/sph_fugue.h \
  crypto/sph_echo.h \
  crypto/sph_shabal.h \
  crypto/sph_whirlpool.h \
  crypto/sph_sha2.h \
  crypto/sph_haval.h \
  crypto/sph_types.h

# libzerocoin library
libzerocoin_libmubdi_zerocin_a_CPPFLAGS = $(AM_CPPFLAGS)
libzerocoin_libmubdi_zerocin_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libzerocoin_libmubdi_zerocoin_a_SOURCES = \
  libzerocoin/Accumulator.h \
  libzerocoin/AccumulatorProofOfKnowledge.h \
  libzerocoin/bignum.h \
  libzerocoin/Coin.h \
  libzerocoin/CoinSpend.h \
  libzerocoin/Commitment.h \
  libzerocoin/Denominations.h \
  libzerocoin/ParamGeneration.h \
  libzerocoin/Params.h \
  libzerocoin/SerialNumberSignatureOfKnowledge.h \
  libzerocoin/ZerocoinDefines.h \
  libzerocoin/Accumulator.cpp \
  libzerocoin/AccumulatorProofOfKnowledge.cpp \
  libzerocoin/Coin.cpp \
  libzerocoin/Denominations.cpp \
  libzerocoin/CoinSpend.cpp \
  libzerocoin/Commitment.cpp \
  libzerocoin/ParamGeneration.cpp \
  libzerocoin/Params.cpp \
  libzerocoin/SerialNumberSignatureOfKnowledge.cpp

# common: shared between mubdid, and mubdi-qt and non-server tools
libmubdi_common_a_CPPFLAGS = $(AM_CPPFLAGS) $(MUBDI_INCLUDES)
libmubdi_common_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libmubdi_common_a_SOURCES = \
  accumulators.cpp \
  accumulatormap.cpp \
  allocators.cpp \
  amount.cpp \
  base58.cpp \
  bip38.cpp \
  chainparams.cpp \
  coins.cpp \
  compressor.cpp \
  primitives/block.cpp \
  primitives/transaction.cpp \
  primitives/zerocoin.cpp \
  core_read.cpp \
  core_write.cpp \
  eccryptoverify.cpp \
  ecwrapper.cpp \
  hash.cpp \
  key.cpp \
  keystore.cpp \
  netbase.cpp \
  protocol.cpp \
  pubkey.cpp \
  scheduler.cpp \
  script/interpreter.cpp \
  script/script.cpp \
  script/sign.cpp \
  script/standard.cpp \
  script/script_error.cpp \
  spork.cpp \
  sporkdb.cpp \
  $(MUBDI_CORE_H)

# util: shared between all executables.
# This library *must* be included to make sure that the glibc
# backward-compatibility objects and their sanity checks are linked.
libmubdi_util_a_CPPFLAGS = $(AM_CPPFLAGS) $(MUBDI_INCLUDES)
libmubdi_util_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libmubdi_util_a_SOURCES = \
  allocators.cpp \
  compat/strnlen.cpp \
  compat/glibc_sanity.cpp \
  compat/glibcxx_sanity.cpp \
  chainparamsbase.cpp \
  clientversion.cpp \
  random.cpp \
  rpcprotocol.cpp \
  sync.cpp \
  uint256.cpp \
  util.cpp \
  utilstrencodings.cpp \
  utilmoneystr.cpp \
  utiltime.cpp \
  $(MUBDI_CORE_H)

if GLIBC_BACK_COMPAT
libmubdi_util_a_SOURCES += compat/glibc_compat.cpp
endif

# cli: shared between mubdi-cli and mubdi-qt
libmubdi_cli_a_CPPFLAGS = $(AM_CPPFLAGS) $(MUBDI_INCLUDES)
libmubdi_cli_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libmubdi_cli_a_SOURCES = \
  rpcclient.cpp \
  $(MUBDI_CORE_H)

nodist_libmubdi_util_a_SOURCES = $(srcdir)/obj/build.h
#

# mubdid binary #
rtwd_SOURCES = mubdid.cpp
rtwd_CPPFLAGS = $(AM_CPPFLAGS) $(MUBDI_INCLUDES)
rtwd_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
rtwd_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if TARGET_WINDOWS
rtwd_SOURCES += mubdid-res.rc
endif
rtwd_LDADD = \
  $(LIBMUBDI_SERVER) \
  $(LIBMUBDI_COMMON) \
  $(LIBUNIVALUE) \
  $(LIBMUBDI_ZEROCOIN) \
  $(LIBMUBDI_UTIL) \
  $(LIBMUBDI_WALLET) \
  $(LIBMUBDI_ZMQ) \
  $(LIBMUBDI_CRYPTO) \
  $(LIBLEVELDB) \
  $(LIBMEMENV) \
  $(LIBSECP256K1)

rtwd_LDADD += $(BOOST_LIBS) $(BDB_LIBS) $(SSL_LIBS) $(CRYPTO_LIBS) $(MINIUPNPC_LIBS) $(EVENT_PTHREADS_LIBS) $(EVENT_LIBS) $(ZMQ_LIBS)

# mubdi-cli binary #
rtw_cli_SOURCES = mubdi-cli.cpp
rtw_cli_CPPFLAGS = $(AM_CPPFLAGS) $(MUBDI_INCLUDES) $(EVENT_CFLAGS)
rtw_cli_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
rtw_cli_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if TARGET_WINDOWS
rtw_cli_SOURCES += mubdi-cli-res.rc
endif

rtw_cli_LDADD = \
  $(LIBMUBDI_CLI) \
  $(LIBUNIVALUE) \
  $(LIBMUBDI_UTIL) \
  $(LIBMUBDI_CRYPTO)

rtw_cli_LDADD += $(BOOST_LIBS) $(SSL_LIBS) $(CRYPTO_LIBS) $(EVENT_LIBS)
#

# mubdi-tx binary #
mubdi_tx_SOURCES = mubdi-tx.cpp
mubdi_tx_CPPFLAGS = $(AM_CPPFLAGS) $(MUBDI_INCLUDES)
mubdi_tx_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
mubdi_tx_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if TARGET_WINDOWS
mubdi_tx_SOURCES += mubdi-tx-res.rc
endif

mubdi_tx_LDADD = \
  $(LIBUNIVALUE) \
  $(LIBMUBDI_COMMON) \
  $(LIBMUBDI_ZEROCOIN) \
  $(LIBMUBDI_UTIL) \
  $(LIBMUBDI_CRYPTO) \
  $(LIBSECP256K1)

mubdi_tx_LDADD += $(BOOST_LIBS) $(CRYPTO_LIBS)
#

# mubdiconsensus library #
if BUILD_MUBDI_LIBS
include_HEADERS = script/mubdiconsensus.h
libmubdiconsensus_la_SOURCES = \
  allocators.cpp \
  primitives/transaction.cpp \
  crypto/hmac_sha512.cpp \
  crypto/scrypt.cpp \
  crypto/sha1.cpp \
  crypto/sha256.cpp \
  crypto/sha512.cpp \
  crypto/ripemd160.cpp \
  eccryptoverify.cpp \
  ecwrapper.cpp \
  hash.cpp \
  pubkey.cpp \
  script/script.cpp \
  script/interpreter.cpp \
  script/mubdiconsensus.cpp \
  uint256.cpp \
  utilstrencodings.cpp

if GLIBC_BACK_COMPAT
  libmubdiconsensus_la_SOURCES += compat/glibc_compat.cpp
endif

libmubdiconsensus_la_LDFLAGS = $(AM_LDFLAGS) -no-undefined $(RELDFLAGS)
libmubdiconsensus_la_LIBADD = $(CRYPTO_LIBS) $(BOOST_LIBS)
libmubdiconsensus_la_CPPFLAGS = $(CRYPTO_CFLAGS) -I$(builddir)/obj -DBUILD_MUBDI_INTERNAL
if USE_LIBSECP256K1
libmubdiconsensus_la_LIBADD += secp256k1/libsecp256k1.la
endif
endif

CLEANFILES = $(EXTRA_LIBRARIES)
CLEANFILES += leveldb/libleveldb.a leveldb/libmemenv.a
CLEANFILES += *.gcda *.gcno
CLEANFILES += compat/*.gcda compat/*.gcno
CLEANFILES += crypto/*.gcda crypto/*.gcno
CLEANFILES += primitives/*.gcda primitives/*.gcno
CLEANFILES += script/*.gcda script/*.gcno
CLEANFILES += univalue/*.gcda univalue/*.gcno
CLEANFILES += zmq/*.gcda zmq/*.gcno
CLEANFILES += obj/build.h


EXTRA_DIST = leveldb

clean-local:
	-$(MAKE) -C leveldb clean
	-$(MAKE) -C secp256k1 clean
	-$(MAKE) -C univalue clean
	rm -f leveldb/*/*.gcno leveldb/helpers/memenv/*.gcno
	-rm -f config.h

.rc.o:
	@test -f $(WINDRES)
	## FIXME: How to get the appropriate modulename_CPPFLAGS in here?
	$(AM_V_GEN) $(WINDRES) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(CPPFLAGS) -DWINDRES_PREPROC -i $< -o $@


%.pb.cc %.pb.h: %.proto
	@test -f $(PROTOC)
	$(AM_V_GEN) $(PROTOC) --cpp_out=$(@D) --proto_path=$(<D) $<

if ENABLE_TESTS
include Makefile.test.include
endif

if ENABLE_QT
include Makefile.qt.include
endif

if ENABLE_QT_TESTS
include Makefile.qttest.include
endif
