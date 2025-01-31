/*
Copyright (c) [2023-2024] [Sparq Network]

This software is distributed under the MIT License.
See the LICENSE.txt file in the project root for more information.
*/

#include "templates/erc20.h"
#include "templates/erc20wrapper.h"
#include "templates/nativewrapper.h"
#include "templates/simplecontract.h"
#include "templates/erc721.h"
#include "templates/dexv2/dexv2pair.h"
#include "templates/dexv2/dexv2factory.h"
#include "templates/dexv2/dexv2router02.h"
#include "templates/throwtestA.h"
#include "templates/throwtestB.h"
#include "templates/throwtestC.h"
#include "templates/erc721test.h"
#include "templates/testThrowVars.h"
#include "templates/erc721mint.h"

/// Typedef for the blockchain's registered contracts.
using ContractTypes = std::tuple<
  ERC20, ERC20Wrapper, NativeWrapper, SimpleContract, DEXV2Pair, DEXV2Factory,
  DEXV2Router02, ERC721, ThrowTestA, ThrowTestB, ThrowTestC, ERC721Test, TestThrowVars, ERC721Mint
>;

