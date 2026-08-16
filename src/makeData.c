/**
 * Author: Noah Albright
 * FIle: makeData.c
 * Purpose: makes the random data for each phase of the project. This is also automated
 * with given bash script. 
*/

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <float.h>


typedef struct point{
    float x;
    float y;
    struct point *next;
}point;

typedef struct node{
    point *nodePoint;
    struct node *next;
}node;

typedef struct edge{
    node *fromNode;
    node *toNode;
    float a;
    float b;
    float c;
    struct edge *next;
}edge;

typedef struct graph{
    point *pointList;
    node *nodeList;
    edge *edgeList;
    int n;
    int e;
    int p;
}graph;

/**
 * max returns the number that is greater between a and b.
*/
float max(float a, float b){
    return (a > b) ? a : b;
}

/**
 *  d1 calculates the d1 metric between
 *  two points in R^2
*/
float d1(point *p1, point *p2){
    return abs(p1->x - p2->x) + abs(p1->y - p2->y);
}

/**
 *  d2 calculates the d2 metric between
 *  two points in R^2
*/
float d2(point *p1, point *p2){
    return pow(pow(p1->x - p2->x, 2) + pow(p1->y - p2->y, 2), 0.5);
}

/**
 * dInf calculates the dInf metric between
 * two points in R^2.
*/
float dInf(point *p1, point *p2){
    return max(fabs(p1->x - p2->x), fabs(p1->y - p2->y));
}

/**
 * pointEquals uses the  dInf metric to check
 * if two points are equal. As floating point
 * numbers are approximations it is set to less
 * than  .0001.
*/
int pointEquals(point *p1, point *p2){
    return dInf(p1, p2) < FLT_EPSILON;
}

/**
 * normalizeEdgeValuees normalize the values
 * for the given edge standard form equation
 * (ax + by = c) by dividing it by the norm
 * of (a, b).
*/
void normalizeEdgeValues(edge *e){
    float norm = pow(pow(e->a, 2) + pow(e->b, 2), 0.5);
    e->a = e->a / norm;
    e->b = e->b / norm;
    e->c = e->c / norm;
}

/**
 * makeEdgeValues makes the values for
 * the standard form equation for the
 * given edge.
*/
void makeEdgeValues(edge *e){
    e->a = e->fromNode->nodePoint->y - e->toNode->nodePoint->y;
    e->b = e->toNode->nodePoint->x - e->fromNode->nodePoint->x;
    e->c = e->b * e->fromNode->nodePoint->y + e->a * e->fromNode->nodePoint->x;
    normalizeEdgeValues(e);
}

/**
 * getCross returns an integer stating which of the specified
 * options in  the cross function between two integers. More
 *  on the cross function and this function specially given
 * in paper. 
*/
int getCross(edge *e1, edge *e2){
    float e1Dist = d2(e1->toNode->nodePoint, e1->fromNode->nodePoint);
    float e2Dist = d2(e2->toNode->nodePoint, e2->fromNode->nodePoint);

    if(e1->a - e2->a < FLT_EPSILON && e1->b - e2->b < FLT_EPSILON){
        if(e1->c - e2->c < FLT_EPSILON){
            if(d2(e1->toNode->nodePoint, e2->toNode->nodePoint) <= e1Dist + FLT_EPSILON){
                if(d2(e1->fromNode->nodePoint, e2->fromNode->nodePoint) <= 2 * (e1Dist + FLT_EPSILON)) return 2;
                else return 0;
            }
            if(d2(e1->fromNode->nodePoint, e2->fromNode->nodePoint) <= e1Dist + FLT_EPSILON){
                if(d2(e1->toNode->nodePoint, e2->toNode->nodePoint) <= 2 * (e1Dist + FLT_EPSILON)) return 2;
                else return 0;
            }
            else return 0; 
        }
        else return 0;
    }

    float intersectX;
    float intersectY;


    if(e1->a == 0){
        float multiplier = e2->b / e1->b;
        intersectX = (e2->c - e1->c * multiplier) /(e2->a - e1->a * multiplier);
        intersectY = e1->c / e1->b;
    } else{
        float multiplier = e2->a / e1->a;
        intersectY = (e2->c - e1->c * multiplier) /(e2->b - e1->b * multiplier);
        intersectX = (e1->c - e1->b * intersectY) / e1->a;
    }

    point *intersectPoint = malloc(sizeof(point));
    intersectPoint->x = intersectX;
    intersectPoint->y = intersectY;

    int e1Endpoint = pointEquals(e1->toNode->nodePoint, intersectPoint) | pointEquals(e1->fromNode->nodePoint, intersectPoint);
    int e2Endpoint = pointEquals(e2->toNode->nodePoint, intersectPoint) | pointEquals(e2->fromNode->nodePoint, intersectPoint);

    int e1Interior = d2(intersectPoint, e1->toNode->nodePoint) < e1Dist + FLT_EPSILON && d2(intersectPoint, e1->fromNode->nodePoint) < e1Dist + FLT_EPSILON;
    int e2Interior = d2(intersectPoint, e2->toNode->nodePoint) < e2Dist + FLT_EPSILON && d2(intersectPoint, e2->fromNode->nodePoint) < e2Dist + FLT_EPSILON;

    free(intersectPoint);

    if(e1Endpoint | e2Endpoint) return 0;
    else if (e1Interior & e2Interior) return 1;
    return 0;
}

/**
 * cross returns the given cross number
 * between the two given edges which is
 * defined in the paper.
*/
int cross(edge *e1, edge *e2, int n){
    switch (getCross(e1, e2))
    {
        case 0: return 0;
        case 1: return 1;
        case 2: return n;
    }
    return -1; // For  compilier.
}

/**
 * crossings returns the crossing number
 * for the given graph as stated by the
 * function above, more info will be given
 * in the paper for the project.
*/
int crossings(graph *g){
    edge *cur = g->edgeList;
    if(cur == NULL) return -1;
    int crossingsNum = 0;
    while(cur->next != NULL){
        edge *curNext = cur->next;
        while(curNext != NULL){
            crossingsNum += cross(cur, curNext, g->n);
            curNext = curNext->next;
        }
        cur = cur->next;
    }

    return crossingsNum;
}

/**
 * randFloat returns an random flloat between the
 * minimum and maximum given.
*/
float randFloat(float min, float max){
    float randRet = (float) rand() / (float) RAND_MAX;
    return (max - min) * randRet + min;
}

/**
 * inPointList returns a 1 if the given point is in
 * the point list and an 0 if it not in the point
 * list. 
*/
int inPointList(point *pointList, point *newPoint){
    point *cur = pointList;
    while (cur != NULL){
        if(pointEquals(cur, newPoint)) {
            return 1;
        }
        cur = cur->next;
    }
    return 0;  
}

/**
 * makePoints makes the p random points in the 
 * closed annulus between innerRad and OuterRad.
*/
void makePoints(graph *g, float innerRad, float outerRad){
    int  count = 0;
    point *center = malloc(sizeof(point));
    center->x = 0;
    center->y = 0;
    while(count < g->p){
        float x = randFloat(-outerRad, outerRad);
        float y = randFloat(-outerRad, outerRad);
        point *newPoint = malloc(sizeof(point));
        newPoint->x = x;
        newPoint->y = y;
        if(d2(center, newPoint) <= outerRad && d2(center, newPoint) >= innerRad){
            if(inPointList(g->pointList, newPoint) == 0){
                newPoint->next = g->pointList;
                g->pointList = newPoint;
                count++;
            } else free(newPoint);
        } else free(newPoint);
    }
    free(center);
}

/**
 * makeNodes makes the n nodes of
 * the graph.
*/
void makeNodes(graph *g){
    point *cur = g->pointList;
    while (cur != NULL){
        node *newNode = malloc(sizeof(node));
        newNode->nodePoint = cur;
        newNode->next = g->nodeList;
        g->nodeList = newNode;
        cur = cur->next;
    }
}

/**
 * makeEdges makes the e random edges
 * for the given graph.
*/
void makeEdges(graph *g){
    int numEdges[g->n];
    for(int i = 0; i < g->n; i++){
        numEdges[i] = g->n - i - 1;
    }
    int numToRemove = (g->n) * (g->n - 1) / 2 - g->e;
    while(numToRemove > 0){
        int nodeToRemove = rand() % (g->n - 1);
        if(numEdges[nodeToRemove] != 0){
            numEdges[nodeToRemove]--;
            numToRemove--;
        }
    }
    node *cur = g->nodeList;
    node *curNext = g->nodeList->next;
    int idx = 0;
    // Appendix - Exxplain why this is still Random.
    while(curNext != NULL){
        int curEdges = numEdges[idx];
        switch (idx % 2){
            case 0:
            for(int i = 0; i < curEdges; i++){
                edge *newEdge = malloc(sizeof(edge));
                newEdge->toNode = cur;
                newEdge->fromNode = curNext;
                makeEdgeValues(newEdge);
                newEdge->next = g->edgeList;
                g->edgeList = newEdge;
                curNext = curNext->next;
            }
            break;
            case 1:
            for(int i = idx + 1; i < g->n; i++){
                if(i >= g->n - curEdges){
                    edge *newEdge = malloc(sizeof(edge));
                    newEdge->toNode = cur;
                    newEdge->fromNode = curNext;
                    makeEdgeValues(newEdge);
                    newEdge->next = g->edgeList;
                    g->edgeList = newEdge;
                }
                curNext = curNext->next;
            }
            break;
        }

        cur = cur->next;
        curNext = cur->next;
        idx++;
    }
}

/**
 * minEdge returns the minimum edge
 * distance.
*/
float minEdge(graph *g){
    float min = __FLT_MAX__;
    edge *cur = g->edgeList;
    while(cur != NULL){
        float curDist = d2(cur->toNode->nodePoint, cur->fromNode->nodePoint);
        if(curDist < min){
            min = curDist;
        }
        cur = cur->next;
    }

    return min;
}

/**
 * maxEdge returns the minimum edge
 * distance.
*/
float maxEdge(graph *g){
    float max = __FLT_MIN__;
    edge *cur = g->edgeList;
    while(cur != NULL){
        float curDist = d2(cur->toNode->nodePoint, cur->fromNode->nodePoint);
        if(curDist > max){
            max = curDist;
        }
        cur = cur->next;
    }
    return max;
}

/**
 * freePoints free the points list 
 * of the given graph
*/
void freePoints(point *pointList){
    point *cur = pointList;
    while(cur != NULL){
        point *curNext = cur->next;
        free(cur);
        cur = curNext;
    }
}

/**
 * freeNodes free the nodes list 
 * of the given graph
*/
void freeNodes(node *nodeList){
    node *cur = nodeList;
    while(cur != NULL){
        node *curNext = cur->next;
        free(cur);
        cur = curNext;
    }
}

/**
 * freeEdges free the edges list 
 * of the given graph
*/
void freeEdges(edge *edgeList){
    edge *cur = edgeList;
    while(cur != NULL){
        edge *curNext = cur->next;
        free(cur);
        cur = curNext;
    }
}

/**
 * freeGraph frees the graph.
*/
void freeGraph(graph *g){
    freeEdges(g->edgeList);
    freeNodes(g->nodeList);
    freePoints(g->pointList);
    free(g);
}

/**
 * MakeGraph makes an random graph with the given values for nodes,
 * edges, inner radius, and outer radius. Also will use the given seed
 * to seed the rand() function.
*/
graph *makeGraph(int nodes, int edges, float innerRad, float outerRad, int seed){
    graph *g = malloc(sizeof(graph));
    g->p = nodes;
    g->n = nodes;
    g->e = edges;
    g->pointList = NULL;
    g->nodeList = NULL;
    g->edgeList = NULL;
    srand(seed);

    makePoints(g, innerRad, outerRad);
    makeNodes(g);
    makeEdges(g);

    return g;
}

int main(int argc, char **argv){
    int phase = atoi(argv[1]);
    int numData = atoi(argv[2]);
    int nodes = 50;
    int edges = 1225;
    float innerRad = 0.0;
    float outerRad = 50.0;
    srand(time(NULL));

    printf("nodes,edges,inner radius,outer radius,minimum distance,maximum distance,crossings");
    for(int i = 0; i < numData; i++){
        printf("\n");
        graph *g;
        switch (phase){
            case 1:
            nodes = rand() % (91) + 10;
            edges = rand() % ((nodes * (nodes - 3) + 2) / 2) + nodes;
            break;
            case 2:
            break;
            case 3:
            outerRad = randFloat(0.0, 50.0);
            break;
            case 4:
            innerRad = randFloat(0.0, outerRad);
            break;
            case 5:
            outerRad = randFloat(0.0, 50.0);
            innerRad = randFloat(0.0, outerRad);
            break;
            case 6:
            nodes = rand() % (91) + 10;
            edges = rand() % ((nodes * (nodes - 3) + 2) / 2) + nodes;
            outerRad = randFloat(1.0, 50.0);
            innerRad = randFloat(0.0, outerRad);
            break;
        }
        g = makeGraph(nodes, edges, innerRad, outerRad, time(NULL));
        printf("%d,%d,%.2f,%.2f,%.2f,%.2f,%d", nodes, edges, innerRad, outerRad, minEdge(g), maxEdge(g), crossings(g));
        freeGraph(g);
    }
}