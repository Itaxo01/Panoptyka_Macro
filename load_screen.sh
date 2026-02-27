#!/bin/bash
# filepath: /home/kauanfank/codes/Panoptyka_Macro/start.sh


Xephyr -br -ac -noreset -screen 1280x720 :1 &
XEPHYR_PID=$!

killall steam
sleep 5

export DISPLAY=:1

steam steam://rungameid/3882730 &
STEAM_PID=$!
