#!/bin/bash

echo "User input number of module: $1" 

echo "Spawn Fan control module clients..."

for ((i = 0; i < $1; i++))
do
    echo "Spawn Client $i"
    ./fan_control_client &
done

echo "Spawn Fan control server program..."
./fan_control_server $1 

