#!/bin/bash

PROJECT_DIR=$(cd $(dirname $0); pwd)
BUILD_DIR=${PROJECT_DIR}/build
[[ -d ${BUILD_DIR} ]] || mkdir -p ${BUILD_DIR}

cmake -S ${PROJECT_DIR} -B ${BUILD_DIR} -DCMAKE_BUILD_TYPE=Debug
cmake --build ${BUILD_DIR} -j $(nproc)

${BUILD_DIR}/glkit_app