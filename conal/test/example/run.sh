#!/bin/bash

grep "Registering component activity_manager" log/ComponentManager.log

exit $?