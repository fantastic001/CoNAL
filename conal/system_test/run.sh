#!/bin/bash 

THIS_DIR=$(readlink -f $(dirname $0))
CONAL_DIR=$2
MODULE_DIR="$THIS_DIR/$1"
MODULE_NAME=$1

$MODULE_DIR/run.sh $CONAL_DIR
exit $?