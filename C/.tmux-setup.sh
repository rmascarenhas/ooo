project=`basename "$PWD"`

tmux has-session -t "$project" 2>/dev/null

if [ $? -ne 0 ]; then
    # Editor
    tmux new-session -s "$project" -n editor -d
    tmux send-keys -t "$project":1.1 "vim allok.c" C-m
    tmux split-window -h -t "$project":1
    tmux select-layout -t "$project":1 main-vertical
fi

tmux attach -t "$project"
