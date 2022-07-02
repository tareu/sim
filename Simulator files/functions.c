#include "functions.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h> 


void printError(int errorCode, int line)
{
    if (errorCode == 0)
    {
        printf("Line %d - Array out of bounds\n", line);
    }
    if (errorCode == 1)
    {
        printf("Line %d - Cannot create array of size 0\n", line);
    }
}

struct Array createIntArray(int line, char* dimensions)
{
    int* sizes; 
    int i;
    int* p;
    int size = 0;
    int counter = 0;
    char inChar;
    //1 means 1 dimension
    int dimCount = 0; 
    char currentnumber[10];
    for ( i = 0; i<10; i++)
    {
        currentnumber[i] = 'x';
    }
    while (1)
    {
        inChar = *(dimensions + counter);
        if (inChar == '\0')
        {
            dimCount++;
            break;
        } 
        else if (inChar == ',')
        {
            dimCount++;
        } 
        counter++;
    } 
    sizes = malloc(dimCount * sizeof(int));
    for (i = 0; i < dimCount; i++)
    {
        *(sizes + i) = 0;
    }
    counter = 0;
    int anothercounter = 0;
    int numbCounter = 0;
    while (1)
    {
        inChar = *(dimensions + counter);
        if (inChar == '\0')
        {
            *(sizes + numbCounter) = atoi(currentnumber);
            break;
        } 
        else if (inChar == ',')
        {
            *(sizes + numbCounter) = atoi(currentnumber);
            numbCounter++;
            for (i = 0; i<10; i++)
            {
                currentnumber[i] = 'x';
            }
            anothercounter = 0;
        } 
        else if (isdigit(inChar)) 
        {
            currentnumber[anothercounter] = inChar;
            anothercounter++;
        }        
        counter++;
    } 
 
    for (i = 0; i < dimCount; i++)
    {
        if (i == 0)
        {
            size = *(sizes + 0);
        }
        else 
        {
            size = size * *(sizes + i);
        }
    }
    if (size == 0)
    {
        printError(1, line);
        struct Array newArray;
        return newArray;
    }
    p = malloc(size * sizeof(int));
    struct Array newArray;
    newArray.arrayHead = p;
    newArray.numDimensions = dimCount;
    newArray.sizes = sizes;    
    newArray.size = size;   
    return newArray;
}

int translateMultiTo1d(struct Array array, int* inputDim)
{
    int x = *(inputDim + 0);
    int oneDimLocation = x;
    int totalN = 1;
    int dimShift = 0;
    int i;
    for (i = 1; i < array.numDimensions; i++)
    {
        totalN = totalN * *(array.sizes + (i - 1));
        dimShift = *(inputDim + i) * totalN; 
        oneDimLocation = oneDimLocation + dimShift;
    }
    return oneDimLocation;
}

int getArrayValue(int line, struct Array array, int* inputDim)
{
    int i;
    for (i = 0; i < array.numDimensions; i++)
    {
        if (*(array.sizes + i) <= *(inputDim + i))
        {
            printError(0, line);
            return -99999999;
        }
    }
            
    int location = translateMultiTo1d(array, inputDim);
    return *(array.arrayHead + location);
}

void setArrayValue(int line, struct Array array, int* inputDim, int value)
{
    int i;
    for (i = 0; i < array.numDimensions; i++)
    {
        if (*(array.sizes + i) <= *(inputDim + i))
        {
            printError(0, line);
            return;
        }
    }
    int location = translateMultiTo1d(array, inputDim);
    *(array.arrayHead + location) = value;
}


void calculatePayoff(struct Graph *g)
{
    //uses:
    //adjMat
    //payoffMat
    //coopMat
    int i, j;
    for (i=0; i<g->size; i++)
    {
        int connectedCooperators = 0;
        int allConnected = 0; int payoff = 0; 
        /*
            for every node count how many nodes are connected to it
            and then out of those connected nodes count how many
            are cooperators. 
            
            then calculate the payoff. then print that out.
        */
        for (j=0; j<g->size; j++)
        {
            int location[] = {i,j};
            if (getArrayValue(__LINE__, g->adjMat, location) == 1)
            {
                int location2[] = {j};
                if (getArrayValue(__LINE__, g->coopMat, location2) == 0|getArrayValue(__LINE__, g->coopMat, location2) == 1)
                {
                    allConnected++;
                }
                if (getArrayValue(__LINE__, g->coopMat, location2) == 1)
                {
                    connectedCooperators++;
                }
            }
        }
        int location3[] = {i};
        if (getArrayValue(__LINE__, g->coopMat, location3) == 1)
        {
            payoff = (g->benefitPayoff * connectedCooperators) - (g->costPayoff * allConnected);
        }
        if (getArrayValue(__LINE__, g->coopMat, location3) == 0)
        {
            payoff = (g->benefitPayoff * connectedCooperators);
        }
        setArrayValue(__LINE__, g->payoffMat, location3, payoff);
    }
}

void createGraph(struct Graph *g)
{  
    /* 
        this section of the code creates one random non-directed
        graph with cooperator and non-cooperator status assigned
        randomly to each node
        
        it creates two matrixes, which have the information about
        the graph, adjMat and coopMat.
        uses:
        adjMat
        coopMat
        
    */
 
    int i,j;
 
    for (j=0; j<g->size; j++)
    {
        for(i=0; i<g->size; i++)
        {
            int location[] = {j, i};
            setArrayValue(__LINE__, g->adjMat, location, 0);
        }
    }
 
    /*
        for all nodes, there is a chance of 0.5 between them and
        another node.
    */
    for (i=0; i<g->size; i++)
    {
        for (j=0; j<g->size; j++)
        {
            if (i == j)
            {
                break;
            }
            //this generates 0 or 1 and so creates a 50% chance of it being either. we want a float that can be specified at the command line
            double x = (double)arc4random() / UINT32_MAX;
            if (x < g->pForErdosRenyi)
            {
                int location1[] = {i, j};
                int location2[] = {j, i};
                setArrayValue(__LINE__, g->adjMat, location1, 1);
                setArrayValue(__LINE__, g->adjMat, location2, 1);
            }
            else
            {
                int location1[] = {i, j};
                int location2[] = {j, i};
                setArrayValue(__LINE__, g->adjMat, location1, 0);
                setArrayValue(__LINE__, g->adjMat, location2, 0);
            }
        }
    }
    
    /*
        create an array that determines whether they are cooperators or
        non-cooperators then assign random cooperator/noncoop status to
        them.
    */
    int haveMixOfCAndD = 0;
    while(haveMixOfCAndD == 0)
    {
        for(i=0; i<g->size; i++)
        {
            int r = arc4random_uniform(2);
            int location3[] = {i};
            setArrayValue(__LINE__, g->coopMat, location3, r);
        }
        calculateRatio(g);
        if (g->cooperators > 0 && g->defectors > 0)
        {
            fflush(stdout);
            haveMixOfCAndD = 1;
        }
    }
}      

void depthFirstSearch(int currentNode, struct Graph *g)
{
    //FILE *fp;
    //char buffer[50];
    //snprintf(buffer, 50, "./dfscount.count");
    //printf("%s", buffer);
    //fp = fopen(buffer, "a");
    //fprintf(fp, "%d\n", currentNode + 1);

    int location[] = {currentNode}; 
    setArrayValue(__LINE__, g->visited, location, 1);
    int i, j; 
    for (i = 0; i < g->size; i++)
    {
        int location2[] = {i};
        int location3[] = {currentNode, i};
        
        int isIVisited = getArrayValue(__LINE__, g->visited, location2);
        int isCurrentNodeAndIConnected = getArrayValue(__LINE__, g->adjMat, location3);
        if (isIVisited == 0 && isCurrentNodeAndIConnected == 1 && currentNode != i)
        {
            depthFirstSearch(i, g);
        }
    }
}

void dfs(int startingNode, struct Graph *g)
{
    int i;
    for (i = 0; i < g->size; i++)
    {
        int location1[] = {i};
        setArrayValue(__LINE__, g->visited, location1, 0);
    }
    depthFirstSearch(startingNode, g);

    //FILE *fp;
    //char buffer[50];
    //snprintf(buffer, 50, "./dfscount.count");
    //fp = fopen(buffer, "a");
}

int supersetOfMST(struct Graph *g)
{
    dfs(0, g);
    int i;
    int count = 0;
    for (i=0; i<g->size; i++)
    {
        int location[] = {i};
        if (getArrayValue(__LINE__, g->visited, location) == 1)
        {
            count++;
        }
    }
 
    if (count == g->size) 
    {
        return 1;
    }
    return 0;
}

void chooseDeadNode(struct Graph *g)
{
    int randomNode = arc4random_uniform(g->size);
    int location[] = {randomNode};
    setArrayValue(__LINE__, g->coopMat, location, 2);
}

void calculateChangeProbability(struct Graph *g)
{
    //uses:
    //adjMat
    //coopMat
    //payoffMat
 
    int i;
    int j;
    double nodeFitness = 0;
    double coopFitness = 0;
    double noncoopFitness = 0;
    for(i=0; i<g->size; i++)
    {
        //if this node is the dead one
        int location[] = {i};
        if (getArrayValue(__LINE__, g->coopMat, location) == 2)
        {
            for (j=0; j<g->size; j++)
            {
                int location2[] = {i, j};
                int location3[] = {j};
                getArrayValue(__LINE__, g->adjMat, location2);
                //if this node is connected to the dead one
                if (getArrayValue(__LINE__, g->adjMat, location2) == 1)
                {
                    //get fitness of that connected node
                    nodeFitness =  pow((1.0 + g->selectionIntensity), getArrayValue(__LINE__, g->payoffMat, location3));
                    //printf("%d's nodeFitness = %f because pow((1.0 + %f), (%d))\n", j, nodeFitness, g->selectionIntensity, getArrayValue(__LINE__, g->payoffMat, location3));
                    //if coop, add to fitness of coop
                    if (getArrayValue(__LINE__, g->coopMat, location3) == 1)
                    {
                        //printf("which is a cooperator\n");
                        coopFitness = coopFitness + nodeFitness;
                    }
                    //if non-coop, add to fitness of non-coop
                    if (getArrayValue(__LINE__, g->coopMat, location3) == 0)
                    {
                        //printf("which is a noncooperator\n");
                        noncoopFitness = noncoopFitness + nodeFitness;
                    }
                }
            }
            g->changeProbability = coopFitness/(coopFitness+noncoopFitness);
            //printf("change probability of %d = %f because %f/(%f+%f)\n", i, g->changeProbability, coopFitness, coopFitness, noncoopFitness);
        }
    }
}

void makeDir(char name[])
{
    char path[100];
    snprintf(path, 100, "./%s", name); 
    mkdir(path, 0777);
}

void changeDir(char name[])
{
    char path[100];
    snprintf(path, 100, "%s", name); 
    chdir(path);
}

void printCoopStatusToFile(struct Graph *g)
{
    FILE *fp;
    char buffer[50];
    snprintf(buffer, 50, "./coopStatus%d.csv", g->step);
    fp = fopen(buffer, "w+");
    int i;
    for (i=0; i<g->size; i++)
    {
        int location[] = {i};
        fprintf(fp, "%d, %d\n", i + 1, getArrayValue(__LINE__, g->coopMat, location));
    }
    fclose(fp);
    //printf(" - Coop status saved to file.\n");
}

void printStepsToFile(struct Graph *g)
{
    char buffer[128];
    snprintf(buffer, sizeof(buffer), "graph_%lu", g->graphNumber);
    changeDir(buffer);
    FILE *fp;
    char bufferTwo[50];
    snprintf(bufferTwo, 50, "./steps.csv");
    fp = fopen(bufferTwo, "w+");
    fprintf(fp, "%d\n", g->step);
    fclose(fp);
    changeDir("..");
}

void printRatioToFile(struct Graph *g)
{
    char buffer[128];
    snprintf(buffer, sizeof(buffer), "graph_%lu", g->graphNumber);
    changeDir(buffer);
    FILE *fp;
    char bufferTwo[50];
    snprintf(bufferTwo, 50, "./ratio.csv");
    fp = fopen(bufferTwo, "w+");
    fprintf(fp, "%d, %d\n", g->cooperators, g->defectors);
    fclose(fp);
    changeDir("..");
}

void printPayoffToFile(struct Graph *g)
{
    FILE *fp;
    char buffer[50];
    snprintf(buffer, 50, "./payoffStatus%d.csv", g->step);
    fp = fopen(buffer, "w+");
    int i, j;
    for (i=0; i<g->size; i++)
    {
        int location[] = {i};
        fprintf(fp, "%d, %d\n", i + 1, getArrayValue(__LINE__, g->payoffMat, location));
    }
    fclose(fp);
    //printf(" - Payoffs saved to file.\n");
}

void printAdjMatToFile(struct Graph *g)
{
    FILE *fp;
    char buffer[50];
    snprintf(buffer, 50, "./adjStatus%d.csv", g->step);
    fp = fopen(buffer, "w+");
    int i, j;
    for (i = 0; i < g->size; i++)
    {
        for (j = 0; j < g->size; j++)
        {
            int location[] = {i, j};
            if (g->step == 0)
            {
            }
            if (i < j && getArrayValue(__LINE__, g->adjMat, location) == 1)
            {
                fprintf(fp, "%d %d\n", i + 1, j + 1);
                
            }
            else
            {
            }
        }
    }
    fclose(fp);
    //printf(" - Edges saved to file.\n");
}

void printAdjMat(struct Graph *g)
{
    int i, j;
    for (i=0; i<g->size; i++)
    {
        for (j=0; j<g->size; j++)
        {
            int location[] = {i,j};
            printf("%d ", getArrayValue(__LINE__, g->adjMat, location));
        }
        
        printf("\n");
    } 
} 

void applyChangeProbability(struct Graph *g)
{
    int i;
    for (i=0; i<g->size; i++)
    {
        int location[] = {i};
        if (getArrayValue(__LINE__, g->coopMat, location) == 2)
        {
            double r = ((double)arc4random() / UINT32_MAX);
            if (r < g->changeProbability)
            {
                //printf("changing %d to a cooperator because random(0,1) = %f and changeProb = %f\n", i, r, g->changeProbability);
                setArrayValue(__LINE__, g->coopMat, location, 1);
            }
            if (r >= g->changeProbability)
            {
                //printf("changing %d to a defector because random(0,1) = %f and changeProb = %f\n", i, r, g->changeProbability);
                setArrayValue(__LINE__, g->coopMat, location, 0);
            }
        }
    }
}

void calculateRatio(struct Graph *g)
{
    g->defectors = 0;
    g->cooperators = 0;
    int i;
    for(i = 0; i < g->size; i++)
    {
        int location[] = {i};
        if (getArrayValue(__LINE__, g->coopMat, location) == 0)
        {
            g->defectors++;
        }
        if (getArrayValue(__LINE__, g->coopMat, location) == 1)
        {
            g->cooperators++;
        }
    }
}

void removeConnection(int node1, int node2, struct Graph *g)
{
    int location[] = {node1, node2};
    int location1[] = {node2, node1};
    setArrayValue(__LINE__, g->adjMat, location, 0);
    setArrayValue(__LINE__, g->adjMat, location1, 0);
}

void createConnection(int node1, int node2, struct Graph *g)
{
    int location[] = {node1, node2};
    int location1[] = {node2, node1};
    setArrayValue(__LINE__, g->adjMat, location, 1);
    setArrayValue(__LINE__, g->adjMat, location1, 1);
}

int nodesConnected(int node1, int node2, struct Graph *g)
{
    int isConnected = 0;
    int location[] = {node1, node2};
    isConnected = getArrayValue(__LINE__, g->adjMat, location);
    return isConnected;
}

int countConnectionsOfNode(int node, struct Graph *g)
{
    int i;
    int count = 0;
    for (i = 0; i < g->size; i++)
    {
        if (nodesConnected(node, i, g))
        {
            count++;
        }
    }
    return count;
}



void removeARRFromLowPayoffNode(struct Graph *g)
{
    
    fflush(stdout);
    //Node 1 will lose a connection and gain a new random one
    int node1 = arc4random_uniform(g->size);
    int node2 = arc4random_uniform(g->size);
    int node1Location[] = {node1};
    int superset = 0;
    while(superset == 0)
    {
        while(getArrayValue(__LINE__, g->payoffMat, node1Location) > 0 || countConnectionsOfNode(node2, g) == 1 || countConnectionsOfNode(node1, g) == (g->size - 1) || nodesConnected(node1, node2, g) == 0 || node1 == node2)
        { 
            node1 = arc4random_uniform(g->size);
            node2 = arc4random_uniform(g->size);
            node1Location[0] = node1;
        }
        removeConnection(node1, node2, g);
     
     
        int newConnection = arc4random_uniform(g->size);
        
        while(nodesConnected(node1, newConnection, g) == 1 || newConnection == node2 || node1 == newConnection)
        {
            newConnection = arc4random_uniform(g->size);
        }
        
        createConnection(node1, newConnection, g);
        if(!supersetOfMST(g))
        {
            createConnection(node1, node2, g);
            removeConnection(node1, newConnection, g);
            superset = 0;
        }
        else
        {
            superset = 1;
        }
    }
    //printf("node that removes link's payoff = %d\n", getArrayValue(__LINE__, g->payoffMat, node1Location));
}
void removeARRFromSameType(struct Graph *g)
{
    fflush(stdout);
    //Node 1 will lose a connection and gain a new random one
    int node1 = arc4random_uniform(g->size);
    int node2 = arc4random_uniform(g->size);
    int node1Location[] = {node1};
    int node2Location[] = {node2};
    int superset = 0;
    int whCount = 0;
    while(superset == 0)
    {
        while(getArrayValue(__LINE__, g->coopMat, node1Location) != getArrayValue(__LINE__, g->coopMat, node2Location) || countConnectionsOfNode(node2, g) == 1 || countConnectionsOfNode(node1, g) == (g->size - 1) || nodesConnected(node1, node2, g) == 0 || node1 == node2)
        { 
            if (whCount > 1000000) 
            {
                break;
            }
            whCount++;
            node1 = arc4random_uniform(g->size);
            node2 = arc4random_uniform(g->size);
        }
        if (whCount > 1000000) 
        {
            break;
        }
        removeConnection(node1, node2, g);
     
     
        int newConnection = arc4random_uniform(g->size);
        
        while(nodesConnected(node1, newConnection, g) == 1 || newConnection == node2 || node1 == newConnection)
        {
            newConnection = arc4random_uniform(g->size);
        }
        
        createConnection(node1, newConnection, g);
        if(!supersetOfMST(g))
        {
            createConnection(node1, node2, g);
            removeConnection(node1, newConnection, g);
            superset = 0;
        }
        else
        {
            superset = 1;
        }
    }
}
void removeAndRewireRandom(struct Graph *g)
{
    fflush(stdout);
    //Node 1 will lose a connection and gain a new random one
    int node1 = arc4random_uniform(g->size);
    int node2 = arc4random_uniform(g->size);
    int superset = 0;
    while(superset == 0)
    {
        while(countConnectionsOfNode(node2, g) == 1 || countConnectionsOfNode(node1, g) == (g->size - 1) || nodesConnected(node1, node2, g) == 0 || node1 == node2)
        { 
            node1 = arc4random_uniform(g->size);
            node2 = arc4random_uniform(g->size);
        }
        removeConnection(node1, node2, g);
     
     
        int newConnection = arc4random_uniform(g->size);
        
        while(nodesConnected(node1, newConnection, g) == 1 || newConnection == node2 || node1 == newConnection)
        {
            newConnection = arc4random_uniform(g->size);
        }
        
        createConnection(node1, newConnection, g);
        if(!supersetOfMST(g))
        {
            createConnection(node1, node2, g);
            removeConnection(node1, newConnection, g);
            superset = 0;
        }
        else
        {
            superset = 1;
        }
    }
}


void printStepToFile(struct Graph *g)
{
    char buffer[128];
    snprintf(buffer, sizeof(buffer), "graph_%lu", g->graphNumber);
    changeDir(buffer);
    snprintf(buffer, sizeof(buffer), "step_%d", g->step);
    makeDir(buffer);
    changeDir(buffer);
    printCoopStatusToFile(g);
    printPayoffToFile(g);
    printAdjMatToFile(g);
    changeDir("../..");
}
void makeGraphFolder(struct Graph *g)
{
    char buffer[128];
    snprintf(buffer, sizeof(buffer), "graph_%lu", g->graphNumber);;
    makeDir(buffer);
}

void simulateGraph(struct Graph *g, long output, int cuttingType)
{
    int i = 0;
    calculateRatio(g);
    int maximumNumberOfSteps = 1000000;
    makeGraphFolder(g);
    if (output == 1)
    {
        printf("asdfasdf\n");
        printStepToFile(g);
    }
    g->step = 0;
    while((i < maximumNumberOfSteps) && (g->cooperators > 0 & g->defectors > 0))
    {
        i++;
        g->step = i;
        if (cuttingType == 0)
        {
            removeAndRewireRandom(g);
        }
        if (cuttingType == 1)
        {
            removeARRFromLowPayoffNode(g);
        }
        if (cuttingType == 2)
        {
            //do nothing
        }
        if (cuttingType == 3)
        {
            removeARRFromSameType(g);
        } 
        chooseDeadNode(g);
        calculatePayoff(g);
        calculateChangeProbability(g);
        applyChangeProbability(g);
        calculateRatio(g);
        if (output == 1)
        {
            printStepToFile(g);
        }
    }
    printRatioToFile(g);
    printStepsToFile(g);
    free(g->adjMat.arrayHead);
    free(g->adjMat.sizes);
    free(g->coopMat.arrayHead);
    free(g->coopMat.sizes);
    free(g->visited.arrayHead);
    free(g->visited.sizes);
    free(g->payoffMat.arrayHead);
    free(g->payoffMat.sizes);
    free(g);
}

struct Graph *generateGraph(double pForErdosRenyi, int graphNumber, int graphSize, double selectionIntensity, int benefitPayoff, int costPayoff)
{
    //we need:
    //an array for the adj matrix
    char adjCharArr[128];
    snprintf(adjCharArr, sizeof(adjCharArr), "%d, %d", graphSize, graphSize);
    struct Array adjMat = createIntArray(__LINE__, adjCharArr);
 
    //an array for the coop matrix
    char coopCharArr[128];
    snprintf(coopCharArr, sizeof(coopCharArr), "%d", graphSize);
    struct Array coopMat = createIntArray(__LINE__, coopCharArr);
 
    //an array for the visited matrix
    char visitedCharArr[128];
    snprintf(visitedCharArr, sizeof(visitedCharArr), "%d", graphSize);
    struct Array visited = createIntArray(__LINE__, visitedCharArr);
  
    //an array for the payoff matrix
    char payoffCharArr[128];
    snprintf(payoffCharArr, sizeof(payoffCharArr), "%d", graphSize);
    struct Array payoffMat = createIntArray(__LINE__, payoffCharArr);
  
 
    struct Graph *g = malloc(sizeof(struct Graph));
    g->selectionIntensity = selectionIntensity;
    g->benefitPayoff = benefitPayoff;
    g->costPayoff = costPayoff;
    g->size = graphSize;
    g->adjMat = adjMat;
    g->coopMat = coopMat;
    g->visited = visited;
    g->payoffMat = payoffMat;
    g->step = 0;
    g->graphNumber = graphNumber;
    g->pForErdosRenyi = pForErdosRenyi;
 
    int i,j;
    for (i=0;i<g->size;i++)
    {
        int location[] = {i};
        setArrayValue(__LINE__, g->visited, location, 0);
        setArrayValue(__LINE__, g->payoffMat, location, 0);
        setArrayValue(__LINE__, g->coopMat, location, 0);
        for (j=0;j<g->size;j++)
        {
            int location1[] = {i, j};
            setArrayValue(__LINE__, g->adjMat, location1, 0);
        }
    }
    while (supersetOfMST(g) == 0)
    {
        createGraph(g);
    }
    return g;
}

