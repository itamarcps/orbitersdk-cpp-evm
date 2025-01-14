#ifndef EC_RECOVER_PRECOMPILE_HPP
#define EC_RECOVER_PRECOMPILE_HPP

#include <evmc/evmc.hpp>
#include <secp256k1.h>

#include "utils/ecdsa.h"
#include "../contract/abi.h"
#include "../utils/randomgen.h"

using namespace evmc::literals;
const auto ECRECOVER_ADDRESS = 0x1000000000000000000000000000100000000001_address;
const auto ABI_PACK = 0x1000000000000000000000000000100000000002_address;
const auto RANDOM = 0x1000000000000000000000000000100000000003_address;

namespace Precompile {
  evmc::Result ecrecover(const evmc_message& msg, std::vector<std::array<uint8_t, 32>>& addrs) noexcept;
  evmc::Result packAndHash(const evmc_message& msg, std::vector<std::array<uint8_t, 32>>& hashs) noexcept;
  evmc::Result keccakSolSign(const evmc_message& msg, std::vector<std::array<uint8_t, 32>>& hashs) noexcept;
  evmc::Result keccak(const evmc_message& msg, std::vector<std::array<uint8_t, 32>>& hashs) noexcept;
  evmc::Result getRandom(const evmc_message& msg, std::vector<std::array<uint8_t, 32>>& hashs, RandomGen* random) noexcept;
}




#endif // EC_RECOVER_PRECOMPILE_HPP