#!/bin/bash

sleep 1
request code_manager "load sample param1 param2"

cat log/code_manager.log

! grep -i error log/code_manager.log && \
    grep "Loading loader at" log/code_manager.log && \
    grep "No suitable loader for sample" log/code_manager.log

TEST_RESULT=$?