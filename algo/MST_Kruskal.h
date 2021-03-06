#include <set>

#include "../graph/graph.h"
#include "utils/disjoint_set_data.h"

template <typename E, typename V = size_t, typename INFO = int>
graph<E, V, INFO> MST_Kruskal (graph<E, V, INFO> & g) {
	graph<E, V, INFO> tree;
	auto edges = g.edges();
	auto vertecies = g.vertecies();
	std::multiset<typename graph<E, V, INFO>::edge> sorted_edges(edges.begin(), edges.end());
	disjoint_set_data<V> dsd(vertecies);
	size_t k = 1;
	for (auto e : sorted_edges) {
		if (dsd.merge(e.from()->name(), e.to()->name())) {
			tree.push_edge(e.from()->name(), e.to()->name(), e.weight());
			++k;
			if (k == g.size()) break;
		}
	}
	for (auto i : vertecies) tree[i].flag = g[i].flag;
	return tree;
}
