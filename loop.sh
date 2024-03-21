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
        *)
            args="$args$key "
            shift
        ;;
    esac
done

j=0
mkdir -p $directory

for instance in "$@"
do
    if [[ $j == $step ]]
    then
        wait_empty_queue
        j=0
    fi
    echo "Submitted $instance"
    ./slurm_submit.sh --file $instance --output $directory/${instance##*/}.out
    j=$((j+1))
done
