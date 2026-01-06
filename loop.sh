#!/bin/bash

function wait_empty_queue {
    while true
    do
        if hash condor_submit 2>/dev/null
        then
            remaining=$( condor_q | tail -n 3 | head -n 1 | cut -f 4 -d\  )
        elif hash sbatch 2>/dev/null
        then
            remaining=$( squeue --me -h | wc -l )
        else
            echo "Unknown cluster"
            exit 1
        fi
        echo "$remaining jobs remaining"
        if [[ $remaining == 0 ]]
        then
            return
        else
            echo "Waiting queue to be empty..."
            sleep 60
        fi
    done
}

directory="log"
step=100
extra=""

while [[ $# > 0 ]]
do
    key="$1"
    case $key in
        --dir)
            shift
            directory="$1"
            shift
        ;;
        --step)
            shift
            step="$1"
            shift
        ;;
        -*)
            shift
            extra="$extra$key $1 "
            shift
            ;;
        *)
            instances="$instances$key "
            shift
        ;;
    esac
done

i=0
j=0
n=$( echo $instances | wc -w )
mkdir -p $directory

for instance in $instances
do
    if [[ $j == $step ]]
    then
        wait_empty_queue
        j=0
    fi
    i=$((i+1))
    echo "Submitted $instance ($i\$n) with arguments: $extra"
    basename=${instance##*/}
    ./submit.sh --file $instance --output $directory/$basename.out --log $directory/$basename.log --error $directory/$basename.err -j $directory/$basename.json $extra
    j=$((j+1))
done
