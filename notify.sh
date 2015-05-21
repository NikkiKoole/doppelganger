#!/bin/bash
while true #run indefinitely
do
inotifywait -qq -r -e close_write ./source && make libgame
done
