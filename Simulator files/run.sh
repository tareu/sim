#!/bin/bash

# this runs the simulation on so many graphs

# the arguments are as follows:

# 1. graph size
# 2. selection intensity
# 3. benefit
# 4. cost
# 5. graph type
# 6. cutting method
# 7. number of simulations to run with these settings
# 8. name of new directory to put graphs into
# 9. float from 0.0 to 1.0 indicating probability of links
# 10. output. 1 is full output (csvs), 0 is 'quiet'; just ratio and turns
# 11. cutting type, 0 is random, 1 is pick a node with negative payoff
# an example of an instruction to create ten graphs in a folder called "ten_graphs"
# sh run.sh 100 0.01 1 1 random random 10 ten_graphs 0.5 1

if [ "$#" -ne 11 ]; then
    echo "arguments != 9"
    echo "# 1. graph size"
    echo "# 2. selection intensity"
    echo "# 3. benefit"
    echo "# 4. cost"
    echo "# 5. graph type"
    echo "# 6. cutting method"
    echo "# 7. number of simulations to run with these settings"
    echo "# 8. name of new directory to put graphs into"
    echo "# 9. float from 0.0 to 1.0 indicating probability of links"
    echo "# 10. output. 1 is full output (csvs), 0 is 'quiet'; just ratio and turns"
    echo "# 11. cutting type, 0 is random, 1 is pick a node with negative payoff"
    printf '%s\n' "specify arguments" >&2
    exit 1
fi
mkdir -p $8
if [ $? -eq 0 ]
then 
    if ./a.out $1 $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11}
    then
        exit 1
    fi
else
    printf '%s\n' "making directory failed" >&2
    exit 1 
fi
