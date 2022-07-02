#include "functions.h"
#include <unistd.h>

int main(int argc, char *argv[]) 
{
    char *ptr;
    long output;
    long graphSize;
    double selectionIntensity;
    long benefitPayoff;
    long costPayoff;
    char *graphType;
    char *cuttingMethod; 
    long numberOfSimulations; 
    char *newDir;
    float pForErdosRenyi;
    int cuttingType;
    if (argc == 12)
    {
//        printf("\n\n input = %s, output = %ld\n\n", argv[10], strtol(argv[10], &ptr, 10));
        output = strtol(argv[10], &ptr, 10);
        graphSize = strtol(argv[1], &ptr, 10);
        selectionIntensity = strtod(argv[2], &ptr);
        benefitPayoff = strtol(argv[3], &ptr, 10);
        costPayoff = strtol(argv[4], &ptr, 10);
        graphType = argv[5];
        cuttingMethod = argv[6]; 
        numberOfSimulations  = strtol(argv[7], &ptr, 10); 
        newDir = argv[8];
        pForErdosRenyi = strtod(argv[9], &ptr);
        cuttingType = (int) strtol(argv[11], (char **)NULL, 10);
        chdir(newDir);
 //       printf("output = %ld\ngraphSize = %ld\nselectionIntensity = %f\nbenefitPayoff = %ld\ncostPayoff = %ld\ngraphType = %s\ncuttingMethod = %s\nnumberOfSimulations = %ld\npForErdosRenyi = %f\n", output, graphSize, selectionIntensity, benefitPayoff, costPayoff, graphType, cuttingMethod, numberOfSimulations, pForErdosRenyi);
    }
    

    else
    {
        printf("%d arguments specified\n", argc);
        return -1;
    }
    int graphNumber;
    int i;
    for (i = 0; i < numberOfSimulations; i++) 
    {
        graphNumber = i;
        struct Graph *g = generateGraph(pForErdosRenyi, graphNumber, graphSize, selectionIntensity, benefitPayoff, costPayoff);

        /*  this is for doing a graph manually
            int i, j;
            for (i = 0; i < 3; i++)
            {
                for (j = 0; j < 3; j++)
                {
                    int location[] = {i,j};
                    setArrayValue(__LINE__, g->adjMat, location, 1);
                }
                int location1[] = {i};
                if (i < 1)
                {
                    setArrayValue(__LINE__, g->coopMat, location1, 0);
                }
                else
                {
                    setArrayValue(__LINE__, g->coopMat, location1, 1);
                }
            }
        */

        simulateGraph(g, output, cuttingType);

        // this is for printing the fixation result of this simulated graph
        //printf("%dc, %dd\n", g->cooperators, g->defectors);
    }
    return 0;
}

/*
    to do:

    - graph input (to test algebra)
        -done (need to properly check)
    - cutting ties
        -done (need to get lots of types, easy on/off
    - .csv, image, gif output
        -graphviz not really good enough... need something newer and better..
        -https://igraph.org ?

*/
