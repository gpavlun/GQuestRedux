#! /bin/bash
tmux kill-window -t Game 2>/dev/null
tmux -L devstudio new-window -n Game ./bin/game.grg
