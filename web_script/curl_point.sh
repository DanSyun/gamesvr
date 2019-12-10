#!/bin/bash

uid=$1
type=$2
id=$3
num=$4
reason=$5

curl -m 1 http://127.0.0.1:30/userAward -X POST -d "uid=$uid&type=$type&id=$id&num=$num&reason=$reason" >> /dev/null 2>&1

