#!/bin/bash


mkdir -p components/ping/bin components/pong/bin
mv ping components/ping/bin/
mv pong components/pong/bin/

echo "Starting pingpong test"

start_component pong
sleep 1
start_component ping
sleep 5
cat log/ping.log
cat log/pong.log
grep "TEST PASSED" log/ping.log 
TEST_RESULT=$?

stop_component ping 
stop_component pong