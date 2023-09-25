#ifndef EVENT_H
#define EVENT_H

#include <algorithm>
#include <string>

#include "../utils/db.h"
#include "../utils/strings.h"
#include "../utils/utils.h"
#include "abi.h"

// TODO: remember to generate the ABI for events later on
// TODO: probably implement eth_getFilterChanges/eth_getLogs/etc. when done?
// TODO: add tests when done
// TODO: update docs when done

/// Abstraction of a Solidity event.
class Event {
  private:
    std::string name_;            ///< Event name.
    uint64_t logIndex_;           ///< Position of the event inside the block it was emitted from.
    Hash txHash_;                 ///< Hash of the transaction that emitted the event.
    uint64_t txIndex_;            ///< Position of the transaction inside the block the event was emitted from.
    Hash blockHash_;              ///< Hash of the block that emitted the event.
    uint64_t blockIndex_;         ///< Height of the block that the event was emitted from.
    Address address_;             ///< Address that emitted the event.
    Bytes data_;                  ///< Non-indexed arguments of the event.
    std::vector<Bytes> topics_;   ///< Indexed arguments of the event, limited to a max of 3 (4 for anonymous events).
    bool anonymous_;              ///< Whether the event is anonymous or not (its signature is indexed and searchable).

    /**
     * Encode an indexed parameter for topic storage, as specified here:
     * https://docs.soliditylang.org/en/develop/abi-spec.html#events
     * https://docs.soliditylang.org/en/develop/abi-spec.html#indexed-event-encoding
     * @param param The parameter to encode.
     * @param type The type of the parameter.
     * @return The topic-encoded parameter.
     */
    Bytes encodeTopicParam(BaseTypes& param, ABI::Types type);

  public:
    /**
     * Constructor.
     * @param name The event's name.
     * @param logIndex The event's position on the block.
     * @param txHash The hash of the transaction that emitted the event.
     * @param txIndex The position of the transaction in the block.
     * @param blockHash The hash of the block that emitted the event.
     * @param blockIndex The height of the block.
     * @param address The address that emitted the event.
     * @param params The event's arguments. Defaults to none (empty list).
     * @param anonymous Whether the event is anonymous or not. Defaults to false.
     */
    Event(
      const std::string& name, uint64_t logIndex, Hash txHash, uint64_t txIndex,
      Hash blockHash, uint256_t blockIndex, Address address,
      std::vector<std::pair<BaseTypes, bool>> params = {}, bool anonymous = false
    );

    /// Getter for `name_`.
    std::string getName() const { return this->name_; }

    /// Getter for `logIndex_`.
    uint64_t getLogIndex() const { return this->logIndex_; }

    /// Getter for `txHash_`.
    Hash getTxHash() const { return this->txHash_; }

    /// Getter for `txIndex_`.
    uint64_t getTxIndex() const { return this->txIndex_; }

    /// Getter for `blockHash_`.
    Hash getBlockHash() const { return this->blockHash_; }

    /// Getter for `blockIndex_`.
    uint256_t getBlockIndex() const { return this->blockIndex_; }

    /// Getter for `address_`.
    Address getAddress() const { return this->address_; }

    /// Getter for `data_`.
    Bytes getData() const { return this->data_; }

    /// Getter for `topics_`.
    std::vector<Bytes> getTopics() const { return this->topics_; }

    /// Getter for `anonymous_`.
    bool isAnonymous() const { return this->anonymous_; }

    /**
     * Get the event's selector (keccak hash of its signature).
     * @return topics[0] when non-anonymous, empty bytes when anonymous.
     */
    Bytes getSelector() const {
      if (!this->anonymous_) return this->topics_[0]; else return Bytes();
    }
};

/**
 * Class that holds all events emitted by contracts in the blockchain.
 * Responsible for registering, managing and saving/loading events to/from the database.
 */
class EventManager {
  private:
    // TODO: keep up to 1000 events in memory, dump older ones to DB (maybe this should be a deque?)
    std::vector<Event> events_;         ///< List of all events in memory.
    const std::unique_ptr<DB>& db_;     ///< Reference pointer to the database.

  public:
    EventManager(const std::unique_ptr<DB>& db) : db_(db) {
      ; // TODO: load events from DB
    }

    ~EventManager() {
      ; // TODO: save events to DB
    }

    // TODO: maybe a periodicSaveToDB() just like on Storage?

    /**
     * Register the event in memory.
     * Keep in mind the original Event object is MOVED to the list.
     * @param event The event to register.
     */
    void registerEvent(Event& event) {
      this->events_.push_back(std::move(event));
    }
};

#endif  // EVENT_H