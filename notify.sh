#!/bin/bash
while true #run indefinitely
do 
#inotifywait -r -e modify,attrib,close_write,move,create,delete ./source && make game
inotifywait -r -e close_write ./source && make game

done
