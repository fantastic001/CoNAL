#!/bin/sh

# Use CONAL_MASTER_HOSTNAME=xxx.yyy.zzz.www to tell CoNAL environment that it is running in slave mode 
# Use CONAL_CLIENT_NAME to set proper client name on server

if [ -z "$CONAL_MASTER_HOSTNAME" ]; then 
    CONAL_MASTER_HOSTNAME=""
    CONAL_CLIENT_NAME=""
fi
CONAL_TEMP_DIR=/tmp/conal/

export CONAL_CLIENT_NAME
export CONAL_MASTER_HOSTNAME

component_autostart() {
    for component in $CONAL_DIR/components/*; do 
        if [ -e "$component/autostart.sh" ]; then 
            component_name=$(basename $component)
            echo "Starting $component_name"
            . $component/autostart.sh
        fi
    done
}

component_autostop() {
    for component in $CONAL_DIR/components/*; do 
        if [ -e "$component/autostart.sh" ]; then 
            component_name=$(basename $component)
            echo "Stopping $component_name"
            stop_component $component_name
        fi
    done

}

start_component() {
    local component
    local component_msg_fifo
    local component_state_dir
    component=$1
    component_state_dir=$CONAL_TEMP_DIR/state/$component
    if [ -f $component_state_dir/$component.pid ]; then 
        echo "Component already running"
        return 
    fi
    # clear old directory structure 
    mkdir -p $CONAL_DIR/log
    rm -rf $component_state_dir $CONAL_TEMP_DIR/comm/$component
    mkdir -p $component_state_dir
    mkdir -p $CONAL_TEMP_DIR/comm/$component
    component_msg_fifo=$CONAL_TEMP_DIR/comm/$component/messages
    mkfifo $component_msg_fifo
    COMPONENT_COMM_DIR=$CONAL_TEMP_DIR/comm/ COMPONENT_MSG_FIFO=$component_msg_fifo LD_LIBRARY_PATH=$CONAL_DIR/lib/:$LD_LIBRARY_PATH $CONAL_DIR/components/$component/bin/$component &
    echo $! > $component_state_dir/$component.pid
}

stop_component() {
    local component
    local component_msg_fifo
    local component_state_dir
    component=$1
    component_state_dir=$CONAL_TEMP_DIR/state/$component
    component_msg_fifo=$CONAL_TEMP_DIR/comm/$component/messages
    rm $component_msg_fifo
    kill $(cat $component_state_dir/$component.pid)
    rm -rf $component_state_dir/$component.pid
}

send_message() {
    local component
    local message 
    local performative
    component=$1
    performative=$2 
    message="$3"
    echo "0 $performative console $component $message" > $CONAL_TEMP_DIR/comm/$component/messages
}

start_task() {
    send_message activity_manager CREATE "$*"
}

create_data() {
    local ___id
    local selection
    local specification 
    ___id=$1
    selection="$2"
    specification="$3"
    send_message data_manager CREATE "$___id=$specification@$selection"
}

request() {
    local component
    local message 
    component=$1
    message="$2"
    send_message $component REQUEST "$message"
}

send_to_slaves() {
    local selection 
    local comm
    selection="$1"
    comm="$2"
    request activity_manager "send $selection#$comm"
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

    export CONAL_DIR

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
