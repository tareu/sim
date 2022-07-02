#include <math.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

struct Array;
struct Graph;

struct Array
{
    int* arrayHead;
    int numDimensions;
    int* sizes;
    int size;
};

struct Graph
{
    double pForErdosRenyi;
    int step;
    double changeProbability;
    int size;
    double selectionIntensity;
    int benefitPayoff;
    int costPayoff;
    int cooperators;
    int defectors;
    struct Array adjMat;
    struct Array coopMat;
    struct Array visited;
    struct Array payoffMat;
    unsigned long graphNumber;
};

void printError(int errorCode, int line);
void calculatePayoff(struct Graph *g);
//createGraph is used internally by generateGraph
void createGraph(struct Graph *g); 
void depthFirstSearch(int currentNode, struct Graph *g);
void dfs(int startingNode, struct Graph *g);
void chooseDeadNode(struct Graph *g);
void calculateChangeProbability(struct Graph *g);
int supersetOfMST(struct Graph *g);
void printCoopStatusToFile(struct Graph *g);
void printPayoffToFile(struct Graph *g);
void printAdjMatToFile(struct Graph *g);
void printAdjMat(struct Graph *g);
void applyChangeProbability(struct Graph *g);
void calculateRatio(struct Graph *g);
void printError(int errorCode, int line);
struct Array createIntArray(int line, char* dimensions);
int translateMultiTo1d(struct Array array, int* inputDim);
int getArrayValue(int line, struct Array array, int* inputDim);
void setArrayValue(int line, struct Array array, int* inputDim, int value);
void simulateGraph(struct Graph *g, long output, int cuttingType);
struct Graph *generateGraph(double pForErdosRenyi, int graphNumber, int graphSize, double selectionIntensity, int benefitPayoff, int costPayoff);
void removeConnection(int node1, int node2, struct Graph *g);
void createConnection(int node1, int node2, struct Graph *g);
int nodesConnected(int node1, int node2, struct Graph *g);
void removeAndRewireConnection(struct Graph *g);
void printStepToFile(struct Graph *g);
void makeGraphFolder(struct Graph *g);
void makeDir(char name[]);
void changeDir(char name[]);
int countConnectionsOfNode(int node, struct Graph *g);
