#!/bin/bash

THIS=$(readlink -f $0)
THISDIR=$(dirname $THIS)

VERSION=1.0

remove_dangling() {
    docker rmi -f $(docker images -f "dangling=true" -q)
}



docker build $THISDIR/.. -f $THISDIR/Dockerfile -t conal:$VERSION

docker tag conal:$VERSION conal:latest