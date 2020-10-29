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
	
		docker run -it --name conal_instance_master fantastixus/conal 
	
	To run CoNAL in slave mode:

		docker run -it --name conal_instance_master \
			-e CONAL_MASTER_HOSTNAME=masterHostname \
			-e CONAL_CLIENT_NAME=myClient \
			fantastixus/conal 
	
	To run CoNAL with custom installation directory, just mount your installation to /opt/conal inside container:

		docker run -it --name conal_instance_master -v conal/location/path:/opt/conal fantastixus/conal 
