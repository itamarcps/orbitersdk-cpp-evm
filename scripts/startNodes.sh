#!/bin/bash


# Check if the user is running this script from the main directory of the project
if [ ! -f "scripts/AIO-setup.sh" ]; then
  echo "Please run this script from the main directory of the project"
  exit 1
fi


cd local_testnet
# Launch the Discovery Node through tmux
echo "Launching Discovery Node"
cd local_testnet_discovery
tmux new-session -d -s local_testnet_discovery './orbitersdkd-discovery || bash && bash'
sleep 1

# Launch the Validators through tmux, don't exit the tmux session when closing the terminal
echo "Launching Validator 1"
cd ../local_testnet_validator1
tmux new-session -d -s local_testnet_validator1 './orbitersdkd || bash && bash'

echo "Launching Validator 2"
cd ../local_testnet_validator2
tmux new-session -d -s local_testnet_validator2 './orbitersdkd || bash && bash'

echo "Launching Validator 3"
cd ../local_testnet_validator3
tmux new-session -d -s local_testnet_validator3 './orbitersdkd || bash && bash'

echo "Launching Validator 4"
cd ../local_testnet_validator4
tmux new-session -d -s local_testnet_validator4 './orbitersdkd || bash && bash'

echo "Launching Validator 5"
cd ../local_testnet_validator5
tmux new-session -d -s local_testnet_validator5 './orbitersdkd || bash && bash'

# Launch the Normal Nodes through tmux, don't exit the tmux session when closing the terminal
echo "Launching Normal Node 1"
cd ../local_testnet_normal1
tmux new-session -d -s local_testnet_normal1 './orbitersdkd || bash && bash'

echo "Launching Normal Node 2"
cd ../local_testnet_normal2
tmux new-session -d -s local_testnet_normal2 './orbitersdkd || bash && bash'

echo "Launching Normal Node 3"
cd ../local_testnet_normal3
tmux new-session -d -s local_testnet_normal3 './orbitersdkd || bash && bash'

echo "Launching Normal Node 4"
cd ../local_testnet_normal4
tmux new-session -d -s local_testnet_normal4 './orbitersdkd || bash && bash'

echo "Launching Normal Node 5"
cd ../local_testnet_normal5
tmux new-session -d -s local_testnet_normal5 './orbitersdkd || bash && bash'

echo "Launching Normal Node 6"
cd ../local_testnet_normal6
tmux new-session -d -s local_testnet_normal6 './orbitersdkd || bash && bash'

# Finish deploying
GREEN=$'\e[0;32m'
NC=$'\e[0m'
echo "${GREEN}Success! All nodes are launched${NC}"
echo "You can now access the local testnet through your favorite web3 client"
echo "RPC URL: http://127.0.0.1:8090"
echo "ChainID: 808080"
echo "You can also attach your terminal to the tmux session to see the logs related to transactions and blocks being processed."
echo "\"tmux a -t local_testnet_validator1\" to attach to the tmux session of the first validator"