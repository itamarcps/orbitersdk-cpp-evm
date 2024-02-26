/*
Copyright (c) [2023-2024] [Sparq Network]

This software is distributed under the MIT License.
See the LICENSE.txt file in the project root for more information.
*/

#include <iostream>
#include <filesystem>
#include <evmc/evmc.hpp>
#include <evmc/hex.hpp>
#include "src/core/mock_host.hpp"
#include <evmone/evmone.h>

#include "src/core/blockchain.h"
#include "src/core/evmhost.hpp"

using namespace evmc::literals;

constexpr int64_t GAS = INT64_MAX;
const auto ZERO_ADDRESS = 0x0000000000000000000000000000000000000000_address;
const auto CONTRACT_ADDRESS = 0x2000000000000000000000000000000000000002_address;
const auto ANOTHER_CONTRACT_ADDRESS = 0x3000000000000000000000000000000000000003_address;
const auto CALLER_ADDRESS = 0x1000000000000000000000000000000000000001_address;
const auto ANOTHER_CALLED_ADDRESS = 0x4000000000000000000000000000000000000004_address;
const auto ECRECOVER_CONTRACT_ADDRESS = 0x5000000000000000000000000000000000000005_address;

std::unique_ptr<Blockchain> blockchain = nullptr;

[[noreturn]] void signalHandler(int signum) {
  Logger::logToDebug(LogType::INFO, "MAIN", "MAIN", "Received signal " + std::to_string(signum) + ". Stopping the blockchain.");
  blockchain->stop();
  blockchain = nullptr; // Destroy the blockchain object, calling the destructor of every module and dumping to DB.
  Utils::safePrint("Exiting...");
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  exit(signum);
}

int main() {
  Bytes contractAbi = Hex::toBytes("61016060405234801562000011575f80fd5b506040518060400160405280600781526020017f4d79546f6b656e00000000000000000000000000000000000000000000000000815250806040518060400160405280600181526020017f31000000000000000000000000000000000000000000000000000000000000008152506040518060400160405280600781526020017f4d79546f6b656e000000000000000000000000000000000000000000000000008152506040518060400160405280600381526020017f4d544b00000000000000000000000000000000000000000000000000000000008152508160039081620000fc919062000818565b5080600490816200010e919062000818565b50505062000127600583620001e260201b90919060201c565b610120818152505062000145600682620001e260201b90919060201c565b6101408181525050818051906020012060e08181525050808051906020012061010081815250504660a08181525050620001846200023760201b60201c565b608081815250503073ffffffffffffffffffffffffffffffffffffffff1660c08173ffffffffffffffffffffffffffffffffffffffff1681525050505050620001dc33670de0b6b3a76400006200029360201b60201c565b62000be7565b5f6020835110156200020757620001ff836200031d60201b60201c565b905062000231565b8262000219836200038760201b60201c565b5f01908162000229919062000818565b5060ff5f1b90505b92915050565b5f7f8b73c3c69bb8fe3d512ecc4cf759cc79239f7b179b0ffacaa9a75d522b39400f60e051610100514630604051602001620002789594939291906200096a565b60405160208183030381529060405280519060200120905090565b5f73ffffffffffffffffffffffffffffffffffffffff168273ffffffffffffffffffffffffffffffffffffffff160362000306575f6040517fec442f05000000000000000000000000000000000000000000000000000000008152600401620002fd9190620009c5565b60405180910390fd5b620003195f83836200039060201b60201c565b5050565b5f80829050601f815111156200036c57826040517f305a27a900000000000000000000000000000000000000000000000000000000815260040162000363919062000a6a565b60405180910390fd5b8051816200037a9062000abb565b5f1c175f1b915050919050565b5f819050919050565b5f73ffffffffffffffffffffffffffffffffffffffff168373ffffffffffffffffffffffffffffffffffffffff1603620003e4578060025f828254620003d7919062000b57565b92505081905550620004b5565b5f805f8573ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff1681526020019081526020015f205490508181101562000470578381836040517fe450d38c000000000000000000000000000000000000000000000000000000008152600401620004679392919062000b91565b60405180910390fd5b8181035f808673ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff1681526020019081526020015f2081905550505b5f73ffffffffffffffffffffffffffffffffffffffff168273ffffffffffffffffffffffffffffffffffffffff1603620004fe578060025f828254039250508190555062000548565b805f808473ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff1681526020019081526020015f205f82825401925050819055505b8173ffffffffffffffffffffffffffffffffffffffff168373ffffffffffffffffffffffffffffffffffffffff167fddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef83604051620005a7919062000bcc565b60405180910390a3505050565b5f81519050919050565b7f4e487b71000000000000000000000000000000000000000000000000000000005f52604160045260245ffd5b7f4e487b71000000000000000000000000000000000000000000000000000000005f52602260045260245ffd5b5f60028204905060018216806200063057607f821691505b602082108103620006465762000645620005eb565b5b50919050565b5f819050815f5260205f209050919050565b5f6020601f8301049050919050565b5f82821b905092915050565b5f60088302620006aa7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff826200066d565b620006b686836200066d565b95508019841693508086168417925050509392505050565b5f819050919050565b5f819050919050565b5f62000700620006fa620006f484620006ce565b620006d7565b620006ce565b9050919050565b5f819050919050565b6200071b83620006e0565b620007336200072a8262000707565b84845462000679565b825550505050565b5f90565b620007496200073b565b6200075681848462000710565b505050565b5b818110156200077d57620007715f826200073f565b6001810190506200075c565b5050565b601f821115620007cc5762000796816200064c565b620007a1846200065e565b81016020851015620007b1578190505b620007c9620007c0856200065e565b8301826200075b565b50505b505050565b5f82821c905092915050565b5f620007ee5f1984600802620007d1565b1980831691505092915050565b5f620008088383620007dd565b9150826002028217905092915050565b6200082382620005b4565b67ffffffffffffffff8111156200083f576200083e620005be565b5b6200084b825462000618565b6200085882828562000781565b5f60209050601f8311600181146200088e575f841562000879578287015190505b620008858582620007fb565b865550620008f4565b601f1984166200089e866200064c565b5f5b82811015620008c757848901518255600182019150602085019450602081019050620008a0565b86831015620008e75784890151620008e3601f891682620007dd565b8355505b6001600288020188555050505b505050505050565b5f819050919050565b6200091081620008fc565b82525050565b6200092181620006ce565b82525050565b5f73ffffffffffffffffffffffffffffffffffffffff82169050919050565b5f620009528262000927565b9050919050565b620009648162000946565b82525050565b5f60a0820190506200097f5f83018862000905565b6200098e602083018762000905565b6200099d604083018662000905565b620009ac606083018562000916565b620009bb608083018462000959565b9695505050505050565b5f602082019050620009da5f83018462000959565b92915050565b5f82825260208201905092915050565b5f5b8381101562000a0f578082015181840152602081019050620009f2565b5f8484015250505050565b5f601f19601f8301169050919050565b5f62000a3682620005b4565b62000a428185620009e0565b935062000a54818560208601620009f0565b62000a5f8162000a1a565b840191505092915050565b5f6020820190508181035f83015262000a84818462000a2a565b905092915050565b5f81519050919050565b5f819050602082019050919050565b5f62000ab28251620008fc565b80915050919050565b5f62000ac78262000a8c565b8262000ad38462000a96565b905062000ae08162000aa5565b9250602082101562000b235762000b1e7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff836020036008026200066d565b831692505b5050919050565b7f4e487b71000000000000000000000000000000000000000000000000000000005f52601160045260245ffd5b5f62000b6382620006ce565b915062000b7083620006ce565b925082820190508082111562000b8b5762000b8a62000b2a565b5b92915050565b5f60608201905062000ba65f83018662000959565b62000bb5602083018562000916565b62000bc4604083018462000916565b949350505050565b5f60208201905062000be15f83018462000916565b92915050565b60805160a05160c05160e051610100516101205161014051611b6e62000c395f395f610a1501525f6109da01525f610f0e01525f610eed01525f6108d801525f61092e01525f6109570152611b6e5ff3fe608060405234801561000f575f80fd5b50600436106100cd575f3560e01c806370a082311161008a57806395d89b411161006457806395d89b411461022d578063a9059cbb1461024b578063d505accf1461027b578063dd62ed3e14610297576100cd565b806370a08231146101a95780637ecebe00146101d957806384b0196e14610209576100cd565b806306fdde03146100d1578063095ea7b3146100ef57806318160ddd1461011f57806323b872dd1461013d578063313ce5671461016d5780633644e5151461018b575b5f80fd5b6100d96102c7565b6040516100e691906113de565b60405180910390f35b6101096004803603810190610104919061148f565b610357565b60405161011691906114e7565b60405180910390f35b610127610379565b604051610134919061150f565b60405180910390f35b61015760048036038101906101529190611528565b610382565b60405161016491906114e7565b60405180910390f35b6101756103b0565b6040516101829190611593565b60405180910390f35b6101936103b8565b6040516101a091906115c4565b60405180910390f35b6101c360048036038101906101be91906115dd565b6103c6565b6040516101d0919061150f565b60405180910390f35b6101f360048036038101906101ee91906115dd565b61040b565b604051610200919061150f565b60405180910390f35b61021161041c565b6040516102249796959493929190611708565b60405180910390f35b6102356104c1565b60405161024291906113de565b60405180910390f35b6102656004803603810190610260919061148f565b610551565b60405161027291906114e7565b60405180910390f35b610295600480360381019061029091906117de565b610573565b005b6102b160048036038101906102ac919061187b565b6106b8565b6040516102be919061150f565b60405180910390f35b6060600380546102d6906118e6565b80601f0160208091040260200160405190810160405280929190818152602001828054610302906118e6565b801561034d5780601f106103245761010080835404028352916020019161034d565b820191905f5260205f20905b81548152906001019060200180831161033057829003601f168201915b5050505050905090565b5f8061036161073a565b905061036e818585610741565b600191505092915050565b5f600254905090565b5f8061038c61073a565b9050610399858285610753565b6103a48585856107e5565b60019150509392505050565b5f6012905090565b5f6103c16108d5565b905090565b5f805f8373ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff1681526020019081526020015f20549050919050565b5f6104158261098b565b9050919050565b5f6060805f805f606061042d6109d1565b610435610a0c565b46305f801b5f67ffffffffffffffff81111561045457610453611916565b5b6040519080825280602002602001820160405280156104825781602001602082028036833780820191505090505b507f0f00000000000000000000000000000000000000000000000000000000000000959493929190965096509650965096509650965090919293949596565b6060600480546104d0906118e6565b80601f01602080910402602001604051908101604052809291908181526020018280546104fc906118e6565b80156105475780601f1061051e57610100808354040283529160200191610547565b820191905f5260205f20905b81548152906001019060200180831161052a57829003601f168201915b5050505050905090565b5f8061055b61073a565b90506105688185856107e5565b600191505092915050565b834211156105b857836040517f627913020000000000000000000000000000000000000000000000000000000081526004016105af919061150f565b60405180910390fd5b5f7f6e71edae12b1b97f4d1f60370fef10105fa2faae0126114a169c64845d6126c98888886105e68c610a47565b896040516020016105fc96959493929190611943565b6040516020818303038152906040528051906020012090505f61061e82610a9a565b90505f61062d82878787610ab3565b90508973ffffffffffffffffffffffffffffffffffffffff168173ffffffffffffffffffffffffffffffffffffffff16146106a157808a6040517f4b800e460000000000000000000000000000000000000000000000000000000081526004016106989291906119a2565b60405180910390fd5b6106ac8a8a8a610741565b50505050505050505050565b5f60015f8473ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff1681526020019081526020015f205f8373ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff1681526020019081526020015f2054905092915050565b5f33905090565b61074e8383836001610ae1565b505050565b5f61075e84846106b8565b90507fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff81146107df57818110156107d0578281836040517ffb8f41b20000000000000000000000000000000000000000000000000000000081526004016107c7939291906119c9565b60405180910390fd5b6107de84848484035f610ae1565b5b50505050565b5f73ffffffffffffffffffffffffffffffffffffffff168373ffffffffffffffffffffffffffffffffffffffff1603610855575f6040517f96c6fd1e00000000000000000000000000000000000000000000000000000000815260040161084c91906119fe565b60405180910390fd5b5f73ffffffffffffffffffffffffffffffffffffffff168273ffffffffffffffffffffffffffffffffffffffff16036108c5575f6040517fec442f050000000000000000000000000000000000000000000000000000000081526004016108bc91906119fe565b60405180910390fd5b6108d0838383610cb0565b505050565b5f7f000000000000000000000000000000000000000000000000000000000000000073ffffffffffffffffffffffffffffffffffffffff163073ffffffffffffffffffffffffffffffffffffffff1614801561095057507f000000000000000000000000000000000000000000000000000000000000000046145b1561097d577f00000000000000000000000000000000000000000000000000000000000000009050610988565b610985610ec9565b90505b90565b5f60075f8373ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff1681526020019081526020015f20549050919050565b6060610a0760057f0000000000000000000000000000000000000000000000000000000000000000610f5e90919063ffffffff16565b905090565b6060610a4260067f0000000000000000000000000000000000000000000000000000000000000000610f5e90919063ffffffff16565b905090565b5f60075f8373ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff1681526020019081526020015f205f815480929190600101919050559050919050565b5f610aac610aa66108d5565b8361100b565b9050919050565b5f805f80610ac38888888861104b565b925092509250610ad38282611132565b829350505050949350505050565b5f73ffffffffffffffffffffffffffffffffffffffff168473ffffffffffffffffffffffffffffffffffffffff1603610b51575f6040517fe602df05000000000000000000000000000000000000000000000000000000008152600401610b4891906119fe565b60405180910390fd5b5f73ffffffffffffffffffffffffffffffffffffffff168373ffffffffffffffffffffffffffffffffffffffff1603610bc1575f6040517f94280d62000000000000000000000000000000000000000000000000000000008152600401610bb891906119fe565b60405180910390fd5b8160015f8673ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff1681526020019081526020015f205f8573ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff1681526020019081526020015f20819055508015610caa578273ffffffffffffffffffffffffffffffffffffffff168473ffffffffffffffffffffffffffffffffffffffff167f8c5be1e5ebec7d5bd14f71427d1e84f3dd0314c0f7b2291e5b200ac8c7c3b92584604051610ca1919061150f565b60405180910390a35b50505050565b5f73ffffffffffffffffffffffffffffffffffffffff168373ffffffffffffffffffffffffffffffffffffffff1603610d00578060025f828254610cf49190611a44565b92505081905550610dce565b5f805f8573ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff1681526020019081526020015f2054905081811015610d89578381836040517fe450d38c000000000000000000000000000000000000000000000000000000008152600401610d80939291906119c9565b60405180910390fd5b8181035f808673ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff1681526020019081526020015f2081905550505b5f73ffffffffffffffffffffffffffffffffffffffff168273ffffffffffffffffffffffffffffffffffffffff1603610e15578060025f8282540392505081905550610e5f565b805f808473ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff1681526020019081526020015f205f82825401925050819055505b8173ffffffffffffffffffffffffffffffffffffffff168373ffffffffffffffffffffffffffffffffffffffff167fddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef83604051610ebc919061150f565b60405180910390a3505050565b5f7f8b73c3c69bb8fe3d512ecc4cf759cc79239f7b179b0ffacaa9a75d522b39400f7f00000000000000000000000000000000000000000000000000000000000000007f00000000000000000000000000000000000000000000000000000000000000004630604051602001610f43959493929190611a77565b60405160208183030381529060405280519060200120905090565b606060ff5f1b8314610f7a57610f7383611294565b9050611005565b818054610f86906118e6565b80601f0160208091040260200160405190810160405280929190818152602001828054610fb2906118e6565b8015610ffd5780601f10610fd457610100808354040283529160200191610ffd565b820191905f5260205f20905b815481529060010190602001808311610fe057829003601f168201915b505050505090505b92915050565b5f6040517f190100000000000000000000000000000000000000000000000000000000000081528360028201528260228201526042812091505092915050565b5f805f7f7fffffffffffffffffffffffffffffff5d576e7357a4501ddfe92f46681b20a0845f1c1115611087575f600385925092509250611128565b5f6001888888886040515f81526020016040526040516110aa9493929190611ac8565b6020604051602081039080840390855afa1580156110ca573d5f803e3d5ffd5b5050506020604051035190505f73ffffffffffffffffffffffffffffffffffffffff168173ffffffffffffffffffffffffffffffffffffffff160361111b575f60015f801b93509350935050611128565b805f805f1b935093509350505b9450945094915050565b5f600381111561114557611144611b0b565b5b82600381111561115857611157611b0b565b5b0315611290576001600381111561117257611171611b0b565b5b82600381111561118557611184611b0b565b5b036111bc576040517ff645eedf00000000000000000000000000000000000000000000000000000000815260040160405180910390fd5b600260038111156111d0576111cf611b0b565b5b8260038111156111e3576111e2611b0b565b5b0361122757805f1c6040517ffce698f700000000000000000000000000000000000000000000000000000000815260040161121e919061150f565b60405180910390fd5b60038081111561123a57611239611b0b565b5b82600381111561124d5761124c611b0b565b5b0361128f57806040517fd78bce0c00000000000000000000000000000000000000000000000000000000815260040161128691906115c4565b60405180910390fd5b5b5050565b60605f6112a083611306565b90505f602067ffffffffffffffff8111156112be576112bd611916565b5b6040519080825280601f01601f1916602001820160405280156112f05781602001600182028036833780820191505090505b5090508181528360208201528092505050919050565b5f8060ff835f1c169050601f81111561134b576040517fb3512b0c00000000000000000000000000000000000000000000000000000000815260040160405180910390fd5b80915050919050565b5f81519050919050565b5f82825260208201905092915050565b5f5b8381101561138b578082015181840152602081019050611370565b5f8484015250505050565b5f601f19601f8301169050919050565b5f6113b082611354565b6113ba818561135e565b93506113ca81856020860161136e565b6113d381611396565b840191505092915050565b5f6020820190508181035f8301526113f681846113a6565b905092915050565b5f80fd5b5f73ffffffffffffffffffffffffffffffffffffffff82169050919050565b5f61142b82611402565b9050919050565b61143b81611421565b8114611445575f80fd5b50565b5f8135905061145681611432565b92915050565b5f819050919050565b61146e8161145c565b8114611478575f80fd5b50565b5f8135905061148981611465565b92915050565b5f80604083850312156114a5576114a46113fe565b5b5f6114b285828601611448565b92505060206114c38582860161147b565b9150509250929050565b5f8115159050919050565b6114e1816114cd565b82525050565b5f6020820190506114fa5f8301846114d8565b92915050565b6115098161145c565b82525050565b5f6020820190506115225f830184611500565b92915050565b5f805f6060848603121561153f5761153e6113fe565b5b5f61154c86828701611448565b935050602061155d86828701611448565b925050604061156e8682870161147b565b9150509250925092565b5f60ff82169050919050565b61158d81611578565b82525050565b5f6020820190506115a65f830184611584565b92915050565b5f819050919050565b6115be816115ac565b82525050565b5f6020820190506115d75f8301846115b5565b92915050565b5f602082840312156115f2576115f16113fe565b5b5f6115ff84828501611448565b91505092915050565b5f7fff0000000000000000000000000000000000000000000000000000000000000082169050919050565b61163c81611608565b82525050565b61164b81611421565b82525050565b5f81519050919050565b5f82825260208201905092915050565b5f819050602082019050919050565b6116838161145c565b82525050565b5f611694838361167a565b60208301905092915050565b5f602082019050919050565b5f6116b682611651565b6116c0818561165b565b93506116cb8361166b565b805f5b838110156116fb5781516116e28882611689565b97506116ed836116a0565b9250506001810190506116ce565b5085935050505092915050565b5f60e08201905061171b5f83018a611633565b818103602083015261172d81896113a6565b9050818103604083015261174181886113a6565b90506117506060830187611500565b61175d6080830186611642565b61176a60a08301856115b5565b81810360c083015261177c81846116ac565b905098975050505050505050565b61179381611578565b811461179d575f80fd5b50565b5f813590506117ae8161178a565b92915050565b6117bd816115ac565b81146117c7575f80fd5b50565b5f813590506117d8816117b4565b92915050565b5f805f805f805f60e0888a0312156117f9576117f86113fe565b5b5f6118068a828b01611448565b97505060206118178a828b01611448565b96505060406118288a828b0161147b565b95505060606118398a828b0161147b565b945050608061184a8a828b016117a0565b93505060a061185b8a828b016117ca565b92505060c061186c8a828b016117ca565b91505092959891949750929550565b5f8060408385031215611891576118906113fe565b5b5f61189e85828601611448565b92505060206118af85828601611448565b9150509250929050565b7f4e487b71000000000000000000000000000000000000000000000000000000005f52602260045260245ffd5b5f60028204905060018216806118fd57607f821691505b6020821081036119105761190f6118b9565b5b50919050565b7f4e487b71000000000000000000000000000000000000000000000000000000005f52604160045260245ffd5b5f60c0820190506119565f8301896115b5565b6119636020830188611642565b6119706040830187611642565b61197d6060830186611500565b61198a6080830185611500565b61199760a0830184611500565b979650505050505050565b5f6040820190506119b55f830185611642565b6119c26020830184611642565b9392505050565b5f6060820190506119dc5f830186611642565b6119e96020830185611500565b6119f66040830184611500565b949350505050565b5f602082019050611a115f830184611642565b92915050565b7f4e487b71000000000000000000000000000000000000000000000000000000005f52601160045260245ffd5b5f611a4e8261145c565b9150611a598361145c565b9250828201905080821115611a7157611a70611a17565b5b92915050565b5f60a082019050611a8a5f8301886115b5565b611a9760208301876115b5565b611aa460408301866115b5565b611ab16060830185611500565b611abe6080830184611642565b9695505050505050565b5f608082019050611adb5f8301876115b5565b611ae86020830186611584565b611af560408301856115b5565b611b0260608301846115b5565b95945050505050565b7f4e487b71000000000000000000000000000000000000000000000000000000005f52602160045260245ffdfea264697066735822122085cbb8d2aed8ee532b622a21430bcdc9979cb36e9b70f49d0d6d4f564bffc18d64736f6c63430008180033");
  const auto vm = evmc_create_evmone();
  evmc::MockedHost host(vm);



  evmc_message create_msg{};
  create_msg.sender = CALLER_ADDRESS;
  create_msg.kind = evmc_call_kind::EVMC_CREATE;
  create_msg.recipient = CONTRACT_ADDRESS;
  create_msg.gas = GAS;

  std::cout << "account code: " << host.accounts[CONTRACT_ADDRESS].code.size() << std::endl;
  std::cout << "account storage size: " << host.accounts[CONTRACT_ADDRESS].storage.size() << std::endl;
  std::cout << "account transient storage: " << host.accounts[CONTRACT_ADDRESS].transient_storage.size() << std::endl;

  auto create_result =
    evmc_execute(vm, &host.get_interface(), (evmc_host_context*)&host,
                 evmc_revision::EVMC_LATEST_STABLE_REVISION, &create_msg,
                 contractAbi.data(), contractAbi.size());

  std::cout << "create_result.status: " << create_result.status_code << std::endl;
  std::cout << "create_result.gas_left: " << create_result.gas_left << std::endl;
  std::cout << "create_resullt.gas used: " << GAS - create_result.gas_left << std::endl;
  std::cout << "account code: " << host.accounts[CONTRACT_ADDRESS].code.size() << std::endl;
  std::cout << "account storage size: " << host.accounts[CONTRACT_ADDRESS].storage.size() << std::endl;
  std::cout << "account transient storage: " << host.accounts[CONTRACT_ADDRESS].transient_storage.size() << std::endl;
  Bytes balanceOf = Hex::toBytes("0x70a082310000000000000000000000001000000000000000000000000000000000000001");

  /// Put the code into the account
  host.accounts[CONTRACT_ADDRESS].code = std::basic_string<uint8_t>(create_result.output_data, create_result.output_size);

  evmc_message call_msg{};
  call_msg.kind = evmc_call_kind::EVMC_CALL;

  // Print which accounts exists in the host
  for (auto& [key, val] : host.accounts) {
    BytesArr<20> addressBytes;
    // key.bytes is a C array of 20 bytes
    std::copy(key.bytes, key.bytes + 20, addressBytes.begin());
    Address address(addressBytes);
    std::cout << "Account: " << address.hex(true) << std::endl;
    std::cout << "Val.code.size(): " << val.code.size() << std::endl;
    std::cout << "Val.nonce: " << val.nonce << std::endl;
    BytesArr<32> balanceBytes;
    std::copy(val.balance.bytes, val.balance.bytes + 32, balanceBytes.begin());
    std::cout << "Val.balance: " << Utils::bytesToUint256(balanceBytes) << std::endl;
    std::cout << "Val.storage.size(): " << val.storage.size() << std::endl;
    for (auto const& [key, val] : val.storage) {
      BytesArr<32> keyBytes;
      std::copy(key.bytes, key.bytes + 32, keyBytes.begin());
      std::cout << "Key: " << Hex::fromBytes(keyBytes) << std::endl;
      BytesArr<32> valOrigBytes;
      std::copy(val.original.bytes, val.original.bytes + 32, valOrigBytes.begin());
      std::cout << "ValOrig: " << Hex::fromBytes(valOrigBytes) << std::endl;
      BytesArr<32> valBytes;
      std::copy(val.current.bytes, val.current.bytes + 32, valBytes.begin());
      std::cout << "Val: " << Hex::fromBytes(valBytes) << std::endl;
    }
    /// We can actually commit the values (set orig = val, and val = empty)
    for (auto& [key, val] : val.storage) {
      std::memcpy(val.original.bytes, val.current.bytes, 32);
    }
  }
  /// Try to call the balanceOf based on the accounts[CONTRACT_ADDRESS].code
  ///auto getBalanceResult = evmc_execute(vm, &host.get_interface(), (evmc_host_context*)&host,
  ///                                     evmc_revision::EVMC_LATEST_STABLE_REVISION, &create_msg,
  ///                                     balanceOf.data(), balanceOf.size());
  // Let's test if we call call balanceOf of ERC20 from another contract... funny :)))

  Bytes getBalContractBytes = Hex::toBytes("608060405234801561000f575f80fd5b506102638061001d5f395ff3fe608060405234801561000f575f80fd5b5060043610610029575f3560e01c8063c7d85a561461002d575b5f80fd5b61004760048036038101906100429190610141565b61005d565b6040516100549190610197565b60405180910390f35b5f808390508073ffffffffffffffffffffffffffffffffffffffff166370a08231846040518263ffffffff1660e01b815260040161009b91906101bf565b602060405180830381865afa1580156100b6573d5f803e3d5ffd5b505050506040513d601f19601f820116820180604052508101906100da9190610202565b91505092915050565b5f80fd5b5f73ffffffffffffffffffffffffffffffffffffffff82169050919050565b5f610110826100e7565b9050919050565b61012081610106565b811461012a575f80fd5b50565b5f8135905061013b81610117565b92915050565b5f8060408385031215610157576101566100e3565b5b5f6101648582860161012d565b92505060206101758582860161012d565b9150509250929050565b5f819050919050565b6101918161017f565b82525050565b5f6020820190506101aa5f830184610188565b92915050565b6101b981610106565b82525050565b5f6020820190506101d25f8301846101b0565b92915050565b6101e18161017f565b81146101eb575f80fd5b50565b5f815190506101fc816101d8565b92915050565b5f60208284031215610217576102166100e3565b5b5f610224848285016101ee565b9150509291505056fea264697066735822122050f09470147bad20c270b0eac2cb6fdcab5c34d48f3f9bf67bb5be54798be20e64736f6c63430008180033");
  evmc_message getBalContractMsg{};
  getBalContractMsg.kind = evmc_call_kind::EVMC_CALL;
  getBalContractMsg.gas = GAS;
  getBalContractMsg.recipient = ANOTHER_CONTRACT_ADDRESS;
  getBalContractMsg.sender = CALLER_ADDRESS;

  auto getBalContractResult = evmc_execute(vm, &host.get_interface(), (evmc_host_context*)&host,
                                           evmc_revision::EVMC_LATEST_STABLE_REVISION, &getBalContractMsg,
                                           getBalContractBytes.data(), getBalContractBytes.size());

  std::cout << "getBalContractResult: " << getBalContractResult.status_code << std::endl;
  std::cout << "getBalContractResult.gas_left: " << getBalContractResult.gas_left << std::endl;
  std::cout << "getBalContractResult.gas used: " << GAS - getBalContractResult.gas_left << std::endl;
  // Commit the code to the account
  host.accounts[ANOTHER_CONTRACT_ADDRESS].code = std::basic_string<uint8_t>(getBalContractResult.output_data, getBalContractResult.output_size);

  // commit the state of the host to the host
  for (auto & [key, val] : host.accounts) {
    for (auto & [key, val] : val.storage) {
      std::memcpy(val.original.bytes, val.current.bytes, 32);
    }
  }
  // Try calling function getBal(address token, address user)

  Bytes getBalBytes = Hex::toBytes("0xc7d85a5600000000000000000000000020000000000000000000000000000000000000020000000000000000000000001000000000000000000000000000000000000001");

  evmc_message getBalMsg{};
  getBalMsg.kind = evmc_call_kind::EVMC_CALL;
  getBalMsg.gas = GAS;
  getBalMsg.recipient = ANOTHER_CONTRACT_ADDRESS;
  getBalMsg.sender = CALLER_ADDRESS;
  getBalMsg.code_address = ANOTHER_CONTRACT_ADDRESS;
  getBalMsg.input_data = getBalBytes.data();
  getBalMsg.input_size = getBalBytes.size();
  getBalMsg.gas = GAS;

  auto getBalResult = evmc_execute(vm, &host.get_interface(), (evmc_host_context*)&host,
                                   evmc_revision::EVMC_LATEST_STABLE_REVISION, &getBalMsg,
                                   host.accounts[ANOTHER_CONTRACT_ADDRESS].code.data(), host.accounts[ANOTHER_CONTRACT_ADDRESS].code.size());

  std::cout << "getBalResult: " << getBalResult.status_code << std::endl;
  std::cout << "getBalResult.gas_left: " << getBalResult.gas_left << std::endl;
  std::cout << "getBalResult.gas used: " << GAS - getBalResult.gas_left << std::endl;

  Bytes getBalResultData;
  std::copy(getBalResult.output_data, getBalResult.output_data + getBalResult.output_size, std::back_inserter(getBalResultData));
  std::cout << "getBalResultData: " << Hex::fromBytes(getBalResultData) << std::endl;


  // Try to make a transaction that sends a **value** to the contract
  evmc_message sendValueMsg{};
  sendValueMsg.kind = evmc_call_kind::EVMC_CALL;
  sendValueMsg.gas = GAS;
  sendValueMsg.recipient = ANOTHER_CALLED_ADDRESS;
  sendValueMsg.sender = CALLER_ADDRESS;
  sendValueMsg.gas = GAS;
  evmc_uint256be value = {};
  BytesArr<32> valueArr = Utils::uint256ToBytes(uint256_t(10000000000));
  std::copy(valueArr.begin(), valueArr.end(), value.bytes);
  sendValueMsg.value = value;
  // Give the caller some value
  host.accounts[CALLER_ADDRESS].set_balance(100000000000);

  auto sendValueResult = evmc_execute(vm, &host.get_interface(), (evmc_host_context*)&host,
                                      evmc_revision::EVMC_LATEST_STABLE_REVISION, &sendValueMsg,
                                      host.accounts[ANOTHER_CALLED_ADDRESS].code.data(), host.accounts[ANOTHER_CALLED_ADDRESS].code.size());

  std::cout << "sendValueResult: " << sendValueResult.status_code << std::endl;
  std::cout << "sendValueResult.gas_left: " << sendValueResult.gas_left << std::endl;
  std::cout << "sendValueResult.gas used: " << GAS - sendValueResult.gas_left << std::endl;


  Bytes randomAddressBytes = Utils::randBytes(20);
  Address randomAddress(randomAddressBytes);
  std::cout << "Random address: " << randomAddress.hex(true) << std::endl;
  evmc_address randomEvmcAddress = randomAddress.toEvmcAddress();
  Bytes evmcAddressBytes;
  std::copy(randomEvmcAddress.bytes, randomEvmcAddress.bytes + 20, std::back_inserter(evmcAddressBytes));
  std::cout << "Random address from evmc: " << Hex::fromBytes(evmcAddressBytes) << std::endl;
  Hash hash = Hash::random();
  std::cout << "Random hash: " << hash.hex(true) << std::endl;
  evmc_bytes32 evmcBytes32 = hash.toEvmcBytes32();
  Bytes evmcBytes32Bytes;
  std::copy(evmcBytes32.bytes, evmcBytes32.bytes + 32, std::back_inserter(evmcBytes32Bytes));
  std::cout << "Random hash from evmc: " << Hex::fromBytes(evmcBytes32Bytes) << std::endl;
  uint256_t uint256("123456789");
  std::cout << "uint256: " << uint256 << std::endl;
  evmc_uint256be evmcUint256 = Utils::uint256ToEvmcUint256(uint256);
  Bytes evmcUint256Bytes;
  std::copy(evmcUint256.bytes, evmcUint256.bytes + 32, std::back_inserter(evmcUint256Bytes));
  uint256_t uint256FromEvmc = Utils::bytesToUint256(evmcUint256Bytes);
  std::cout << "uint256 from evmc: " << uint256FromEvmc << std::endl;
  evmc_uint256be evmcUint256FromHash = Utils::bytesToEvmcUint256(hash.view());
  Bytes evmcUint256FromHashBytes;
  std::copy(evmcUint256FromHash.bytes, evmcUint256FromHash.bytes + 32, std::back_inserter(evmcUint256FromHashBytes));
  uint256_t uint256FromHash = Utils::bytesToUint256(evmcUint256FromHashBytes);
  std::cout << "uint256 from hash: " << uint256FromHash << std::endl;
  Hash hashFromEvmcUint256 = Utils::evmcUint256ToBytes(evmcUint256);
  std::cout << "hash from evmc uint256: " << hashFromEvmcUint256.hex(true) << std::endl;

  Bytes ecRecoverContract = Hex::toBytes("608060405234801561000f575f80fd5b506103018061001d5f395ff3fe608060405234801561000f575f80fd5b5060043610610029575f3560e01c8063c2bf17b01461002d575b5f80fd5b61004760048036038101906100429190610159565b61005d565b60405161005491906101fc565b60405180910390f35b5f6510000000000173ffffffffffffffffffffffffffffffffffffffff16633ee6ac9e868686866040518563ffffffff1660e01b81526004016100a39493929190610233565b602060405180830381865afa1580156100be573d5f803e3d5ffd5b505050506040513d601f19601f820116820180604052508101906100e291906102a0565b9050949350505050565b5f80fd5b5f819050919050565b610102816100f0565b811461010c575f80fd5b50565b5f8135905061011d816100f9565b92915050565b5f60ff82169050919050565b61013881610123565b8114610142575f80fd5b50565b5f813590506101538161012f565b92915050565b5f805f8060808587031215610171576101706100ec565b5b5f61017e8782880161010f565b945050602061018f87828801610145565b93505060406101a08782880161010f565b92505060606101b18782880161010f565b91505092959194509250565b5f73ffffffffffffffffffffffffffffffffffffffff82169050919050565b5f6101e6826101bd565b9050919050565b6101f6816101dc565b82525050565b5f60208201905061020f5f8301846101ed565b92915050565b61021e816100f0565b82525050565b61022d81610123565b82525050565b5f6080820190506102465f830187610215565b6102536020830186610224565b6102606040830185610215565b61026d6060830184610215565b95945050505050565b61027f816101dc565b8114610289575f80fd5b50565b5f8151905061029a81610276565b92915050565b5f602082840312156102b5576102b46100ec565b5b5f6102c28482850161028c565b9150509291505056fea26469706673582212203f4f7805af6b57b9d35d68dd078bb0f0c7b58d555f7b06c057f515a6511ba83064736f6c63430008180033");

  evmc_message ecRecoverMsg{};
  ecRecoverMsg.kind = evmc_call_kind::EVMC_CALL;
  ecRecoverMsg.gas = GAS;
  ecRecoverMsg.recipient = ECRECOVER_CONTRACT_ADDRESS;
  ecRecoverMsg.sender = CALLER_ADDRESS;

  auto ecRecoverResult = evmc_execute(vm, &host.get_interface(), (evmc_host_context*)&host,
                                      evmc_revision::EVMC_LATEST_STABLE_REVISION, &ecRecoverMsg,
                                      ecRecoverContract.data(), ecRecoverContract.size());

  std::cout << "ecRecoverResult: " << ecRecoverResult.status_code << std::endl;
  std::cout << "ecRecoverResult.gas_left: " << ecRecoverResult.gas_left << std::endl;
  std::cout << "ecRecoverResult.gas used: " << GAS - ecRecoverResult.gas_left << std::endl;

  // Put the code into the account
  host.accounts[ECRECOVER_CONTRACT_ADDRESS].code = std::basic_string<uint8_t>(ecRecoverResult.output_data, ecRecoverResult.output_size);


  Bytes ecRecoverTryExecuting = Hex::toBytes("c2bf17b0383e876328a5e13335ee2f4baf85782293e849f67552e06dc0923e2dfd2e9186000000000000000000000000000000000000000000000000000000000000001c7188aba1ebd9a97617005e20170d7195b1790dc284f33efe21038ac5545b6c666beb1faee29f69929b8d7f122a525889b8dd685f9957dff29f4894927b849da7");

  evmc_message ecRecoverTryExecutingMsg{};
  ecRecoverTryExecutingMsg.kind = evmc_call_kind::EVMC_CALL;
  ecRecoverTryExecutingMsg.gas = GAS;
  ecRecoverTryExecutingMsg.recipient = ECRECOVER_CONTRACT_ADDRESS;
  ecRecoverTryExecutingMsg.sender = CALLER_ADDRESS;
  ecRecoverTryExecutingMsg.code_address = ECRECOVER_CONTRACT_ADDRESS;
  ecRecoverTryExecutingMsg.input_data = ecRecoverTryExecuting.data();
  ecRecoverTryExecutingMsg.input_size = ecRecoverTryExecuting.size();

  auto ecRecoverTryExecutingResult = evmc_execute(vm, &host.get_interface(), (evmc_host_context*)&host,
                                                  evmc_revision::EVMC_LATEST_STABLE_REVISION, &ecRecoverTryExecutingMsg,
                                                  host.accounts[ECRECOVER_CONTRACT_ADDRESS].code.data(), host.accounts[ECRECOVER_CONTRACT_ADDRESS].code.size());

  std::cout << "ecRecoverTryExecutingResult: " << ecRecoverTryExecutingResult.status_code << std::endl;
  std::cout << "ecRecoverTryExecutingResult.gas_left: " << ecRecoverTryExecutingResult.gas_left << std::endl;
  std::cout << "ecRecoverTryExecutingResult.gas used: " << GAS - ecRecoverTryExecutingResult.gas_left << std::endl;
  Bytes outputData;
  std::copy(ecRecoverTryExecutingResult.output_data, ecRecoverTryExecutingResult.output_data + ecRecoverTryExecutingResult.output_size, std::back_inserter(outputData));
  std::cout << "ecRecoverTryExecutingResult.output_data: " << Hex::fromBytes(outputData) << std::endl;

  return 0;
  Utils::logToCout = true;
  std::string blockchainPath = std::filesystem::current_path().string() + std::string("/blockchain");
  blockchain = std::make_unique<Blockchain>(blockchainPath);
  // Start the blockchain syncing engine.
  std::signal(SIGINT, signalHandler);
  std::signal(SIGHUP, signalHandler);
  blockchain->start();
  std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::hours(std::numeric_limits<int>::max()));
  return 0;
}

