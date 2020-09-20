#!/bin/sh

component_autostart() {
    for component in $CONAL_DIR/components/*; do 
        if [ -e "$component/autostart.sh" ]; then 
            component_name=$(basename $component)
            echo "Starting $component_name"
            $component/autostart.sh
        fi
    done
}

platform_capabilities_discovery() {
    export CONAL_HOST_PLATFORM="$(uname -m)"
}

if [ "$CONAL_INITIALIZED" != "1" ]; then 

    CONAL_DIR="."
    export CONAL_INITIALIZED=1

    if [ ! -z "$1" ]; then 
        CONAL_DIR="$1"
    fi

    for component in $CONAL_DIR/components/*; do 
        if [ -d "$component" ]; then 
            export PATH="$component/bin:$PATH"
        fi
    done

    platform_capabilities_discovery

    for env_script in $CONAL_DIR/env/*; do
        echo "Soucing $env_script"
        . $env_script
    done

    component_autostart

else 
    echo "CoNAL environment is already initialized"

fi
