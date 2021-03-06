#!/bin/bash
# Copyright 2022 Huawei Technologies Co., Ltd
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# ============================================================================

if [ $# -lt 3 ]
then
    echo "Usage: bash run_train_gpu.sh [DATA_URL] [VISIABLE_DEVICES(0,1,2,3,4,5,6,7)] [DEVICE_NUM]"
exit 1
fi

if [ $3 -lt 1 ] && [ $3 -gt 8 ]
then
    echo "error: DEVICE_NUM=$3 is not in (1-8)"
exit 1
fi

DATA_URL=$1

export DEVICE_NUM=$3
export RANK_SIZE=$3

BASEPATH=$(cd "`dirname $0`" || exit; pwd)
export PYTHONPATH=${BASEPATH}:$PYTHONPATH
if [ -d "./train" ];
then
    rm -rf ./train
fi
mkdir ./train
cd ./train || exit

export CUDA_VISIBLE_DEVICES="$2"

mpirun -n $1 --allow-run-as-root --output-filename log_output --merge-stderr-to-stdout \
python3 ${BASEPATH}/../train.py --run_distribute=True --device_target=GPU --data_url=$DATA_URL> train_gpu.log 2>&1 &

