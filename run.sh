#!/bin/bash

cd ./build/Debug/output/

valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=unittest-common.log ./unittest-common

valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=unittest-logger.log ./unittest-logger

valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=multitest-logger.log ./multitest-logger

valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=benchtest-logger.log ./benchtest-logger
