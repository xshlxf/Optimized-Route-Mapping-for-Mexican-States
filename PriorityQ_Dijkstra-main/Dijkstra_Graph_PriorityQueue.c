#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#define MaxNumV 100

char vertex[MaxNumV][200];
int edges[MaxNumV][MaxNumV];
int NumV;

typedef struct {
    char city[40];
    int distance;
} Node;

Node priorityQueue[MaxNumV];
int front = -1, rear = -1;

void enqueue(Node element) {
    if (rear == MaxNumV - 1) {
        printf("Queue overflow\n");
    } else {
        if (front == -1) {
            front = 0;
        }
        rear++;
        priorityQueue[rear] = element;
    }
}

Node dequeue() {
    if (front == -1 || front > rear) {
        printf("Queue underflow\n");
        exit(1);
    } else {
        Node item = priorityQueue[front];
        front++;
        return item;
    }
}

int isEmpty() {
    return front == -1 || front > rear;
}

//print the queue with the sum distance.
void printQueue() {
    if (isEmpty()) {
        printf("\nQueue: empty!\n");
    } else {
        printf("\nQueue:\n");

        for (int i = front; i <= rear; i++) {
            printf(" %s (%d)\n", priorityQueue[i].city, priorityQueue[i].distance);
        }

        printf("\n");
    }
}
//find index related to the city name.
int findIndex(char *city) {
    for (int i = 0; i < NumV; i++) {
        if (strcmp(vertex[i], city) == 0) {
            return i;
        }
    }
    return -1;
}

// Recursive function to print the nodes in the shortest path
void printShortestPath(int parent[], int j) {
    if (parent[j] == -1) {
        printf("%s", vertex[j]);
        return;
    }

    printShortestPath(parent, parent[j]);

    printf("->%s", vertex[j]);
}

void dijkstra(char *cityFrom, char *cityTo) {
    printf("Finding shortest path from '%s' to '%s' using Dijkstra's algorithm...\n", cityFrom, cityTo);

    int distance[MaxNumV]; // Array to store the distances from the source node to each graph node.
    int visited[MaxNumV]; // Array that indicates if the node is visited.
    int parent[MaxNumV]; // Array to store the parent node of each node in the shortest path
    //initialize the arrays.
    for (int i = 0; i < NumV; i++) {
        distance[i] = INT_MAX;
        visited[i] = 0;
        parent[i] = -1;
    }

    int fromIndex = findIndex(cityFrom);
    int toIndex = findIndex(cityTo);

    if (fromIndex == -1 || toIndex == -1) {
        printf("Invalid city names.\n");
        return;
    }

    distance[fromIndex] = 0;
    Node startNode;
    strcpy(startNode.city, cityFrom);
    startNode.distance = 0;
    enqueue(startNode);
    //loop to explore all the graph
    while (!isEmpty()) {
        int currentIndex = -1;
        int minDistance = INT_MAX;

        for (int i = front; i <= rear; i++) {
            if (priorityQueue[i].distance < minDistance && !visited[findIndex(priorityQueue[i].city)]) {
                currentIndex = i;
                minDistance = priorityQueue[i].distance;
            }
        }

        if (currentIndex == -1) {
            break;
        }

        Node currentNode = dequeue();
        visited[findIndex(currentNode.city)] = 1;
        //for that compares, enqueues and sum the costs of the nodes.
        for (int i = 0; i < NumV; i++) {
            if (edges[findIndex(currentNode.city)][i] != 0) {
                int newDistance = distance[findIndex(currentNode.city)] + edges[findIndex(currentNode.city)][i];
                if (!visited[i] && newDistance < distance[i]) {
                    distance[i] = newDistance;
                    parent[i] = findIndex(currentNode.city);
                    Node newNode;
                    strcpy(newNode.city, vertex[i]);
                    newNode.distance = newDistance;
                    enqueue(newNode);
                }
            }
        }
        //prints the queue in iterations.
        printQueue();
    }

    // Print the nodes in the shortest path
    printf("\nShortest path from '%s' to '%s': ", cityFrom, cityTo);
    printShortestPath(parent, toIndex);
    printf(" total distance = %d\n", distance[toIndex]);

}
//reads and saves all data form files.
void readGraphFromFile(char *verticesFile, char *edgesFile) {
    FILE *fp = fopen(verticesFile, "r");

    if (fp == NULL) {
        printf("Unable to open vertices file...");
        exit(-1);
    }

    printf("Vertices file successfully open...\n");
    int i = 0;

    while (fscanf(fp, "%s", vertex[i]) == 1) {
        printf("City %d: %s\n", i, vertex[i]);
        i++;
    }

    printf("\nNum of vertices in file: %d\n\n", i);
    NumV = i;

    fclose(fp);

    fp = fopen(edgesFile, "r");

    if (fp == NULL) {
        printf("Unable to open edges file...");
        exit(-1);
    }

    printf("Edges file successfully open...\n");

    char cityFrom[40];
    char cityTo[40];
    int cost;

    while (fscanf(fp, "%s %s %d", cityFrom, cityTo, &cost) == 3) {
        int k = findIndex(cityFrom);
        int j = findIndex(cityTo);

        if (k == -1 || j == -1) {
            printf("Invalid city names in edges file.\n");
            exit(-1);
        }

        edges[k][j] = cost;
        //prints all the data saved from the edges file
        printf("<From '%s'(%d) to '%s'(%d): %d>\n", cityFrom, k, cityTo, j, cost); 
    }
    //print the adacency matrix.
    printf("\nAdjacency matrix\n");
    printf("    ");
    for (int k = 0; k < NumV; k++)
        printf("%d\t", k);
    printf("\n    ");
    for (int k = 0; k < NumV; k++)
        printf("-------");

    for (int k = 0; k < NumV; k++) {
        printf("\n %d| ", k);
        for (int j = 0; j < NumV; j++)
            printf("%d\t", edges[k][j]);
    }

    printf("\n");

    fclose(fp);
}

int main() {
    readGraphFromFile("vertices.txt", "edges.txt");

    int retry;
    char cityFrom[40];
    char cityTo[40];

    do {
        
        FILE *fp = fopen("vertices.txt", "r");
        int j = 0;
        int i = 0;  
        printf("\nAvailable cities\n\t");
        while(fscanf(fp, "%s", vertex[i]) == 1) { //prints all the available cities.
            printf("%s\t",vertex[i]);
            if(j==10){ //each 10 cities prints \n for better understanding.
                printf("\n\t");
                j=0;
            }
            j++;
            i++;
        }
        fclose(fp);
        printf("\n");
        
        do { //scans the source city.
            printf("\nType the source city: ");
            scanf("%s", cityFrom);  
            if (findIndex(cityFrom) == -1) {
                printf("\nInvalid city name. Try again.\n");
            }
        } while (findIndex(cityFrom) == -1);

        do { //scans the destiny city.
            printf("Type the destination city: ");
            scanf("%s", cityTo);
            if (findIndex(cityTo) == -1) {
                printf("\nInvalid city name. Try again.\n");
            }
        } while (findIndex(cityTo) == -1);

        dijkstra(cityFrom, cityTo);

        printf("Do you want to restart? (1 for yes, 0 for no): ");
        scanf("%d", &retry);
    } while (retry == 1);
    return 0;
}
