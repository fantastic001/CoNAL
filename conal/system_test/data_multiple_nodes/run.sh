#!/bin/bash 
# set -e 
CONAL_DIR=$1

DOCKER_MASTER_COMMAND="docker run --rm -it --name conal_instance_data_master -v $CONAL_DIR:/opt/conal/ conal"
DOCKER_SLAVE_COMMAND="docker run --rm -it --name conal_instance_slave -e CONAL_MASTER_HOSTNAME=172.17.0.2 -e CONAL_CLIENT_NAME=slave1 conal"

FIFO_FILE=/tmp/systemtest
MASTER_FIFO_FILE=$FIFO_FILE-master-$$
SLAVE_FIFO_FILE=$FIFO_FILE-slave-$$
PIDFILE=/tmp/conal-systemtest-pid-$$
LOG=/tmp/conal-systemtest-log-$$

empty -f -p $PIDFILE-master -i $MASTER_FIFO_FILE-input -o $MASTER_FIFO_FILE-output -L $LOG-master.log $DOCKER_MASTER_COMMAND
sleep 1
empty -f -p $PIDFILE-slave -i $SLAVE_FIFO_FILE-input -o $SLAVE_FIFO_FILE-output -L $LOG-slave.log $DOCKER_SLAVE_COMMAND
sleep 1

echo -e "#include <iostream>\nint main() {std::cout << \"HELLO\"; return 0; }" > $CONAL_DIR/test.cpp

echo "create_data \"x\" \"*\" \"DummySource()\"" > $MASTER_FIFO_FILE-input 
sleep 0.2
echo "variable y" > $MASTER_FIFO_FILE-input
echo "for ((i=0; i<10; i++)); do sleep 0.3; variable stefan_\$i haha_\$i; done" > $MASTER_FIFO_FILE-input
sleep 5
echo "cat log/data_manager.log" > $SLAVE_FIFO_FILE-input 
echo "request data_manager list" > $SLAVE_FIFO_FILE-input 
echo "request data_manager get x" > $SLAVE_FIFO_FILE-input 
echo "request data_manager end x" > $SLAVE_FIFO_FILE-input 
sleep 5
echo "client_request \"*\" data_manager list" >> $MASTER_FIFO_FILE-input
sleep 2
kill -9 $(cat $PIDFILE-*)
sleep 1
rm -rf $SLAVE_FIFO_FILE-* $MASTER_FIFO_FILE-* $PIDFILE-*

echo "Log for master"
cat $LOG-master.log

echo "Log for slave"
cat $LOG-slave.log 

cat $LOG-slave.log | grep "Created data instance: x" && \
    cat $LOG-slave.log | grep "x = DummySource()" && \
    cat $LOG-slave.log | grep "x -> TEST" && \
    cat $LOG-slave.log | grep "x finalized? yes" && \
    cat $LOG-slave.log | grep "y = Variable()" && \
    cat $LOG-slave.log | grep "stefan_5 = Variable(haha_5)" && \
    cat $LOG-slave.log | grep "stefan_9 = Variable(haha_9)" && \
    (cat $LOG-master.log | grep "From 172.17.0.3: " | grep stefan_1)

exit $?
