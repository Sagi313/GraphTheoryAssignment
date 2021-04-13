#include <iostream>
#include<list>
#include<vector>
#include<random>
#include <queue>
#include <fstream>
#include<time.h>


using namespace std;


int is_isolated(std::vector<int>* vertices, int V) // Checks all vetrtcies, if one has no niegbhors, return 1
{
	for (int i = 0; i < V; i++)
	{
		if (vertices[i].size() == 0)
			return 1;
	}
	return 0;
}


std::vector<int>* build_random_graph(int V,double p) {
	std::vector<int>* vertices = new vector<int>[V]; //Creates an array of vertices, for the adjancency list
	for (int i = 0;i < V-1;i++) {
		for (int j = i+1;j < V;j++) {
			rand(); // Trash one psudeo-random number, to gain a "real" random number
			if (rand() % 100 <= p * 100) { // There is an edge
					vertices[i].push_back(j); //Adds the edge
					vertices[j].push_back(i); //Adds the edge
			}
		}
	}
	return vertices;
	
}
void print_graph(std::vector<int>* vertices, int V) {
	for (int i = 0;i < V;i++) {
		cout << i << ": ";
		for (int j = 0; j < vertices[i].size();j++) {
			if (j == vertices[i].size() - 1) { // To lost the last arrow print
				cout << vertices[i][j];
			}
			else {
				cout << vertices[i][j] << " ->";
			}
		}
		cout << "\n";
	}
}

int* DFS(std::vector<int>* vertices, int root, int* visited) {
	if (visited[root] == 1) // I am already visited. Don't check me
		return visited;
	// Do something
	visited[root] = 1; // I got visited.
	for (int i = 0; i < vertices[root].size();i++) {
		DFS(vertices, vertices[root][i], visited);
	}
	return visited;
}

int connectivity(std::vector<int>* vertices, int V) { // We use one DFS iteration. if DFS algo visited all the nodes, then we can sure the graph is connected
	int *result = new int[V](); // Dynamic array set with zeros. for the use of the DFS search function.
	result = DFS(vertices, 0, result);
	
	for (int i = 0; i < V;i++) // Checks to see if there is an unvisited node. if so, return 0
		if (result[i] == 0)
			return 0;
	return 1; // All nodes got visited
}

int BFS(std::vector<int>* vertices,int V,int root) {
	int* visited = new int[V](); // Dynamic array set with zeros. Will hold binary values
	
	int* distance = new int[V](); // Dynamic array set with zeros

	int maxlen = 0; // The longest path found in this BFS iteration
	int curr;
	queue<int> queue; // Create a queue for BFS

	visited[root] = 1; // Mark the current node as visited and enqueue it
	queue.push(root);


	while (!queue.empty())
	{
		curr = queue.front();	//gets the first element in the queue
		queue.pop(); // Delets the first element in the queue
		
		//if (visited[curr] == 1)
		//	continue;

		for (int i = 0; i < vertices[curr].size();i++) {
			if (visited[vertices[curr][i]] == 0) // Not visited
			{
				distance[vertices[curr][i]] = distance[curr] + 1; // Father's node distance +1

				queue.push(vertices[curr][i]);
				visited[vertices[curr][i]] = 1;
			}

		}
	}


	for (int i = 0;i < V;i++) // Gets the highest value of the distances array
	{
		if (distance[i] > maxlen)
			maxlen = distance[i];
	}
	return maxlen;
}

int diameter(std::vector<int>* vertices, int V) {
	int diam = 0; // -2 -> To allow it to be set to -1 flag
	int currLen;

	if (!connectivity(vertices, V)) // If the graph is not connected than diameter=infi
		return -1; // equvalent to infinity
	
	for (int i = 0; i < V;i++) {
		currLen = BFS(vertices, V, i);
		if (currLen > diam)
			diam = currLen;
	}
	return diam;

}

double genRandDouble(double fMin, double fMax) // Generates 2 random doubles between the given limits
{
	double f = (double)rand() / RAND_MAX;
	return fMin + f * (fMax - fMin);
}

double* random_p(double threshold) {
    int amountOfP = 10;

    double* results = new double[amountOfP];
	srand(time(0));

	for (int i = 0; i < amountOfP;i++) {

		if (i < amountOfP / 2) {
			results[i] = genRandDouble(0, threshold);
		}
		else {
			results[i] = genRandDouble(threshold, 1);
		}
	}
	
	sort(results, results + amountOfP); // For convience porpuses.

	return results;
}

void writeCSV(std::ofstream &myfile,double p, int isIso, int isCon, int diam) {
	myfile << p << "," << isIso << "," << isCon << "," << diam << "\n";

	return;
}

int main()
{
	
	int v =5;
	double threshold = 0.5;
	double *allPValues = random_p(threshold);
	std::vector<int>* aGraph;
	int isIso, isConn, diam;
	srand(time(0));

	std::ofstream myfile;
	myfile.open("AlgoAssign.csv");

	for (int i = 0; i < 10;i++) {
		for (int j=0;j < 50;j++) {
			aGraph = build_random_graph(v, allPValues[i]);
			isIso = is_isolated(aGraph, v);
			isConn = connectivity(aGraph, v);
			diam = diameter(aGraph, v);
			writeCSV(myfile , allPValues[i], isIso, isConn, diam);
		}
	}
	
	myfile.close();
	
	
}




