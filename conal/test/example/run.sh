#!/bin/bash

./exampleIntegrationTest 


grep "Server initialized" log/activity_manager.log 

TEST_RESULT=$?