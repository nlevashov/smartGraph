#include <iostream>
#include <fstream>
#include <set>
#include <string>

#include "../graph/graph.h"
#include "../algo/SCC_Tarjan.h"
#include "../algo/MST_Prim.h"
#include "../algo/MST_Kruskal.h"

using namespace std;

int main (int argc, char ** argv)
{
	string file, start;
	if (argc == 2) {
		file = argv[1];
		start = "1";
	} else if (argc == 3) {
		file = argv[1];
		start = argv[2];
	} else {
		file = "graphs/in2";
		start = "1";
	}
	cout << file << " from " << start << ":" << endl;

	graph<int, string> g;
	ifstream fin;
	fin.open(file);
	while (!fin.eof()) {
		string from, to;
		int weight;
		fin >> from >> to >> weight;
		g.push_edge(from, to, weight);
	}
	fin.close();

	for (graph<int, string>::dfs i(&g, start); i != graph<int, string>::dfs_end(); i++) {
		cout << i->name() << " ";
	}
	cout << endl;

	for (graph<int, string>::bfs i(&g, start); i != graph<int, string>::bfs_end(); i++) {
		cout << i->name() << " ";
	}
	cout << endl;

	for (graph<int, string>::full_dfs i(&g, start); i != g.full_dfs_end(); ++i) {
		cout << i->name() << " ";
	}
	cout << endl;

	for (graph<int, string>::full_dfs i(&g, start); i != g.full_dfs_end(); ++i) {
		cout << i->name() << " " << (int) i.color() << " | ";
	}
	cout << "<|" << endl;


	cout << "----------------" << endl << "Tarjan:" << endl;

	auto comp = SCC_Tarjan<int, string>(g);
	for (auto i : comp) {
		for (auto j : i) {
			cout << j << " ";
		}
		cout << endl;
	}


	cout << "----------------" << endl << "Prim:" << endl;

	graph<int, string> mst = MST_Prim<int, string>(g);
	auto edges = mst.edges();
	for (auto i : edges) {
		cout << i.from()->name() << " " << i.to()->name() << " " << i.weight() << endl;
	}


	cout << "----------------" << endl << "Kruskal:" << endl;

	mst = MST_Kruskal<int, string>(g);
	edges = mst.edges();
	for (auto i : edges) {
		cout << i.from()->name() << " " << i.to()->name() << " " << i.weight() << endl;
	}
}