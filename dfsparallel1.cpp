#include <iostream>
#include <vector>
#include <stack>
#include <Windows.h>
#include <omp.h>

using namespace std;

void parallelDFS(const vector<vector<int>>& graph, int startNode, vector<bool>& visited) {
    int numNodes = graph.size();
    stack<int> dfsStack;
    dfsStack.push(startNode);

    #pragma omp parallel
    {
        while (!dfsStack.empty()) {
            int currentNode;

            #pragma omp critical
            {
                currentNode = dfsStack.top();
                dfsStack.pop();
            }

            #pragma omp critical
            {
                if (!visited[currentNode]) {
                    visited[currentNode] = true;
                    #pragma omp critical
                    cout << "Node " << currentNode << " visited by Thread " << GetCurrentThreadId() << endl;
                }
            }

            #pragma omp for
            for (int i = 0; i < graph[currentNode].size(); ++i) {
                int neighborNode = graph[currentNode][i];

                #pragma omp critical
                {
                    if (!visited[neighborNode]) {
                        dfsStack.push(neighborNode);
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

    vector<bool> visited(numNodes, false);

    parallelDFS(graph, startNode, visited);

    return 0;
}

