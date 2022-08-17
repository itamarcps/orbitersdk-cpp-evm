#ifndef CHAINHEAD_H
#define CHAINHEAD_H

#include "block.h"
#include "db.h"
#include "utils.h"

class ChainHead {
  private:
    std::shared_ptr<DBService> &dbServer;
    std::deque<std::shared_ptr<Block>> internalChainHead;
    std::unordered_map<std::string,std::shared_ptr<Block>> lookupBlockByHash;
    std::unordered_map<std::string,std::shared_ptr<Block>> lookupBlockByTxHash;
    std::unordered_map<std::string,std::shared_ptr<Tx::Base>> lookupTxByHash;
    std::unordered_map<std::string,uint64_t> lookupBlockHeightByHash;
    std::unordered_map<uint64_t,std::string> lookupBlockHashByHeight;
    std::mutex internalChainHeadLock;
    bool hasBlock(std::string &blockHash);
    bool hasBlock(uint64_t &blockHeight);
    // Only access these functions if you are absolute sure that internalChainHeadLock is locked.
    void _push_back(Block& block);
    void _push_front(Block& block);


  public:
    ChainHead(std::shared_ptr<DBService> &_dbService);
    // Mutex locked.
    void push_back(Block& block);
    void push_front(Block& block);
    void pop_back();
    void pop_front();
    bool exists(std::string &blockHash);
    bool exists(uint64_t &blockHeight);
    Block getBlock(std::string &blockHash);
    Block getBlock(uint64_t &blockHeight);
    bool hasTransaction(std::string &txHash);
    Tx::Base getTransaction(std::string &txHash);
    Block getBlockFromTx(std::string &txHash);
    Block latest();
    uint64_t blockSize();
    void loadFromDB();
    void dumpToDB();
    void periodicSaveToDB();  // TODO: implement this
};

#endif  // CHAINHEAD_H