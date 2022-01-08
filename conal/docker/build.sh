#!/bin/bash

THIS=$(readlink -f $0)
THISDIR=$(dirname $THIS)

VERSION=1.0

remove_dangling() {
    docker rmi -f $(docker images -f "dangling=true" -q)
}
IMAGE_NAME=fantastic001/conal

docker build $THISDIR/.. -f $THISDIR/Dockerfile -t $IMAGE_NAME:$VERSION

docker tag $IMAGE_NAME:$VERSION $IMAGE_NAME:latest


if [ $1 = "--push" ]; then 
    docker push $IMAGE_NAME:$VERSION
    docker push $IMAGE_NAME:latest
fi 