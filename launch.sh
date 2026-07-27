#! /bin/bash
tmux -L devstudio -f ./devtmux.conf new-session -s GQREDUX -n Devstudio ./bin/devstudio.grg 
#./bin/devstudio.grg