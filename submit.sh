#!/bin/bash

# default instance parameters
instance=""

# default job parameters
out=""
err="/dev/null"
log="/dev/null"
time="1:00:00"
cpus="4"
memory="8GB"
priority=0

args=""

while [[ $# > 0 ]]
do
    key="$1"
    case $key in
        -f|--file)
            shift
            instance="$1"
            shift
        ;;
        --output)
            shift
            out="$1"
            shift
        ;;
        --error)
            shift
            err="$1"
            shift
        ;;
        --time)
            shift
            time="$1"
            shift
        ;;
        --cpus)
            shift
            cpus="$1"
            shift
        ;;
        --memory)
            shift
            memory="$1"
            shift
        ;;
        -p|--priority)
            shift
            priority="$1"
            shift
        ;;
        *)
            args="$args$key "
            shift
        ;;
    esac
done

if [ -z "$instance" ]
then
    echo "Required instance option (-f) missing!"
    exit 1
fi

if [ ! -f "$instance" ]
then
    echo "$instance does not exists!"
    exit 1
fi

if [ -z "$out" ]
then
    out=${instance##*/}.out
fi

if hash condor_submit 2>/dev/null
then

home="/lhome/ext/iiia021/iiia0211"
root="$home/induced-width"
exe="$root/induced-width"

tmpfile=$(mktemp)
condor_submit 1> $tmpfile <<EOF
universe = vanilla
stream_output = True
stream_error = True
executable = $exe
arguments = -f $instance $args
log = $log
output = $out
error = $err
getenv = true
priority = $priority
queue
EOF

elif hash sbatch 2>/dev/null
then

tmpfile=$(mktemp)
sbatch 1> $tmpfile <<EOF
#!/bin/bash
#SBATCH --job-name=iw-${instance##*/}
#SBATCH --time=$time
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=$cpus
#SBATCH --mem=$memory
#SBATCH --output=$out
#SBATCH --error=$err
echo srun $LUSTRE/induced-width/induced-width -f $instance $args
srun $LUSTRE/induced-width/induced-width -f $instance $args
EOF

else
echo "Unknown cluster"
fi
