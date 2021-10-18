#!/bin/bash
source g_set_env.sh
cd mind_applications/mind_linux/test_programs/07_linked_list
pwd

make_cmd="default"
echo "Make cmd: ${make_cmd}"
make $make_cmd
./bin/api_test
