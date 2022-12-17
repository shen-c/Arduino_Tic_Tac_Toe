#!/bin/bash
MQTTSUB="C:/Program Files/mosquitto/mosquitto_sub.exe"
while true
do
    "$MQTTSUB" -t "tictactoe" | while read -r payload
    do
        if [[ "$payload" == 1 ]]; then
            echo 1 > /dev/ttyACM0
            break
        elif [[ "$payload" == 2 ]]; then
            echo 2 > /dev/ttyACM0
        fi
    done
done
