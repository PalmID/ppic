
#! /bin/bash

CurDir="$(cd "$(dirname "${BASH_SOURCE[0]}" )" && pwd)"

#####################################################################
# mysql environments, you can configure it according you own needs. #
#####################################################################
# set data dir
MYSQL_DATA=/data/mysql/data
MYSQL_LOG=/data/mysql/logs
if [ -z ${MYSQL_ROOT_PASSWORD} ]; then
    MYSQL_ROOT_PASSWORD=root123
fi
if [ -z ${MYSQL_DATABASE} ]; then
    MYSQL_DATABASE=ppic_test
fi
MYSQL_IMAGE=mysql:8.0
MYSQL_CONTAINER_NAME=mysql
if [ -z ${MYSQL_CONNECTION_URL} ]; then
    MYSQL_CONNECTION_URL=root:${MYSQL_ROOT_PASSWORD}@${MYSQL_CONTAINER_NAME}:33060
fi

if [ -z ${PLATFORM} ]; then
    PLATFORM=local
fi

########################################################################
# ppic app environments, you can configure it according you own needs. #
########################################################################
PPIC_CI_IMAGE=registry.cn-hangzhou.aliyuncs.com/leosocy/ppic:ci
PPIC_CONTAINER_NAME=ppic
BUILD_DIR=build
APP_EXE=./ppic_app
APP_TEST_NAME=ppic_test
PPIC_DOCKER_NETWORK=ppic-net

###########################
# Google cpplint settings #
###########################
CPP_LINT_IMAGE=registry.cn-hangzhou.aliyuncs.com/leosocy/cpplint
CPP_LINT_CONTAINER_NAME=cpplint

error_return() {
    echo "[ERROR] $1"
    exit 1
}

check_exec_success() {
    if [ "$1" != "0" ]; then
        echo "[ERROR] $2 failed!"
        exit 1
    else
        echo "[INFO] $2 success!"
    fi
}

updateimages() {
    docker pull ${MYSQL_IMAGE}
    docekr pull ${PPIC_CI_IMAGE}
}

is_mysql_running() {
    docker inspect ${MYSQL_CONTAINER_NAME} 1>/dev/null 2>&1
    return $?
}

is_docker_network_exist() {
    nn=$(docker network ls -f name=$1 | tail -n 1  | awk '{print $2}')
    if [ "${nn}" = "$1" ]; then
        return 1
    fi
    return 0
}

startmysql() {
    if [ "${PLATFORM}" != "local" ]; then
        error_return "You can only start mysql container in **local**!"
    fi
    docker pull ${MYSQL_IMAGE}
    check_exec_success "$?" "pulling mysql image"

    docker kill ${MYSQL_CONTAINER_NAME} 1>/dev/null 2>&1
    docker rm -v ${MYSQL_CONTAINER_NAME} 1>/dev/null 2>&1
    docker network create ${PPIC_DOCKER_NETWORK}
    docker run -d --rm --name ${MYSQL_CONTAINER_NAME} \
        --network ${PPIC_DOCKER_NETWORK} \
        -v ${CurDir}/configs/mysql/conf:/etc/mysql/conf.d \
        -v ${MYSQL_DATA}:/var/lib/mysql \
        -v ${MYSQL_LOG}:/var/log/mysql \
        -e MYSQL_ROOT_PASSWORD=${MYSQL_ROOT_PASSWORD} \
        -e MYSQL_DATABASE=${MYSQL_DATABASE} \
        --log-opt max-size=10m \
        --log-opt max-file=9 \
        ${MYSQL_IMAGE}
    check_exec_success "$?" "start mysql container"
}

reloadmysql() {
    destroymysql
    startmysql
}

restartmysql() {
    stopmysql
    startmysql
}

stopmysql() {
    docker stop ${MYSQL_CONTAINER_NAME} 1>/dev/null 2>&1
    docker rm -v ${MYSQL_CONTAINER_NAME} 1>/dev/null 2>&1
    docker network rm ${PPIC_DOCKER_NETWORK}
}

destroymysql() {
    stopmysql
    rm -rf ${MYSQL_DATA}
    rm -rf ${MYSQL_LOG}
    check_exec_success "$?" "destroy mysql container"
}

createdb() {
    docker exec ${MYSQL_CONTAINER_NAME} mysql -uroot -p${MYSQL_ROOT_PASSWORD} \
     -e "CREATE DATABASE IF NOT EXISTS $1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_unicode_ci;" 1>/dev/null 2>&1
    check_exec_success "$?" "create database $1"
}

dropdb() {
    docker exec ${MYSQL_CONTAINER_NAME} mysql -uroot -p${MYSQL_ROOT_PASSWORD} \
     -e "DROP DATABASE IF EXISTS $1" 1>/dev/null 2>&1
    check_exec_success "$?" "drop database $1"
}

need_start_services() {
    if [ "${PLATFORM}" = "local" ]; then
        is_mysql_running
        if [ "$?" != "0" ]; then
            return 1
        fi
    fi
    return 0
}
start_services() {
    need_start_services
    if [ $? = 1 ]; then
        startmysql
    fi
}

startapp() {
    start_services
    is_docker_network_exist ${PPIC_DOCKER_NETWORK}
    if [ $? = 0 ]; then
        restartmysql
    fi
    # start the rpc server
    docker stop ${PPIC_CONTAINER_NAME} 2>/dev/null
    docker rm -v ${PPIC_CONTAINER_NAME} 2>/dev/null
    docker run -it --rm --name ${PPIC_CONTAINER_NAME} --network ${PPIC_DOCKER_NETWORK} \
        -v ${CurDir}:/home/ppic -w /home/ppic \
        -e MYSQL_CONNECTION_URL=${MYSQL_CONNECTION_URL} \
        -e MYSQL_DATABASE=${MYSQL_DATABASE} \
        ${PPIC_CI_IMAGE} sh -c " \
            rm -rf ${BUILD_DIR} && mkdir -p ${BUILD_DIR} && cd ${BUILD_DIR} \
            && cmake ../app && make -j && ${APP_EXE} \
        "
}

gdbapp() {
    start_services
    is_docker_network_exist ${PPIC_DOCKER_NETWORK}
    if [ $? = 0 ]; then
        restartmysql
    fi
    # start the rpc server
    docker stop ${PPIC_CONTAINER_NAME} 2>/dev/null
    docker rm -v ${PPIC_CONTAINER_NAME} 2>/dev/null
    docker run -it --rm --name ${PPIC_CONTAINER_NAME} --network ${PPIC_DOCKER_NETWORK} \
        --cap-add=SYS_PTRACE --security-opt seccomp=unconfined \
        -v ${CurDir}:/home/ppic -w /home/ppic \
        -e MYSQL_CONNECTION_URL=${MYSQL_CONNECTION_URL} \
        -e MYSQL_DATABASE=${MYSQL_DATABASE} \
        ${PPIC_CI_IMAGE} sh -c " \
            rm -rf ${BUILD_DIR} && mkdir -p ${BUILD_DIR} && cd ${BUILD_DIR} \
            && cmake ../app && make -j && gdb ${APP_EXE} \
        "
}

runtest() {
    start_services
    is_docker_network_exist ${PPIC_DOCKER_NETWORK}
    if [ $? = 0 ]; then
        restartmysql
    fi
    dropdb ${MYSQL_DATABASE}
    docker stop ${PPIC_CONTAINER_NAME} 2>/dev/null
    docker rm -v ${PPIC_CONTAINER_NAME} 2>/dev/null
    docker run -it --rm --name ${PPIC_CONTAINER_NAME} --network ${PPIC_DOCKER_NETWORK} \
        -v ${CurDir}:/home/ppic -w /home/ppic \
        -e MYSQL_CONNECTION_URL=${MYSQL_CONNECTION_URL} \
        -e MYSQL_DATABASE=${MYSQL_DATABASE} \
        ${PPIC_CI_IMAGE} sh -c " \
            mkdir -p ${BUILD_DIR} && cd ${BUILD_DIR} \
            && cmake ../tests && make -j build_and_test
        "
}

gdbtest() {
    start_services
    is_docker_network_exist ${PPIC_DOCKER_NETWORK}
    if [ $? = 0 ]; then
        restartmysql
    fi
    dropdb ${MYSQL_DATABASE}
    docker stop ${PPIC_CONTAINER_NAME} 2>/dev/null
    docker rm -v ${PPIC_CONTAINER_NAME} 2>/dev/null
    docker run -it --rm --name ${PPIC_CONTAINER_NAME} --network ${PPIC_DOCKER_NETWORK} \
        --cap-add=SYS_PTRACE --security-opt seccomp=unconfined \
        -v ${CurDir}:/home/ppic -w /home/ppic \
        -e MYSQL_CONNECTION_URL=${MYSQL_CONNECTION_URL} \
        -e MYSQL_DATABASE=${MYSQL_DATABASE} \
        ${PPIC_CI_IMAGE} sh -c " \
            mkdir -p ${BUILD_DIR} && cd ${BUILD_DIR} \
            && cmake ../tests && make -j && gdb ${APP_TEST_NAME}
        "
}

cpplint() {
    docker stop ${CPP_LINT_CONTAINER_NAME} 2>/dev/null
    docker rm -v ${CPP_LINT_CONTAINER_NAME} 2>/dev/null
    docker run -it --rm --name ${CPP_LINT_CONTAINER_NAME} \
        -v ${CurDir}:/home/ppic -w /home \
        ${CPP_LINT_IMAGE} sh -c " \
            cpplint --linelength=120 --extensions=h,cpp,cc,c \
            \$(find ppic/app -name *.h -or -name *.c -or -name *.cc -or -name *.cpp)
        "
}

################
# script start #
################

case "$1" in
    startapp) startapp ;;
    gdbapp) gdbapp ;;
    runtest) runtest ;;
    gdbtest) gdbtest ;;
    cpplint) cpplint ;;
    createdb) createdb $2 ;;
    dropdb) dropdb $2 ;;
    startmysql) startmysql ;;
    stopmysql) stopmysql ;;
    reloadmysql) reloadmysql ;;
    restartmysql) restartmysql ;;
    destroymysql) destroymysql ;;
    updateimages) updateimages ;;
    *)
        echo "Usage:"
        echo "./manage.sh startapp|gdbapp"
        echo "./manage.sh runtest|gdbtest"
        echo "./manage.sh cpplint"
        echo "./manage.sh createdb|dropdb"
        echo "./manage.sh startmysql|stopmysql|destroymysql"
        echo "./manage.sh restartmysql(stop+start)|reloadmysql(destroy+start)"
        echo "./manage.sh updateimages"
        exit 1
        ;;
esac

exit 0
