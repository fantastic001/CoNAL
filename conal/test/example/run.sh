#!/bin/bash

./exampleIntegrationTest 

grep "Registering component activity_manager" log/ComponentManager.log && \
    grep "Server initialized" log/activity_manager.log 

TEST_RESULT=$?