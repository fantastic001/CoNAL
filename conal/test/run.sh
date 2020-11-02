#!/bin/bash 

THIS_DIR=$(readlink -f $(dirname $0))
CONAL_DIR=$2
MODULE_DIR="$THIS_DIR/$1"
MODULE_NAME=$1


# copy conal directory to temporary directory 
TEMP_DIR=/tmp/conal-moduletest-$$
cp -r $CONAL_DIR $TEMP_DIR

echo "Copying configuration for moduletest $MODULE_NAME"
cp $MODULE_DIR/config/* $TEMP_DIR/env/

if [ -f $MODULE_DIR/src/CMakeLists.txt ]; then 
    mkdir $TEMP_DIR/build
    cd $TEMP_DIR/build
    cmake $MODULE_DIR/src/
    make 
    echo "Copying executables to temp directory"
    find . -maxdepth 1 -executable -type f -print -exec cp {} .. \;
fi

cd $TEMP_DIR

source setup-env.sh .
source $MODULE_DIR/run.sh

component_autostop

echo Removing $TEMP_DIR
rm -rf $TEMP_DIR

exit $TEST_RESULT