#!/bin/bash

# default instance parameters
instance=""

# default job parameters
out=""
err="/dev/null"
time="1:00:00"
cpus="4"
memory="8GB"

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

if hash sbatch 2>/dev/null
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
#SBATCH --mail-type=begin
#SBATCH --mail-type=end
#SBATCH --mail-type=fail
#SBATCH --mail-user=email@domain
echo srun $LUSTRE/induced-width/induced-width -f $instance $args
srun $LUSTRE/induced-width/induced-width -f $instance $args
EOF

else
echo "Unknown cluster"
fi
