#!/bin/bash
source g_set_env.sh
cd ${MIND_PATH}/mind_linux/test_programs/07_linked_list
mkdir -p $LOG_DIR
pwd

make_cmd="linked_list"
echo "Make cmd: ${make_cmd}"
make $make_cmd
sleep 10
cp logs_03a_sharing_ratio/* ${LOG_DIR}/.
cd ${MIND_PATH}/mind_linux/util_modules
make
sudo rmmod pprint.ko
sudo insmod pprint.ko
sleep 5
tail -n 20 /var/log/kern.log > ${LOG_DIR}/kern.node_$1_of_$4_sr_$2_rw_$3.log
