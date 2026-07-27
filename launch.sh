#! /bin/bash
tmux -L devstudio -f ./devtmux.conf new-session -A -s GQREDUX -n Devstudio ./bin/devstudio.grg 
