#!/bin/sh

# there are two arguments at the moment:
# 1. print files or just ratio and step count (1 or 0)
# 2. cutting type:
#   0 random cutting
#   1 cut link of zero or negative payoff node

sh run.sh $1 $2 $3 $4 $5 $6 $7 $8 $9 {$10} {$11}
if [ $1 -eq 1 ]
then
    rscript visgraph0.r $8
    GRAPHCOUNT=$(ls ./$8 | wc -l)
    echo $GRAPHCOUNT
    counter=0
    rm *.gif
    while [ $counter -lt $GRAPHCOUNT ]
    do
        ffmpeg -f image2 -framerate 8 -i "./$8/graph_$counter/step_%d.jpg" "out$counter.gif"
        ((counter++))
    done
fi
exit 0
