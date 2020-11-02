#!/bin/bash

! grep -i error log/code_manager.log && \
    grep "Loading loader at" log/code_manager.log

TEST_RESULT=$?