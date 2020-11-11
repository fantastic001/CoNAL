#!/bin/bash

THIS=$(readlink -f $0)
THISDIR=$(dirname $THIS)

remove_dangling() {
    docker rmi -f $(docker images -f "dangling=true" -q)
}

docker build $THISDIR/.. -f $THISDIR/Dockerfile -t conal:1.0

