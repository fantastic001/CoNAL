#!/bin/bash


THIS_DIR=$(readlink -f $(dirname $0))
CONAL_DIR=$1

cd $THIS_DIR
FAILED_TESTS=""
ALL_TESTS=""
for module in $(ls .); do 
    if [ -d $module ]; then 
        ALL_TESTS="$ALL_TESTS $module"
        ./run.sh $module $CONAL_DIR
        if [ $? -ne 0 ]; then 
            FAILED_TESTS="$FAILED_TESTS $module"
        fi
    fi
done

echo "-------------------------------- TEST RESULT"
echo "Run $(echo $ALL_TESTS | wc -w) tests"
echo "Failed tests: $FAILED_TESTS"

