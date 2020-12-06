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

Tasks are started using `start_task` command in CoNAL prompt. For instance, to execute test task on all clients, in master node run:

	start_task "*" TEST param1 param2 

On client side, you should see message "Hello World". 

If you want to implement your own task code, you can start with this example:

```cpp
#include <vector>
#include <iostream>
#include <conal_utilities.hpp>

using namespace std; 
using namespace conal::code_manager;
using namespace conal::utilities;

int start(params& in, params& out) {
	cout << "Hello from loaded program\n";
	// check if there is some input data
	if (in.size() > 0) {
		wait_for_data(in[0]);
		cout << "Value changed\n";
	}
	return 0; 
}
```

To start task on all clients:

	start_task "*" mycode.cpp

or, for instance, to start task only on client with ip 192.168.0.100:

	start_task "name=192.168.0.100" mycode.cpp

Now, you can see that our task code requires some data to work with, you can define data in various ways:

* having shared variable across all clients (value will be same in the beginning)
* have dummy variable

For creating simple variables you can simply type:

	variable myVarName myvalue

and variable will be created on all clients. 

# Listing all connected clients on master node

To list all clients, on master node run:

	request activity_manager list "*"

Here "*" means "all clients". This parameter can be some other filter, please refer to architecture document for more details. 

