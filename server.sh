#!/bin/bash

deploy_file=./server_deploy.txt
max_try=30

function usage
{
    echo -e "\033[32m./$(basename $0) [start|stop|restart|reload] [all|machinesvr|websvr|...]\033[0m"
    echo -e "\033[32m./$(basename $0) [status|check|clearshm]\033[0m"
}

function main
{
    sudo sysctl -w kernel.shmmax=4000000000 >> /dev/null 2>&1
    ulimit -s 102400
    ulimit -c unlimited

    case $1 in
        start)      shift; _start       ${@};;
        stop)       shift; _stop        ${@};;
        restart)    shift; _restart     ${@};;
        reload)     shift; _reload      ${@};;
        status)     _status;;
        check)      _check;;
        clearshm)   _clearshm;;
        *)          usage;;
    esac
}

function _start
{
    [ -z $1 ] && usage && return 1

    case $1 in
        all)
            cat $deploy_file | while read _deploy; do
                if [ ! -z "${_deploy}" ] && [ "${_deploy:0:1}" != "#" ]; then
                    __start ${_deploy}
                fi
            done;;

        *)
            cat $deploy_file | grep "\./$1" | while read _deploy; do
                if [ "${_deploy:0:1}" != "#" ]; then
                    __start ${_deploy}
                fi
            done;;
    esac
}
function _stop
{
    [ -z $1 ] && usage && return 1

    case $1 in
        all)
            cat $deploy_file | while read _deploy; do
                if [ ! -z "${_deploy}" ] && [ "${_deploy:0:1}" != "#" ]; then
                    __stop ${_deploy}
                fi
            done;;

        *)
            cat $deploy_file | grep "\./$1" | while read _deploy; do
                if [ "${_deploy:0:1}" != "#" ]; then
                    __stop ${_deploy}
                fi
            done;;
    esac
}
function _restart
{
    [ -z $1 ] && usage && return 1

    case $1 in
        all)
            cat $deploy_file | while read _deploy; do
                if [ ! -z "${_deploy}" ] && [ "${_deploy:0:1}" != "#" ]; then
                    __restart ${_deploy}
                fi
            done;;

        *)
            cat $deploy_file | grep "\./$1" | while read _deploy; do
                if [ "${_deploy:0:1}" != "#" ]; then
                    __restart ${_deploy}
                fi
            done;;
    esac
}
function _reload
{
    [ -z $1 ] && usage && return 1

    case $1 in
        all)
            cat $deploy_file | while read _deploy; do
                if [ ! -z "${_deploy}" ] && [ "${_deploy:0:1}" != "#" ]; then
                    __reload ${_deploy}
                fi
            done;;

        *)
            cat $deploy_file | grep "\./$1" | while read _deploy; do
                if [ "${_deploy:0:1}" != "#" ]; then
                    __reload ${_deploy}
                fi
            done;;
    esac
}
function _status
{
    cat $deploy_file | while read _deploy; do
        if [ ! -z "${_deploy}" ] && [ "${_deploy:0:1}" != "#" ]; then
            if is_alive $_deploy; then
                echo -e "\033[32m$_deploy is running\033[0m"
            else
                echo -e "\033[31m$_deploy is not running\033[0m"
            fi
        fi
    done
}
function _check
{
    cat $deploy_file | while read _deploy; do
        if [ ! -z "${_deploy}" ] && [ "${_deploy:0:1}" != "#" ]; then
            if ! is_alive $_deploy; then
                echo -e "\033[31m$_deploy is not running\033[0m"
                __start $_deploy
            else
                echo -e "\033[32m$_deploy is running\033[0m"
            fi
        fi
    done
}
function _clearshm
{
    for _id in $(ipcs -m | awk '{ if ($6 == 0) { print $2 } }'); do
        sudo ipcrm -m $_id
    done

    sudo ipcs -m
}


#########################################
function __start
{
    cd ./svr/$(basename ${1})/bin
    ! is_alive ${@} && eval ${@}

    for ((count = 1; count <= $max_try; count++)); do
        sleep 0.3
        is_alive ${@} && break
        [ $(($count % 10)) -eq 0 ] && echo -e "\033[31m${@} start retry $(($count/ 10)) time\033[0m" && eval ${@}
    done

    cd ->>/dev/null 2>&1

    if [ ! $count -gt $max_try ]; then
        echo -e "\033[32m${@} start successed\033[0m"
        return 0
    else
        echo -e "\033[31m${@} start failed\033[0m"
        return 1
    fi
}

function __stop
{
    pkill -SIGRTMIN -fx "${*}"

    for ((count = 1; count <= $max_try; count++)); do
        sleep 0.3
        ! is_alive ${@} && break
        [ $(($count % 10)) -eq 0 ] && echo -e "\033[31m${@} stop retry $(($count / 10)) time\033[0m" && pkill -SIGRTMIN -fx "${*}"
    done

    if [ ! $count -gt $max_try ]; then
        echo -e "\033[32m${@} stop successed\033[0m"
        return 0
    else
        echo -e "\033[31m${@} stop failed\033[0m"
        return 1
    fi
}

function __restart
{
    ! __stop ${@} && return 1
    ! __start ${@} && return 1
}

function __reload
{
    pkill -SIGRTMIN+1 -fx "${*}"

    if [ $? -eq 0 ]; then
        echo -e "\033[32m${@} reload successed\033[0m"
    else
        echo -e "\033[31m${@} reload failed\033[0m"
    fi
}


#########################################
function is_alive
{
    pgrep -fx "${*}" >> /dev/null 2>&1 && return 0 || return 1
}

main ${@}

