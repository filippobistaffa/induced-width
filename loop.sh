#!/bin/bash

function wait_empty_queue {
    while true
    do
        if hash condor_submit 2>/dev/null
        then
            remaining=$( condor_q | tail -n 3 | head -n 1 | cut -f 4 -d\  )
        elif hash sbatch 2>/dev/null
        then
            remaining=$( squeue --me -h | grep quick | wc -l )
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

step=200
j=0

for instance in "$@"
do
    if [[ $j == $step ]]
    then
        wait_empty_queue
        j=0
    fi
    echo "Submitted $instance"
    ./submit_slurm.sh -f $instance
    j=$((j+1))
done
