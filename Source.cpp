#include <iostream>
#include<list>
#include<vector>
#include<random>
#include <queue>
#include <fstream>
#include<time.h>


using namespace std;

/*By: Sagi Buria 313560252
		Lior Nissan 313428138 */


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

	if (p == 0)  // Base case for no prob for an edge
		return vertices;

	for (int i = 0;i < V;i++) {
		for (int j = i+1;j < V;j++) {
			rand(); // Trash one psudeo-random number, to gain a "real" random number
			
			if ((double)rand() / RAND_MAX <= p) { // There is an edge
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
		{
			delete[] result;
			return 0;
		}
	delete[] result;
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

	delete[] visited;
	delete[] distance;
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

void threshold1(int v,std::ofstream& myfile) {
	std::vector<int>* aGraph;
	double pValues[10] = { 0.00511,0.0052,0.0057,0.0059,0.006,0.007,0.008,0.009,0.01,0.011 }; //first argument
	int isConn;

	for (int i = 0; i < 10;i++) {
		for (int j = 0;j < 500;j++) {
			aGraph = build_random_graph(v, pValues[i]);
			isConn = connectivity(aGraph, v);
			myfile << pValues[i] << "," << isConn << "\n";
			delete[] aGraph;
		}
	}

}

void threshold2(int v, std::ofstream& myfile) {
	std::vector<int>* aGraph;
	double pValues[10] = { 0.05,0.07,0.08,0.09,0.1,0.12,0.122,0.13,0.14,0.2 }; //second argument
	int diam;

	for (int i = 0; i < 10;i++) {
		for (int j = 0;j < 500;j++) {
			aGraph = build_random_graph(v, pValues[i]);
			diam = diameter(aGraph, v);
			myfile << pValues[i] << "," << diam << "\n";
			delete[] aGraph;
		}
	}

}

void threshold3(int v, std::ofstream& myfile) {
	std::vector<int>* aGraph;
	double pValues[10] = { 0.0055,0.00594,0.0061,0.0063,0.0066,0.0075,0.0078,0.0083,0.0089,0.0095 }; //third argument
	int isIso;

	for (int i = 0; i < 10;i++) {
		for (int j = 0;j < 500;j++) {
			aGraph = build_random_graph(v, pValues[i]);
			isIso = is_isolated(aGraph, v);			
			myfile << pValues[i] << "," << isIso << "\n";
			delete[] aGraph;
		}
	}

}


int main()
{
	int v =1000;
	std::ofstream myfile;
	myfile.open("AlgoAssign.csv");
	srand(time(0)); // For the build graph random generator

	threshold1(v, myfile);
	threshold2(v, myfile);
	threshold3(v, myfile);
	
	myfile.close();
}




