/*
Copyright (c) [2023-2024] [Sparq Network]

This software is distributed under the MIT License.
See the LICENSE.txt file in the project root for more information.
*/

#include "../../src/libs/catch2/catch_amalgamated.hpp"
#include "../../src/contract/templates/erc20.h"
#include "../../src/contract/abi.h"
#include "../../src/utils/db.h"
#include "../../src/utils/options.h"
#include "../../src/contract/contractmanager.h"
#include "../../src/core/rdpos.h"

#include "../sdktestsuite.hpp"

#include <filesystem>

// TODO: test events if/when implemented

namespace TERC20 {
  TEST_CASE("EVMOne Class", "[contract][evmone]") {
    SECTION("EVMOne AIO Test") {
      TestAccount toAccount = TestAccount::newRandomAccount();
      uint256_t finalOwnerNativeBal = 0;
      uint256_t finalRecipientNativeBal = 0;
      uint256_t finalOwnerTokenBal = 0;
      uint256_t finalRecipientTokenBal = 0;
      uint256_t finalOwnerNonce = 0;
      uint256_t finalRecipientNonce = 0;
      Hash createTxHash = Hash();
      Address ERC20Address = Address();
      {
        SDKTestSuite sdk = SDKTestSuite::createNewEnvironment("TestEVMOne_ERC20");
        const auto erc20CreateBytes = Hex::toBytes("61016060405234801562000011575f80fd5b506040518060400160405280600781526020017f4d79546f6b656e00000000000000000000000000000000000000000000000000815250806040518060400160405280600181526020017f31000000000000000000000000000000000000000000000000000000000000008152506040518060400160405280600781526020017f4d79546f6b656e000000000000000000000000000000000000000000000000008152506040518060400160405280600381526020017f4d544b00000000000000000000000000000000000000000000000000000000008152508160039081620000fc919062000825565b5080600490816200010e919062000825565b50505062000127600583620001ef60201b90919060201c565b610120818152505062000145600682620001ef60201b90919060201c565b6101408181525050818051906020012060e08181525050808051906020012061010081815250504660a08181525050620001846200024460201b60201c565b608081815250503073ffffffffffffffffffffffffffffffffffffffff1660c08173ffffffffffffffffffffffffffffffffffffffff1681525050505050620001e93374446c3b15f9926687d2c40534fdb564000000000000620002a060201b60201c565b62000bf4565b5f60208351101562000214576200020c836200032a60201b60201c565b90506200023e565b8262000226836200039460201b60201c565b5f01908162000236919062000825565b5060ff5f1b90505b92915050565b5f7f8b73c3c69bb8fe3d512ecc4cf759cc79239f7b179b0ffacaa9a75d522b39400f60e0516101005146306040516020016200028595949392919062000977565b60405160208183030381529060405280519060200120905090565b5f73ffffffffffffffffffffffffffffffffffffffff168273ffffffffffffffffffffffffffffffffffffffff160362000313575f6040517fec442f050000000000000000000000000000000000000000000000000000000081526004016200030a9190620009d2565b60405180910390fd5b620003265f83836200039d60201b60201c565b5050565b5f80829050601f815111156200037957826040517f305a27a900000000000000000000000000000000000000000000000000000000815260040162000370919062000a77565b60405180910390fd5b805181620003879062000ac8565b5f1c175f1b915050919050565b5f819050919050565b5f73ffffffffffffffffffffffffffffffffffffffff168373ffffffffffffffffffffffffffffffffffffffff1603620003f1578060025f828254620003e4919062000b64565b92505081905550620004c2565b5f805f8573ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff1681526020019081526020015f20549050818110156200047d578381836040517fe450d38c000000000000000000000000000000000000000000000000000000008152600401620004749392919062000b9e565b60405180910390fd5b8181035f808673ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff1681526020019081526020015f2081905550505b5f73ffffffffffffffffffffffffffffffffffffffff168273ffffffffffffffffffffffffffffffffffffffff16036200050b578060025f828254039250508190555062000555565b805f808473ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff1681526020019081526020015f205f82825401925050819055505b8173ffffffffffffffffffffffffffffffffffffffff168373ffffffffffffffffffffffffffffffffffffffff167fddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef83604051620005b4919062000bd9565b60405180910390a3505050565b5f81519050919050565b7f4e487b71000000000000000000000000000000000000000000000000000000005f52604160045260245ffd5b7f4e487b71000000000000000000000000000000000000000000000000000000005f52602260045260245ffd5b5f60028204905060018216806200063d57607f821691505b602082108103620006535762000652620005f8565b5b50919050565b5f819050815f5260205f209050919050565b5f6020601f8301049050919050565b5f82821b905092915050565b5f60088302620006b77fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff826200067a565b620006c386836200067a565b95508019841693508086168417925050509392505050565b5f819050919050565b5f819050919050565b5f6200070d620007076200070184620006db565b620006e4565b620006db565b9050919050565b5f819050919050565b6200072883620006ed565b62000740620007378262000714565b84845462000686565b825550505050565b5f90565b6200075662000748565b620007638184846200071d565b505050565b5b818110156200078a576200077e5f826200074c565b60018101905062000769565b5050565b601f821115620007d957620007a38162000659565b620007ae846200066b565b81016020851015620007be578190505b620007d6620007cd856200066b565b83018262000768565b50505b505050565b5f82821c905092915050565b5f620007fb5f1984600802620007de565b1980831691505092915050565b5f620008158383620007ea565b9150826002028217905092915050565b6200083082620005c1565b67ffffffffffffffff8111156200084c576200084b620005cb565b5b62000858825462000625565b620008658282856200078e565b5f60209050601f8311600181146200089b575f841562000886578287015190505b62000892858262000808565b86555062000901565b601f198416620008ab8662000659565b5f5b82811015620008d457848901518255600182019150602085019450602081019050620008ad565b86831015620008f45784890151620008f0601f891682620007ea565b8355505b6001600288020188555050505b505050505050565b5f819050919050565b6200091d8162000909565b82525050565b6200092e81620006db565b82525050565b5f73ffffffffffffffffffffffffffffffffffffffff82169050919050565b5f6200095f8262000934565b9050919050565b620009718162000953565b82525050565b5f60a0820190506200098c5f83018862000912565b6200099b602083018762000912565b620009aa604083018662000912565b620009b9606083018562000923565b620009c8608083018462000966565b9695505050505050565b5f602082019050620009e75f83018462000966565b92915050565b5f82825260208201905092915050565b5f5b8381101562000a1c578082015181840152602081019050620009ff565b5f8484015250505050565b5f601f19601f8301169050919050565b5f62000a4382620005c1565b62000a4f8185620009ed565b935062000a61818560208601620009fd565b62000a6c8162000a27565b840191505092915050565b5f6020820190508181035f83015262000a91818462000a37565b905092915050565b5f81519050919050565b5f819050602082019050919050565b5f62000abf825162000909565b80915050919050565b5f62000ad48262000a99565b8262000ae08462000aa3565b905062000aed8162000ab2565b9250602082101562000b305762000b2b7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff836020036008026200067a565b831692505b5050919050565b7f4e487b71000000000000000000000000000000000000000000000000000000005f52601160045260245ffd5b5f62000b7082620006db565b915062000b7d83620006db565b925082820190508082111562000b985762000b9762000b37565b5b92915050565b5f60608201905062000bb35f83018662000966565b62000bc2602083018562000923565b62000bd1604083018462000923565b949350505050565b5f60208201905062000bee5f83018462000923565b92915050565b60805160a05160c05160e051610100516101205161014051611b6e62000c465f395f610a1501525f6109da01525f610f0e01525f610eed01525f6108d801525f61092e01525f6109570152611b6e5ff3fe608060405234801561000f575f80fd5b50600436106100cd575f3560e01c806370a082311161008a57806395d89b411161006457806395d89b411461022d578063a9059cbb1461024b578063d505accf1461027b578063dd62ed3e14610297576100cd565b806370a08231146101a95780637ecebe00146101d957806384b0196e14610209576100cd565b806306fdde03146100d1578063095ea7b3146100ef57806318160ddd1461011f57806323b872dd1461013d578063313ce5671461016d5780633644e5151461018b575b5f80fd5b6100d96102c7565b6040516100e691906113de565b60405180910390f35b6101096004803603810190610104919061148f565b610357565b60405161011691906114e7565b60405180910390f35b610127610379565b604051610134919061150f565b60405180910390f35b61015760048036038101906101529190611528565b610382565b60405161016491906114e7565b60405180910390f35b6101756103b0565b6040516101829190611593565b60405180910390f35b6101936103b8565b6040516101a091906115c4565b60405180910390f35b6101c360048036038101906101be91906115dd565b6103c6565b6040516101d0919061150f565b60405180910390f35b6101f360048036038101906101ee91906115dd565b61040b565b604051610200919061150f565b60405180910390f35b61021161041c565b6040516102249796959493929190611708565b60405180910390f35b6102356104c1565b60405161024291906113de565b60405180910390f35b6102656004803603810190610260919061148f565b610551565b60405161027291906114e7565b60405180910390f35b610295600480360381019061029091906117de565b610573565b005b6102b160048036038101906102ac919061187b565b6106b8565b6040516102be919061150f565b60405180910390f35b6060600380546102d6906118e6565b80601f0160208091040260200160405190810160405280929190818152602001828054610302906118e6565b801561034d5780601f106103245761010080835404028352916020019161034d565b820191905f5260205f20905b81548152906001019060200180831161033057829003601f168201915b5050505050905090565b5f8061036161073a565b905061036e818585610741565b600191505092915050565b5f600254905090565b5f8061038c61073a565b9050610399858285610753565b6103a48585856107e5565b60019150509392505050565b5f6012905090565b5f6103c16108d5565b905090565b5f805f8373ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff1681526020019081526020015f20549050919050565b5f6104158261098b565b9050919050565b5f6060805f805f606061042d6109d1565b610435610a0c565b46305f801b5f67ffffffffffffffff81111561045457610453611916565b5b6040519080825280602002602001820160405280156104825781602001602082028036833780820191505090505b507f0f00000000000000000000000000000000000000000000000000000000000000959493929190965096509650965096509650965090919293949596565b6060600480546104d0906118e6565b80601f01602080910402602001604051908101604052809291908181526020018280546104fc906118e6565b80156105475780601f1061051e57610100808354040283529160200191610547565b820191905f5260205f20905b81548152906001019060200180831161052a57829003601f168201915b5050505050905090565b5f8061055b61073a565b90506105688185856107e5565b600191505092915050565b834211156105b857836040517f627913020000000000000000000000000000000000000000000000000000000081526004016105af919061150f565b60405180910390fd5b5f7f6e71edae12b1b97f4d1f60370fef10105fa2faae0126114a169c64845d6126c98888886105e68c610a47565b896040516020016105fc96959493929190611943565b6040516020818303038152906040528051906020012090505f61061e82610a9a565b90505f61062d82878787610ab3565b90508973ffffffffffffffffffffffffffffffffffffffff168173ffffffffffffffffffffffffffffffffffffffff16146106a157808a6040517f4b800e460000000000000000000000000000000000000000000000000000000081526004016106989291906119a2565b60405180910390fd5b6106ac8a8a8a610741565b50505050505050505050565b5f60015f8473ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff1681526020019081526020015f205f8373ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff1681526020019081526020015f2054905092915050565b5f33905090565b61074e8383836001610ae1565b505050565b5f61075e84846106b8565b90507fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff81146107df57818110156107d0578281836040517ffb8f41b20000000000000000000000000000000000000000000000000000000081526004016107c7939291906119c9565b60405180910390fd5b6107de84848484035f610ae1565b5b50505050565b5f73ffffffffffffffffffffffffffffffffffffffff168373ffffffffffffffffffffffffffffffffffffffff1603610855575f6040517f96c6fd1e00000000000000000000000000000000000000000000000000000000815260040161084c91906119fe565b60405180910390fd5b5f73ffffffffffffffffffffffffffffffffffffffff168273ffffffffffffffffffffffffffffffffffffffff16036108c5575f6040517fec442f050000000000000000000000000000000000000000000000000000000081526004016108bc91906119fe565b60405180910390fd5b6108d0838383610cb0565b505050565b5f7f000000000000000000000000000000000000000000000000000000000000000073ffffffffffffffffffffffffffffffffffffffff163073ffffffffffffffffffffffffffffffffffffffff1614801561095057507f000000000000000000000000000000000000000000000000000000000000000046145b1561097d577f00000000000000000000000000000000000000000000000000000000000000009050610988565b610985610ec9565b90505b90565b5f60075f8373ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff1681526020019081526020015f20549050919050565b6060610a0760057f0000000000000000000000000000000000000000000000000000000000000000610f5e90919063ffffffff16565b905090565b6060610a4260067f0000000000000000000000000000000000000000000000000000000000000000610f5e90919063ffffffff16565b905090565b5f60075f8373ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff1681526020019081526020015f205f815480929190600101919050559050919050565b5f610aac610aa66108d5565b8361100b565b9050919050565b5f805f80610ac38888888861104b565b925092509250610ad38282611132565b829350505050949350505050565b5f73ffffffffffffffffffffffffffffffffffffffff168473ffffffffffffffffffffffffffffffffffffffff1603610b51575f6040517fe602df05000000000000000000000000000000000000000000000000000000008152600401610b4891906119fe565b60405180910390fd5b5f73ffffffffffffffffffffffffffffffffffffffff168373ffffffffffffffffffffffffffffffffffffffff1603610bc1575f6040517f94280d62000000000000000000000000000000000000000000000000000000008152600401610bb891906119fe565b60405180910390fd5b8160015f8673ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff1681526020019081526020015f205f8573ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff1681526020019081526020015f20819055508015610caa578273ffffffffffffffffffffffffffffffffffffffff168473ffffffffffffffffffffffffffffffffffffffff167f8c5be1e5ebec7d5bd14f71427d1e84f3dd0314c0f7b2291e5b200ac8c7c3b92584604051610ca1919061150f565b60405180910390a35b50505050565b5f73ffffffffffffffffffffffffffffffffffffffff168373ffffffffffffffffffffffffffffffffffffffff1603610d00578060025f828254610cf49190611a44565b92505081905550610dce565b5f805f8573ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff1681526020019081526020015f2054905081811015610d89578381836040517fe450d38c000000000000000000000000000000000000000000000000000000008152600401610d80939291906119c9565b60405180910390fd5b8181035f808673ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff1681526020019081526020015f2081905550505b5f73ffffffffffffffffffffffffffffffffffffffff168273ffffffffffffffffffffffffffffffffffffffff1603610e15578060025f8282540392505081905550610e5f565b805f808473ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff1681526020019081526020015f205f82825401925050819055505b8173ffffffffffffffffffffffffffffffffffffffff168373ffffffffffffffffffffffffffffffffffffffff167fddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef83604051610ebc919061150f565b60405180910390a3505050565b5f7f8b73c3c69bb8fe3d512ecc4cf759cc79239f7b179b0ffacaa9a75d522b39400f7f00000000000000000000000000000000000000000000000000000000000000007f00000000000000000000000000000000000000000000000000000000000000004630604051602001610f43959493929190611a77565b60405160208183030381529060405280519060200120905090565b606060ff5f1b8314610f7a57610f7383611294565b9050611005565b818054610f86906118e6565b80601f0160208091040260200160405190810160405280929190818152602001828054610fb2906118e6565b8015610ffd5780601f10610fd457610100808354040283529160200191610ffd565b820191905f5260205f20905b815481529060010190602001808311610fe057829003601f168201915b505050505090505b92915050565b5f6040517f190100000000000000000000000000000000000000000000000000000000000081528360028201528260228201526042812091505092915050565b5f805f7f7fffffffffffffffffffffffffffffff5d576e7357a4501ddfe92f46681b20a0845f1c1115611087575f600385925092509250611128565b5f6001888888886040515f81526020016040526040516110aa9493929190611ac8565b6020604051602081039080840390855afa1580156110ca573d5f803e3d5ffd5b5050506020604051035190505f73ffffffffffffffffffffffffffffffffffffffff168173ffffffffffffffffffffffffffffffffffffffff160361111b575f60015f801b93509350935050611128565b805f805f1b935093509350505b9450945094915050565b5f600381111561114557611144611b0b565b5b82600381111561115857611157611b0b565b5b0315611290576001600381111561117257611171611b0b565b5b82600381111561118557611184611b0b565b5b036111bc576040517ff645eedf00000000000000000000000000000000000000000000000000000000815260040160405180910390fd5b600260038111156111d0576111cf611b0b565b5b8260038111156111e3576111e2611b0b565b5b0361122757805f1c6040517ffce698f700000000000000000000000000000000000000000000000000000000815260040161121e919061150f565b60405180910390fd5b60038081111561123a57611239611b0b565b5b82600381111561124d5761124c611b0b565b5b0361128f57806040517fd78bce0c00000000000000000000000000000000000000000000000000000000815260040161128691906115c4565b60405180910390fd5b5b5050565b60605f6112a083611306565b90505f602067ffffffffffffffff8111156112be576112bd611916565b5b6040519080825280601f01601f1916602001820160405280156112f05781602001600182028036833780820191505090505b5090508181528360208201528092505050919050565b5f8060ff835f1c169050601f81111561134b576040517fb3512b0c00000000000000000000000000000000000000000000000000000000815260040160405180910390fd5b80915050919050565b5f81519050919050565b5f82825260208201905092915050565b5f5b8381101561138b578082015181840152602081019050611370565b5f8484015250505050565b5f601f19601f8301169050919050565b5f6113b082611354565b6113ba818561135e565b93506113ca81856020860161136e565b6113d381611396565b840191505092915050565b5f6020820190508181035f8301526113f681846113a6565b905092915050565b5f80fd5b5f73ffffffffffffffffffffffffffffffffffffffff82169050919050565b5f61142b82611402565b9050919050565b61143b81611421565b8114611445575f80fd5b50565b5f8135905061145681611432565b92915050565b5f819050919050565b61146e8161145c565b8114611478575f80fd5b50565b5f8135905061148981611465565b92915050565b5f80604083850312156114a5576114a46113fe565b5b5f6114b285828601611448565b92505060206114c38582860161147b565b9150509250929050565b5f8115159050919050565b6114e1816114cd565b82525050565b5f6020820190506114fa5f8301846114d8565b92915050565b6115098161145c565b82525050565b5f6020820190506115225f830184611500565b92915050565b5f805f6060848603121561153f5761153e6113fe565b5b5f61154c86828701611448565b935050602061155d86828701611448565b925050604061156e8682870161147b565b9150509250925092565b5f60ff82169050919050565b61158d81611578565b82525050565b5f6020820190506115a65f830184611584565b92915050565b5f819050919050565b6115be816115ac565b82525050565b5f6020820190506115d75f8301846115b5565b92915050565b5f602082840312156115f2576115f16113fe565b5b5f6115ff84828501611448565b91505092915050565b5f7fff0000000000000000000000000000000000000000000000000000000000000082169050919050565b61163c81611608565b82525050565b61164b81611421565b82525050565b5f81519050919050565b5f82825260208201905092915050565b5f819050602082019050919050565b6116838161145c565b82525050565b5f611694838361167a565b60208301905092915050565b5f602082019050919050565b5f6116b682611651565b6116c0818561165b565b93506116cb8361166b565b805f5b838110156116fb5781516116e28882611689565b97506116ed836116a0565b9250506001810190506116ce565b5085935050505092915050565b5f60e08201905061171b5f83018a611633565b818103602083015261172d81896113a6565b9050818103604083015261174181886113a6565b90506117506060830187611500565b61175d6080830186611642565b61176a60a08301856115b5565b81810360c083015261177c81846116ac565b905098975050505050505050565b61179381611578565b811461179d575f80fd5b50565b5f813590506117ae8161178a565b92915050565b6117bd816115ac565b81146117c7575f80fd5b50565b5f813590506117d8816117b4565b92915050565b5f805f805f805f60e0888a0312156117f9576117f86113fe565b5b5f6118068a828b01611448565b97505060206118178a828b01611448565b96505060406118288a828b0161147b565b95505060606118398a828b0161147b565b945050608061184a8a828b016117a0565b93505060a061185b8a828b016117ca565b92505060c061186c8a828b016117ca565b91505092959891949750929550565b5f8060408385031215611891576118906113fe565b5b5f61189e85828601611448565b92505060206118af85828601611448565b9150509250929050565b7f4e487b71000000000000000000000000000000000000000000000000000000005f52602260045260245ffd5b5f60028204905060018216806118fd57607f821691505b6020821081036119105761190f6118b9565b5b50919050565b7f4e487b71000000000000000000000000000000000000000000000000000000005f52604160045260245ffd5b5f60c0820190506119565f8301896115b5565b6119636020830188611642565b6119706040830187611642565b61197d6060830186611500565b61198a6080830185611500565b61199760a0830184611500565b979650505050505050565b5f6040820190506119b55f830185611642565b6119c26020830184611642565b9392505050565b5f6060820190506119dc5f830186611642565b6119e96020830185611500565b6119f66040830184611500565b949350505050565b5f602082019050611a115f830184611642565b92915050565b7f4e487b71000000000000000000000000000000000000000000000000000000005f52601160045260245ffd5b5f611a4e8261145c565b9150611a598361145c565b9250828201905080821115611a7157611a70611a17565b5b92915050565b5f60a082019050611a8a5f8301886115b5565b611a9760208301876115b5565b611aa460408301866115b5565b611ab16060830185611500565b611abe6080830184611642565b9695505050505050565b5f608082019050611adb5f8301876115b5565b611ae86020830186611584565b611af560408301856115b5565b611b0260608301846115b5565b95945050505050565b7f4e487b71000000000000000000000000000000000000000000000000000000005f52602160045260245ffdfea26469706673582212204c3515b97d018ad9f70b9fbbf4ed456ec939a9586dd16b64ef87458530a0f30b64736f6c63430008180033");
        // const TestAccount& from, const Address& to, const uint256_t& value, Bytes data = Bytes()
        auto createTx = sdk.createNewTx(sdk.getChainOwnerAccount(), Address(), 0, erc20CreateBytes);
        std::cout << "test txDataSize: " << createTx.getData().size() << std::endl;
        auto gas = sdk.estimateGas(createTx);

        std::cout << "test gas: " << gas << std::endl;
        REQUIRE(sdk.getEvmContracts().empty());
        REQUIRE(sdk.getEvmContractAddress(createTx.hash()) == Address());

        sdk.advanceChain(0, {createTx});
        REQUIRE(sdk.getEvmContracts().size() == 1);
        REQUIRE(sdk.getEvmContractAddress(createTx.hash()) != Address());
        createTxHash = createTx.hash();
        auto contractAddress = sdk.getEvmContractAddress(createTxHash);
        REQUIRE(sdk.isEvmContract(contractAddress));
        ERC20Address = contractAddress;
        // Try calling get balance, technically we are ABI compatible!
        finalOwnerTokenBal = sdk.callViewFunction(contractAddress, &ERC20::balanceOf, sdk.getChainOwnerAccount().address);
        std::cout << "test ownerBalance: " << finalOwnerTokenBal << std::endl;
        // now try transfering the token!!!
        auto transferTx = sdk.callFunction(contractAddress, &ERC20::transfer, toAccount.address, uint256_t(1000000000));
        // Now get owner and user balance again...
        finalOwnerTokenBal = sdk.callViewFunction(contractAddress, &ERC20::balanceOf, sdk.getChainOwnerAccount().address);
        finalRecipientTokenBal = sdk.callViewFunction(contractAddress, &ERC20::balanceOf, toAccount.address);
        finalOwnerNativeBal = sdk.getNativeBalance(sdk.getChainOwnerAccount().address);
        finalRecipientNativeBal = sdk.getNativeBalance(toAccount.address);
        finalRecipientNonce = sdk.getNativeNonce(toAccount.address);
        finalOwnerNonce = sdk.getNativeNonce(sdk.getChainOwnerAccount().address);
        std::cout << "test ownerBalanceAfter: " << finalRecipientTokenBal << std::endl;
        std::cout << "test toBalance: " << finalRecipientTokenBal << std::endl;
        // Let's see if we can find the transfer event!
        auto events = sdk.getEventsEmittedByAddress(ERC20Address, &ERC20::Transfer);
        REQUIRE(events.size() == 2);  // mint is also called on the constructor which calls an event to be emmited

      }
      std::cout << "dump to DB successfull" << std::endl;

      // Try loading the State again
      SDKTestSuite sdk = SDKTestSuite::loadEnvironment("TestEVMOne_ERC20");
      REQUIRE(sdk.isEvmContract(ERC20Address));
      uint256_t loadedOwnerNativeBal = sdk.getNativeBalance(sdk.getChainOwnerAccount().address);
      uint256_t loadedRecipientNativeBal = sdk.getNativeBalance(toAccount.address);
      uint256_t loadedOwnerTokenBal = sdk.callViewFunction(ERC20Address, &ERC20::balanceOf, sdk.getChainOwnerAccount().address);
      uint256_t loadedRecipientTokenBal = sdk.callViewFunction(ERC20Address, &ERC20::balanceOf, toAccount.address);
      uint256_t loadedOwnerNonce = sdk.getNativeNonce(sdk.getChainOwnerAccount().address);
      uint256_t loadedRecipientNonce = sdk.getNativeNonce(toAccount.address);
      auto loadedContractAddress = sdk.getEvmContractAddress(createTxHash);
      REQUIRE(loadedOwnerNativeBal == finalOwnerNativeBal);
      REQUIRE(loadedRecipientNativeBal == finalRecipientNativeBal);
      REQUIRE(loadedOwnerTokenBal == finalOwnerTokenBal);
      REQUIRE(loadedRecipientTokenBal == finalRecipientTokenBal);
      REQUIRE(loadedOwnerNonce == finalOwnerNonce);
      REQUIRE(loadedRecipientNonce == finalRecipientNonce);
      std::cout << "createTxHash: " << createTxHash.hex(true) << std::endl;
      std::cout << "loadedContractAddress: " << loadedContractAddress.hex(true) << std::endl;
      std::cout << "ERC20Address: " << ERC20Address.hex(true) << std::endl;
      REQUIRE(loadedContractAddress == ERC20Address);
      // Now try to transfer the token again
      auto loadedTransferTx = sdk.callFunction(loadedContractAddress, &ERC20::transfer, toAccount.address, uint256_t(1000000000));
      // Now get owner and user balance again...
      uint256_t loadedOwnerTokenBalAfter = sdk.callViewFunction(loadedContractAddress, &ERC20::balanceOf, sdk.getChainOwnerAccount().address);
      uint256_t loadedRecipientTokenBalAfter = sdk.callViewFunction(loadedContractAddress, &ERC20::balanceOf, toAccount.address);
      uint256_t loadedOwnerNativeBalAfter = sdk.getNativeBalance(sdk.getChainOwnerAccount().address);
      uint256_t loadedRecipientNativeBalAfter = sdk.getNativeBalance(toAccount.address);
      uint256_t loadedRecipientNonceAfter = sdk.getNativeNonce(toAccount.address);
      uint256_t loadedOwnerNonceAfter = sdk.getNativeNonce(sdk.getChainOwnerAccount().address);

      REQUIRE(loadedOwnerTokenBalAfter == finalOwnerTokenBal - 1000000000);
      REQUIRE(loadedRecipientTokenBalAfter == finalRecipientTokenBal + 1000000000);
      REQUIRE(loadedOwnerNativeBalAfter < finalOwnerNativeBal);
      REQUIRE(loadedRecipientNativeBalAfter == finalRecipientNativeBal);
      REQUIRE(loadedRecipientNonceAfter == finalRecipientNonce);
      REQUIRE(loadedOwnerNonceAfter == finalOwnerNonce + 1);
      // Sanity Check
      REQUIRE(loadedRecipientNonce == 0);
      REQUIRE(loadedRecipientNativeBal == 0);
    }
  }
}