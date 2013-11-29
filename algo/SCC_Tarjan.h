#include <vector>
#include <stack>

#include "../graph/graph.h"

template <typename E, typename V = size_t, typename INFO = int>
std::vector<std::vector<V>> SCC_Tarjan (graph<E, V, INFO> & g)
{
	struct indexes {
		size_t id, low;
	};
	indexes max_indexes;
	max_indexes.id = -1;
	max_indexes.low = -1;
	
	graph<E, V, indexes> orlov;

	auto edges = g.edges();
	for (auto i : edges) orlov.push_edge(i.from()->name(), i.to()->name(), 0);

	auto vertecies = orlov.vertecies();
	for (auto i : vertecies) orlov[i].flag = max_indexes;

	std::stack<V> s;
	std::vector<std::vector<V>> components;

	int k = 1;
	for (auto i = orlov.full_dfs_begin(); i != orlov.full_dfs_end(); ++i) {
		if (i.color() == 1) {
			i->flag.id = i->flag.low = k;
			s.push(i->name());
			++k;
		} else {
			auto out = orlov.v_from_v(i->name());
			for (auto j : out) {
				if (i->flag.low > orlov[j].flag.low) i->flag.low = orlov[j].flag.low;
			}

			if (i->flag.id == i->flag.low) {
				std::vector<V> temp;
				while (s.top() != i->name()) {
					orlov[s.top()].flag.low = -1;
					temp.push_back(s.top());
					s.pop();
				}
				orlov[s.top()].flag.low = -1;
				temp.push_back(s.top());
				s.pop();
				components.push_back(temp);
			}
		}
	}

	return components;
}
