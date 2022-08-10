#!/usr/bin/env bash

set -o errexit
set -o nounset
set -o pipefail


LOGFILE=($(pwd)/AIO-setup.log)
SUBNETOOORD=($(dirname $(pwd))/build/subnetooord)
# Build subnetooord.

cd ../build/
cmake .. && make -j$(nproc)

# Avalanchego root folder
AVALANCHE_ROOT_PATH="$HOME/go/src/github.com/ava-labs"

mkdir -p $AVALANCHE_ROOT_PATH
cd $AVALANCHE_ROOT_PATH

# Clone latest Avalanchego
git clone https://github.com/ava-labs/avalanchego


# Build latest AvalancheGo
cd avalanchego
chmod +x scripts/build.sh
./scripts/build.sh


# Setup 5 local nodes script files.

echo -n "./build/avalanchego --public-ip=127.0.0.1 --http-port=9650 --staking-port=9651 --db-dir=db/node1 --network-id=local --staking-tls-cert-file=$(pwd)/staking/local/staker1.crt --staking-tls-key-file=$(pwd)/staking/local/staker1.key" >> start1.sh
echo "./build/avalanchego --public-ip=127.0.0.1 --http-port=9652 --staking-port=9653 --db-dir=db/node2 --network-id=local --bootstrap-ips=127.0.0.1:9651 --bootstrap-ids=NodeID-7Xhw2mDxuDS44j42TCB6U5579esbSt3Lg --staking-tls-cert-file=$(pwd)/staking/local/staker2.crt --staking-tls-key-file=$(pwd)/staking/local/staker2.key" >> start2.sh
echo "./build/avalanchego --public-ip=127.0.0.1 --http-port=9654 --staking-port=9655 --db-dir=db/node3 --network-id=local --bootstrap-ips=127.0.0.1:9651 --bootstrap-ids=NodeID-7Xhw2mDxuDS44j42TCB6U5579esbSt3Lg --staking-tls-cert-file=$(pwd)/staking/local/staker3.crt --staking-tls-key-file=$(pwd)/staking/local/staker3.key" >> start3.sh
echo "./build/avalanchego --public-ip=127.0.0.1 --http-port=9656 --staking-port=9657 --db-dir=db/node4 --network-id=local --bootstrap-ips=127.0.0.1:9651 --bootstrap-ids=NodeID-7Xhw2mDxuDS44j42TCB6U5579esbSt3Lg --staking-tls-cert-file=$(pwd)/staking/local/staker4.crt --staking-tls-key-file=$(pwd)/staking/local/staker4.key" >> start4.sh
echo "./build/avalanchego --public-ip=127.0.0.1 --http-port=9658 --staking-port=9659 --db-dir=db/node5 --network-id=local --bootstrap-ips=127.0.0.1:9651 --bootstrap-ids=NodeID-7Xhw2mDxuDS44j42TCB6U5579esbSt3Lg --staking-tls-cert-file=$(pwd)/staking/local/staker5.crt --staking-tls-key-file=$(pwd)/staking/local/staker5.key" >> start5.sh
chmod +x start1.sh start2.sh start3.sh start4.sh start5.sh

# Start 5 local nodes using tmux.

tmux new-session -d -s avalanchego-1 "./start1.sh"
tmux new-session -d -s avalanchego-2 "./start2.sh"
tmux new-session -d -s avalanchego-3 "./start3.sh"
tmux new-session -d -s avalanchego-4 "./start4.sh"
tmux new-session -d -s avalanchego-5 "./start5.sh"

# Wait 30 seconds for bootstrap

sleep 30

# Create the user.


USERNAME=$(openssl rand -base64 16)
PASSWORD=$(openssl rand -base64 32)

echo "USERNAME: $USERNAME --- PASSWORD: $PASSWORD" >> $LOGFILE


SETUP_USER_OUTPUT=$(curl --location --request POST '127.0.0.1:9650/ext/keystore' \
                                    --header 'Content-Type: application/json' \
                                    --data-raw '{
                                      "jsonrpc": "2.0",
                                      "id"     : 1,
                                      "method" : "keystore.createUser",
                                      "params" : {
                                        "username": "'$USERNAME'",
                                        "password": "'$PASSWORD'"
                                      }
                                    }')

# Fund network.

FUND_NETWORK_OUTPUT=$(curl --location --request POST '127.0.0.1:9650/ext/bc/P' \
                                      --header 'Content-Type: application/json' \
                                      --data-raw '{
                                        "jsonrpc": "2.0",
                                        "id"     : 1,
                                        "method" : "platform.importKey",
                                        "params" : {
                                          "username"  : "'$USERNAME'",
                                          "password"  : "'$PASSWORD'",
                                          "privateKey": "PrivateKey-ewoqjP7PxY4yr3iLTpLisriqt94hdyDFNgchSxGGztUrTXtNN"
                                        }
                                      }')

FUNDING_ADDRESS=$(echo $FUND_NETWORK_OUTPUT | jq '.result.address' | sed 's/^"//' | sed 's/"$//')

echo "Funding Address: " $FUNDING_ADDRESS >> $LOGFILE


# Create Validator Addresses

CREATE_ADDRESS_1_OUTPUT=$(curl -X POST --data '{
                                                "jsonrpc": "2.0",
                                                "method" : "platform.createAddress",
                                                "params" : {
                                                  "username": "'$USERNAME'",
                                                  "password": "'$PASSWORD'"
                                                  },
                                                "id": 1
                                                }' -H 'content-type:application/json;' 127.0.0.1:9650/ext/P)


CREATE_ADDRESS_2_OUTPUT=$(curl -X POST --data '{
                                                "jsonrpc": "2.0",
                                                "method" : "platform.createAddress",
                                                "params" : {
                                                  "username": "'$USERNAME'",
                                                  "password": "'$PASSWORD'"
                                                  },
                                                "id": 1
                                                }' -H 'content-type:application/json;' 127.0.0.1:9650/ext/P)      


VALIDATOR_ADDRESS_1=$(echo $CREATE_ADDRESS_1_OUTPUT | jq '.result.address' | sed 's/^"//' | sed 's/"$//')
VALIDATOR_ADDRESS_2=$(echo $CREATE_ADDRESS_2_OUTPUT | jq '.result.address' | sed 's/^"//' | sed 's/"$//')

echo "VALIDATOR 1 ADDRESS: " $VALIDATOR_ADDRESS_1 >> $LOGFILE
echo "VALIDATOR 2 ADDRESS: " $VALIDATOR_ADDRESS_2 >> $LOGFILE

# Create Subnet

CREATE_SUBNET_OUTPUT=$(curl -X POST --data '{
                                            "jsonrpc": "2.0",
                                            "method" : "platform.createSubnet",
                                            "params" : {
                                              "controlKeys": [
                                                "'$VALIDATOR_ADDRESS_1'",
                                                "'$VALIDATOR_ADDRESS_2'"
                                              ],
                                              "threshold": 2,
                                              "username" : "'$USERNAME'",
                                              "password" : "'$PASSWORD'"
                                              },
                                            "id": 1
                                        }' -H 'content-type:application/json;' 127.0.0.1:9650/ext/P)

SUBNET_ID=$(echo $CREATE_SUBNET_OUTPUT | jq '.result.txID' | sed 's/^"//' | sed 's/"$//')


echo "SUBNET ID: " $SUBNET_ID >> $LOGFILE


# Get Node ID

NODE_ID_OUTPUT=$(curl -X POST --data '{
                                        "jsonrpc": "2.0",
                                        "id"     : 1,
                                        "method" : "info.getNodeID"
                                      }' -H 'content-type:application/json;' 127.0.0.1:9650/ext/info)

NODE_ID=$(echo $NODE_ID_OUTPUT | jq '.result.nodeID' | sed 's/^"//' | sed 's/"$//')


# Place subnetooord into plugins folder using subnet ID as filename.

cp $SUBNETOOORD $AVALANCHE_ROOT_PATH/avalanchego/build/plugins/$SUBNET_ID


# Stop AvalancheGo-1 and update script to include subnet.

while [ -n "$(tmux ls | grep -i "avalanchego-1")" ]
do
  tmux send-keys -t avalanchego-1 C-c
  sleep 1
done

echo " --whitelisted-subnets="$SUBNET_ID >> $AVALANCHE_ROOT_PATH/avalanchego/start1.sh

# Start avalanchego-1 back again.

tmux new-session -d -s avalanchego-1 "./start1.sh"

sleep 10

# Add subnet validator

START_TIME=$(date --date="10 minutes" +%s)
END_TIME=$(date --date="10 days" +%s)

SUBNET_VALIDATOR_OUTPUT=$(curl -X POST --data '{
        "jsonrpc": "2.0",
        "method" : "platform.addSubnetValidator",
        "params" : {
            "nodeID"    : "'$NODE_ID'",
            "subnetID"  : "'$SUBNET_ID'",
            "startTime" : '$START_TIME',
            "endTime"   : '$END_TIME',
            "weight"    : 30,
            "changeAddr": "'$FUNDING_ADDRESS'",
            "username"  : "'$USERNAME'",
            "password"  : "'$PASSWORD'"
        },
        "id": 1
    }' -H 'content-type:application/json;' 127.0.0.1:9650/ext/P)

echo "SUBNET VALIDATOR OUTPUT: " $SUBNET_VALIDATOR_OUTPUT >> $LOGFILE
echo "Waiting 10 minutes to start subnet..."

sleep 630
# Create Subnet Blockchain.


CREATE_SUBNET_OUTPUT=$(curl -X POST --data '{
        "jsonrpc": "2.0",
        "method": "platform.createBlockchain",
        "params" : {
            "subnetID"   : "'$SUBNET_ID'",
            "vmID"       : "'$SUBNET_ID'",
            "name"       : "Subnetooor",
            "genesisData": "0x68656c6c6f776f726c648f8f07af",
            "username"   : "'$USERNAME'",
            "password"   : "'$PASSWORD'"
        },
        "id": 1
    }' -H 'content-type:application/json;' 127.0.0.1:9650/ext/P)

echo "CREATE SUBNET OUTPUT: " $CREATE_SUBNET_OUTPUT >> $LOGFILE