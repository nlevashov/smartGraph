#include <set>
#include <vector>

#include "../graph/graph.h"

template <typename E, typename V = size_t, typename INFO = int>
graph<E, V, INFO> MST_Prim (graph<E, V, INFO> & g) {
	graph<E, V, INFO> tree;
	
	struct EorMaxE {
		bool isSet;
		E num;
	};

	class mypair {
	public:
		EorMaxE distance;
		size_t id;

		bool operator < (const mypair & obj) const {
			if (distance.isSet) {
				if (obj.distance.isSet) return (distance.num < obj.distance.num);
				else					return true;
			} else {
				if (obj.distance.isSet) return false;
				else					return (id < obj.id);
			}
		}
	};

	EorMaxE maxE;
	maxE.isSet = false;
	std::vector<EorMaxE> d (g.size(), maxE);
	std::vector<size_t> prev(g.size());

	std::set<mypair> not_choosen;
	std::vector<V> vertecies = g.vertecies();
	for (size_t i = 0; i < vertecies.size(); ++i) {
		mypair p;
		p.distance = maxE;
		p.id = i;
		not_choosen.insert(p);
	}

	graph<E, V, size_t> orlov;
	auto edges = g.edges();
	for (auto i : edges) orlov.push_edge(i.from()->name(), i.to()->name(), i.weight());
	for (size_t i = 0; i < vertecies.size(); ++i) orlov[vertecies[i]].flag = i;

	size_t current = not_choosen.begin()->id;
	not_choosen.erase(not_choosen.begin());
	while (not_choosen.size() != 0) { //а вот кто его знает?
		auto adjacents = orlov.e_from_v(orlov[vertecies[current]].name());
		for (auto e : adjacents) {
			size_t id = e.to()->flag;
			mypair p;
			p.distance = d[id];
			p.id = id;
			if ((not_choosen.count(p) > 0) && ((!d[id].isSet) || (e.weight() < d[id].num))) {
				not_choosen.erase(p);
				d[id].isSet = true;
				d[id].num = e.weight();
				p.distance = d[id];
				not_choosen.insert(p);
				prev[id] = orlov[vertecies[current]].flag;
			}
		}
		current = not_choosen.begin()->id;
		not_choosen.erase(not_choosen.begin());
		if (d[current].isSet) tree.push_edge(orlov[vertecies[prev[current]]].name(), orlov[vertecies[current]].name(), d[current].num);
	}

	for (auto i : vertecies) tree[i].flag = g[i].flag;

	return tree;
}
