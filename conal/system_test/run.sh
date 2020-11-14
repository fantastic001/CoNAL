#!/bin/bash 

THIS_DIR=$(readlink -f $(dirname $0))
CONAL_DIR=$2
MODULE_DIR="$THIS_DIR/$1"
MODULE_NAME=$1

TEMP_CONAL_DIR=/tmp/conal-systemtest-$(date +%s)

cp -r $CONAL_DIR $TEMP_CONAL_DIR

$MODULE_DIR/run.sh $TEMP_CONAL_DIR
exit $?