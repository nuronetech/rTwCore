/***********************************************************************
**************Copyright (c) 2009-2010 Satoshi Nakamoto******************
***********Copyright (c) 2009-2012 The Bitcoin developers***************
**********Copyright (c) 2014-2015 The Dash Core developers**************
*************Copyright (c) 2015-2017 The PIVX developers****************
******************Copyright (c) 2010-2019 Nur1Labs**********************
>Distributed under the MIT/X11 software license, see the accompanying
>file COPYING or http://www.opensource.org/licenses/mit-license.php.
************************************************************************/

#include "libzerocoin/Params.h"
#include "chainparams.h"
#include "random.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

using namespace std;
using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

/**
 * Main network
 */

//! Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress>& vSeedsOut, const SeedSpec6* data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7 * 24 * 60 * 60;
    for (unsigned int i = 0; i < count; i++) {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

//   What makes a good checkpoint block?
// + Is surrounded by blocks with reasonable timestamps
//   (no blocks before with a timestamp after, none after with
//    timestamp before)
// + Contains no strange transactions
static Checkpoints::MapCheckpoints mapCheckpoints =
    boost::assign::map_list_of
    (0, uint256("0x838264c8705b4d54ea13ad5d032e3bf95102dffc9be73a1cc65ac56ae9d387d9")); //start block
static const Checkpoints::CCheckpointData data = {
    &mapCheckpoints,
    0, // * UNIX timestamp of last checkpoint block
    0,    // * total number of transactions between genesis and last checkpoint
                //   (the tx=... number in the SetBestChain debug.log lines)
    0        // * estimated number of transactions per day after checkpoint
};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
    boost::assign::map_list_of(0, uint256("0x001"));
static const Checkpoints::CCheckpointData dataTestnet = {
    &mapCheckpointsTestnet,
    0,
    0,
    0};

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
    boost::assign::map_list_of(0, uint256("0x001"));
static const Checkpoints::CCheckpointData dataRegtest = {
    &mapCheckpointsRegtest,
    0,
    0,
    0};

libzerocoin::ZerocoinParams* CChainParams::Zerocoin_Params() const
{
    assert(this);
    static CBigNum bnTrustedModulus(zerocoinModulus);
    static libzerocoin::ZerocoinParams ZCParams = libzerocoin::ZerocoinParams(bnTrustedModulus);

    return &ZCParams;
}

class CMainParams : public CChainParams
{
public:
    CMainParams()
    {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";
        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0x7c;
        pchMessageStart[1] = 0x70;
        pchMessageStart[2] = 0x0a;
        pchMessageStart[3] = 0x72;
        vAlertPubKey = ParseHex("045ecefd3b1518badf895a35783acb866cc2b105f1febaedf35bfec91c773da1e5f63a4a0cbdd7dad6d1dab924fb4261823244a8b6678fa67a825222246d3d77e6");
        nDefaultPort = 45001;
        bnProofOfWorkLimit = ~uint256(0) >> 20; //20 MuBdI starting difficulty is 1 / 2^12
        nSubsidyHalvingInterval = 210000;
        nMaxReorganizationDepth = 100;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 0;
        nTargetTimespan = 1 * 30; // MuBdI: 0.5 minutes
        nTargetSpacingSlowLaunch = 10 * 60;  // MuBdI: 10 minutes (Slow launch - Block 288)
        nTargetSpacing = 1 * 60;  // MuBdI: 60 Seconds
        nMaturity = 30;
        nMasternodeCountDrift = 20;
        nMaxMoneyOut = 10000000000 * COIN;

        /** Height or Time Based Activations **/
        nLastPOWBlock = 500000;
        nModifierUpdateBlock = 999999999;
        nZerocoinStartHeight = 999999999;
        nAccumulatorStartHeight = 1;
        nZerocoinStartTime = 1527974340; // Disable
        nBlockEnforceSerialRange = 1; //Enforce serial range starting this block
        nBlockRecalculateAccumulators = ~1; //Trigger a recalculation of accumulators
        nBlockFirstFraudulent = ~1; //First block that bad serials emerged
        nBlockLastGoodCheckpoint = ~1; //Last valid accumulator checkpoint
        nBlockEnforceInvalidUTXO = ~1; //Start enforcing the invalid UTXO's
        nBlockZerocoinV2 = 99999999; //The block that zerocoin v2 becomes active

        /**
         * Build the genesis block. Note that the output of the genesis coinbase cannot
         * be spent as it did not originally exist in the database.
         *
            genesis.nTime = 1527974340
            genesis.nNonce = 236551
            genesis.nVersion = 1
            genesis.GetHash = 000003f46e57c04e31374710022a6685abea12cb99960dfe96eaaa430edc670f
            genesis.hashMerkleRoot = 90d274dca16659997d8f5fc9644127d260087d4a39ca61ba9b3334482aa8c55d

         */
        const char* pszTimestamp = "2/15/2019 - MuBdI replace AriA Platform for zEtA";
        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 1 * COIN;
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("047c0842fb9c109adc8a83747d294acd796451d53b1b8239ab8090ed721d5594695c51c2f2f64994ce9f8316479b72b742909cf888f2315c4356c4a049cfbbdf83") << OP_CHECKSIG;
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime = 1562351048;
        genesis.nBits = 0x1e0ffff0;
        genesis.nNonce = 550989;
 
	    hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0x838264c8705b4d54ea13ad5d032e3bf95102dffc9be73a1cc65ac56ae9d387d9"));
        assert(genesis.hashMerkleRoot == uint256("0x7943519a963b76bdf183107e12d115106cc01c6b785c4b99008cd9d90d0e8b9c"));

    	vSeeds.push_back(CDNSSeedData("188.166.107.64", "188.166.107.64"));             // central_nodes
		vSeeds.push_back(CDNSSeedData("188.166.121.99", "188.166.121.99"));             // seed1
		//vSeeds.push_back(CDNSSeedData("142.93.246.46", "142.93.246.46"));             // seed1

        //vFixedSeeds.clear();
        //vSeeds.clear();

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 122); // r
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 66); // T
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 135); // w
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0xb5)(0x49)(0x70)(0xbc).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0xd9)(0x06)(0x18)(0x5f).convert_to_container<std::vector<unsigned char> >();
        // 	BIP used MuBdI Consensus
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0xbb)(0x45)(0xa1)(0x82).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fSkipProofOfWorkCheck = false;
        fTestnetToBeDeprecatedFieldRPC = false;
        fHeadersFirstSyncingActive = false;

        nPoolMaxTransactions = 3;
        //strSporkKey = "0459eede7626441f7802af2736cb3a4aeb3e1f95070cde39d068a4f16525ee8fdd3c075f29f9e115aeb91952239194aa6ac19765574fed8a0d7f174f2b450e9630";
	    strSporkKey = "041dc87cc1e89ee19a8d1e45ee7140eaf82024bd3d184daac0a285b83cb1bd79fca8b7416998bb22ca92f92ef74741834117aa0c4c4974196f9c718e4a870daf5b";
        strObfuscationPoolDummyAddress = "aW3y7JyvbVXTv4sdMD1ZrowsBKU8oHc7xn";
        nStartMasternodePayments = 1550403527 + 6000; //Wed, 17 Feb 2019 Tokyo Time

        /** Zerocoin */
        zerocoinModulus = "0xc95577b6dce0049b0a20c779af38079355abadde1a1d80c353f6cb697a7ae5a087bad39caa5798478551d0f9d91e6267716506f32412de1d19d17588765eb9502b85c6a18abdb05791cfd8b734e960281193705eeece210920cc922b3af3ceb178bf12c22eb565d5767fbf19545639be8953c2c38ffad41f3371e4aac750ac2d7bd614b3faabb453081d5d88fdbb803657a980bc93707e4b14233a2358c97763bf28f7c933206071477e8b371f229bc9ce7d6ef0ed7163aa5dfe13bc15f7816348b328fa2c1e69d5c88f7b94cee7829d56d1842d77d7bb8692e9fc7b7db059836500de8d57eb43c345feb58671503b932829112941367996b03871300f25efb5";
        nMaxZerocoinSpendsPerTransaction = 7; // Assume about 20kb each
        nMinZerocoinMintFee = 1 * ZCENT; //high fee required for zerocoin mints
        nMintRequiredConfirmations = 20; //the maximum amount of confirmations until accumulated in 19
        nRequiredAccumulation = 1;
        nDefaultSecurityLevel = 100; //full security level for accumulators
        nZerocoinHeaderVersion = 4; //Block headers must be this version once zerocoin is active
	nZerocoinRequiredStakeDepth = 300; //The required confirmations for a zar to be stakable

        nBudget_Fee_Confirmations = 6; // Number of confirmations for the finalization fee
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return data;
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams
{
public:
    CTestNetParams()
    {
        networkID = CBaseChainParams::TESTNET;
        strNetworkID = "test";
        pchMessageStart[0] = 0xc4;
        pchMessageStart[1] = 0x5e;
        pchMessageStart[2] = 0xb2;
        pchMessageStart[3] = 0x10;
        vAlertPubKey = ParseHex("04931a51d6b01a86b902f7436c5df534b1a46097b0bde3e74b219c567084437ba23573a273f2581cc4d1b5d3820ece44f5c2a1575b37440435e681090fbd223614");
        nDefaultPort = 43000;
        nEnforceBlockUpgradeMajority = 51;
        nRejectBlockOutdatedMajority = 75;
        nToCheckBlockUpgradeMajority = 100;
        nMinerThreads = 0;
        nTargetTimespan = 1 * 60; // MuBdI: 1 day
        nTargetSpacing = 1 * 60;  // MuBdI: 1 minute
        nLastPOWBlock = 200;
        nMaturity = 15;
        nMasternodeCountDrift = 4;
        nModifierUpdateBlock = 51197; //approx Mon, 17 Apr 2017 04:00:00 GMT
        nMaxMoneyOut = 43199500 * COIN;
        nZerocoinStartHeight = 201576;
        nZerocoinStartTime = 1524711188;
        nBlockEnforceSerialRange = 1; //Enforce serial range starting this block
        nBlockRecalculateAccumulators = 9908000; //Trigger a recalculation of accumulators
        nBlockFirstFraudulent = 9891737; //First block that bad serials emerged
        nBlockLastGoodCheckpoint = 9891730; //Last valid accumulator checkpoint
        nBlockEnforceInvalidUTXO = 902850; //Start enforcing the invalid UTXO's
	nBlockZerocoinV2 = 99999999; //The block that zerocoin v2 becomes active

        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1535038683;
        genesis.nNonce = 79855;

	hashGenesisBlock = genesis.GetHash();
        //assert(hashGenesisBlock == uint256("0xad8dd144b354813febbf81b0c4878a5c45e8eec53eb4ebdd83ffba894410164d"));

        //vFixedSeeds.clear();
        //vSeeds.clear();
        //vSeeds.push_back(CDNSSeedData("157.230.85.46", "157.230.85.46"));         // Testnet will online


        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 23); // Testnet MuBdI addresses start with 'a'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 61);  // Testnet MuBdI script addresses start with 'r'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 41);     // Testnet private keys start with 'h' (MuBdI defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x9e)(0xbb)(0x20)(0xdd).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x29)(0x65)(0xbc)(0x9d).convert_to_container<std::vector<unsigned char> >();
        // Testnet used MuBdI Consensus
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x07)(0xcf)(0xcc)(0xa4).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        nPoolMaxTransactions = 2;
        strSporkKey = "046baf149c4bf5595d318a6c8b6aefb6383699d88949a89219659d83c9fed4edb9e8813b9d928178979ab652c8d543380e1b55cc1438ab54809f95346301101aea";
        strObfuscationPoolDummyAddress = "xp87cG8UEQgzs1Bk67Yk884C7pnQfAeo7q";
        nStartMasternodePayments = 1420837558; //Fri, 09 Jan 2015 21:05:58 GMT
        nBudget_Fee_Confirmations = 3; // Number of confirmations for the finalization fee. We have to make this very short
                                       // here because we only have a 8 block finalization window on testnet
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataTestnet;
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams
{
public:
    CRegTestParams()
    {
        networkID = CBaseChainParams::REGTEST;
        strNetworkID = "regtest";
        strNetworkID = "regtest";
        pchMessageStart[0] = 0x4b;
        pchMessageStart[1] = 0x9b;
        pchMessageStart[2] = 0x13;
        pchMessageStart[3] = 0x72;
        nSubsidyHalvingInterval = 150;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 1;
        nTargetTimespan = 24 * 60 * 60; // MuBdI: 1 day
        nTargetSpacing = 1 * 60;        // MuBdI: 1 minutes
        bnProofOfWorkLimit = ~uint256(0) >> 1;
        genesis.nTime = 1535040987;
        genesis.nBits = 0x1e0ffff0;
        genesis.nNonce = 232084;

        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 53000;
        //assert(hashGenesisBlock == uint256("0x000008415bdca132b70cf161ecc548e5d0150fd6634a381ee2e99bb8bb77dbb3"));

        vFixedSeeds.clear(); //! Testnet mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Testnet mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataRegtest;
    }
};
static CRegTestParams regTestParams;

/**
 * Unit test
 */
class CUnitTestParams : public CMainParams, public CModifiableParams
{
public:
    CUnitTestParams()
    {
        networkID = CBaseChainParams::UNITTEST;
        strNetworkID = "unittest";
        nDefaultPort = 23000;
        vFixedSeeds.clear(); //! Unit test mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Unit test mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fAllowMinDifficultyBlocks = false;
        fMineBlocksOnDemand = true;
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        // UnitTest share the same checkpoints as MAIN
        return data;
    }

    //! Published setters to allow changing values in unit test cases
    virtual void setSubsidyHalvingInterval(int anSubsidyHalvingInterval) { nSubsidyHalvingInterval = anSubsidyHalvingInterval; }
    virtual void setEnforceBlockUpgradeMajority(int anEnforceBlockUpgradeMajority) { nEnforceBlockUpgradeMajority = anEnforceBlockUpgradeMajority; }
    virtual void setRejectBlockOutdatedMajority(int anRejectBlockOutdatedMajority) { nRejectBlockOutdatedMajority = anRejectBlockOutdatedMajority; }
    virtual void setToCheckBlockUpgradeMajority(int anToCheckBlockUpgradeMajority) { nToCheckBlockUpgradeMajority = anToCheckBlockUpgradeMajority; }
    virtual void setDefaultConsistencyChecks(bool afDefaultConsistencyChecks) { fDefaultConsistencyChecks = afDefaultConsistencyChecks; }
    virtual void setAllowMinDifficultyBlocks(bool afAllowMinDifficultyBlocks) { fAllowMinDifficultyBlocks = afAllowMinDifficultyBlocks; }
    virtual void setSkipProofOfWorkCheck(bool afSkipProofOfWorkCheck) { fSkipProofOfWorkCheck = afSkipProofOfWorkCheck; }
};

static CChainParams* pCurrentParams = 0;

const CChainParams& Params()
{
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(CBaseChainParams::Network network)
{
    switch (network) {
    case CBaseChainParams::MAIN:
        return mainParams;
    case CBaseChainParams::TESTNET:
        return testNetParams;
    case CBaseChainParams::REGTEST:
        return regTestParams;
    default:
        assert(false && "Unimplemented network");
        return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}
