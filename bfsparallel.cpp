#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>

using namespace std;

void parallelBFS(const vector<vector<int>>& graph, int startNode, vector<int>& distances) {
    int numNodes = graph.size();
    distances.resize(numNodes, -1); // Initialize distances to -1 (unvisited)

    queue<int> bfsQueue;
    bfsQueue.push(startNode);
    distances[startNode] = 0;

    #pragma omp parallel
    {
        while (!bfsQueue.empty()) {
            int currentNode;

            #pragma omp critical
            {
                currentNode = bfsQueue.front();
                bfsQueue.pop();
            }

            #pragma omp for
            for (int i = 0; i < graph[currentNode].size(); ++i) {
                int neighborNode = graph[currentNode][i];

                if (distances[neighborNode] == -1) {
                    distances[neighborNode] = distances[currentNode] + 1;

                    #pragma omp critical
                    {
                        bfsQueue.push(neighborNode);
                    }
                }
            }
        }
    }
}

int main() {
    int numNodes;
    cout << "Enter the number of nodes in the graph: ";
    cin >> numNodes;

    vector<vector<int>> graph(numNodes);

    // Get input for the graph
    for (int i = 0; i < numNodes; ++i) {
        int numNeighbors;
        cout << "Enter the number of neighbors for node " << i << ": ";
        cin >> numNeighbors;

        graph[i].resize(numNeighbors);
        cout << "Enter the neighbors of node " << i << ": ";
        for (int j = 0; j < numNeighbors; ++j) {
            cin >> graph[i][j];
        }
    }

    int startNode;
    cout << "Enter the starting node: ";
    cin >> startNode;

    vector<int> distances;

    parallelBFS(graph, startNode, distances);

    // Print the distances from the start node
    cout << "Distances from node " << startNode << ":" << endl;
    for (int i = 0; i < numNodes; ++i) {
        cout << "Node " << i << ": " << distances[i] << endl;
    }

    return 0;
}

