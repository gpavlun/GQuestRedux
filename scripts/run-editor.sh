#! /bin/bash
tmux kill-window -t Editor 2>/dev/null
tmux -L devstudio new-window -n Editor ./bin/editor.grg

