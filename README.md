# CoNAL

## Setup

	mkdir build
	cd build
	cmake ../conal/ -DCMAKE_INSTALL_PREFIX=/opt/conal/
	make
	make install

## Running environment

	. /opt/conal/setup-env.sh /opt/conal/

## Using Docker image 

to run CoNAL master instance:
	
	docker run --rm -it --name conal_instance_master fantastixus/conal 
	
To run CoNAL in slave mode:

	docker run -it --rm --name conal_instance_client \
		-e CONAL_MASTER_HOSTNAME=masterHostname \
		-e CONAL_CLIENT_NAME=myClient \
		fantastixus/conal 

To run CoNAL with custom installation directory, just mount your installation to /opt/conal inside container:

	docker run -it --rm --name conal_instance_master -v conal/location/path:/opt/conal fantastixus/conal 

## Starting tasks

Tasks are started using `start_task` command in CoNAL prompt. For instance, to execute test task, in master node run:

	start_task TEST param1 param2 

On client side, you should see message "Hello World". 

# Listing all connected clients on master node

To list all clients, on master node run:

	request activity_manager list
