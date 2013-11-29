#ifndef _GRAPH

#define _GRAPH

#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

template <typename E, typename V = size_t, typename INFO = int>
class graph
{
public:
	class vertex;
	class edge {
		std::weak_ptr<vertex> _from, _to;
		E _weight;
	public:
		std::shared_ptr<vertex> from () const
		{
			return _from.lock();
		}

		std::shared_ptr<vertex> to () const
		{
			return _to.lock();
		}

		E weight () const
		{
			return _weight;
		}

		bool operator < (const edge & e) const {
			return (_weight < e.weight());
		}

		friend class graph;
	};

	class vertex {
		std::vector<std::weak_ptr<edge>> _in, _out;
		V _name;
	public:
		INFO flag;

		V name () const
		{
			return _name;
		}

		friend class graph;
	};

private:
	std::unordered_map<V, std::shared_ptr<vertex>> _vertecies;
	std::unordered_set<std::shared_ptr<edge>> _edges;

public:
	graph() {}
	graph(const graph & g);
	graph & operator = (const graph & g);

	vertex & operator [] (V id)		const;
	size_t size()					const;

	std::vector<V> 		v_to_v(V id)	const;
	std::vector<V> 		v_from_v(V id)	const;
	std::vector<V> 		vertecies()		const;

	std::vector<edge> 	e_to_v(V id)	const;
	std::vector<edge> 	e_from_v(V id);	//const;
	std::vector<edge> 	edges()			const;

	void push_edge(V from, V to, E weight);
	void push_vertex(V id);

	struct dfs;
	dfs dfs_begin() const;
	static dfs dfs_end();

	struct full_dfs;
	full_dfs full_dfs_begin();
	static full_dfs full_dfs_end();

	struct bfs;
	bfs bfs_begin();
	static bfs bfs_end();
};


template <typename E, typename V, typename INFO>
graph<E, V, INFO>::graph(const graph<E, V, INFO> & g)
{
	_vertecies = g._vertecies;
	_edges = g._edges;
}

template <typename E, typename V, typename INFO>
graph<E, V, INFO> & graph<E, V, INFO>::operator = (const graph<E, V, INFO> & g)
{
	_vertecies = g._vertecies;
	_edges = g._edges;
}

template <typename E, typename V, typename INFO>
typename graph<E, V, INFO>::vertex & graph<E, V, INFO>::operator [] (V id) const
{
	return *(_vertecies.at(id));
}

template <typename E, typename V, typename INFO>
size_t graph<E, V, INFO>::size() const {
	return _vertecies.size();
}

template <typename E, typename V, typename INFO>
std::vector<V> graph<E, V, INFO>::v_to_v(V id) const {
	std::vector<V> result;
	for (auto i : _vertecies.at(id)->_in) {
		result.push_back(i.lock()->from()->name());
	}
	return result;
}

template <typename E, typename V, typename INFO>
std::vector<V> graph<E, V, INFO>::v_from_v(V id) const {
	std::vector<V> result;
	for (auto i : _vertecies.at(id)->_out) {
		result.push_back(i.lock()->to()->name());
	}
	return result;
}

template <typename E, typename V, typename INFO>
std::vector<V> graph<E, V, INFO>::vertecies() const {
	std::vector<V> result;
	for (auto i : _vertecies) {
		result.push_back(i.first);
	}
	return result;
}

template <typename E, typename V, typename INFO>
std::vector<typename graph<E, V, INFO>::edge> graph<E, V, INFO>::e_to_v(V id) const {
	std::vector<edge> result;
	for (auto i : _vertecies.at(id)->_in) {
		result.push_back(*(i.lock()));
	}
	return result;
}

template <typename E, typename V, typename INFO>
std::vector<typename graph<E, V, INFO>::edge> graph<E, V, INFO>::e_from_v(V id) {
	std::vector<edge> result;
	for (auto i : _vertecies.at(id)->_out) {
		result.push_back(*(i.lock()));
	}
	return result;
}

template <typename E, typename V, typename INFO>
std::vector<typename graph<E, V, INFO>::edge> graph<E, V, INFO>::edges() const {
	std::vector<edge> result;
	for (auto i : _edges) {
		result.push_back(*i);
	}
	return result;
}

template <typename E, typename V, typename INFO>
void graph<E, V, INFO>::push_edge(V from, V to, E weight)
{
	std::shared_ptr<edge> e (new edge);
	e->_weight = weight;

	if (_vertecies.count(from) == 0) _vertecies[from] = std::shared_ptr<vertex> (new vertex);
	if (_vertecies.count(to) == 0) _vertecies[to] = std::shared_ptr<vertex> (new vertex);

	e->_from = _vertecies[from];
	e->_to = _vertecies[to];

	if (_edges.insert(e).second) {
		_vertecies[from]->_out.push_back(e);
		_vertecies[to]->_in.push_back(e);
	}

	_vertecies[from]->_name = from;
	_vertecies[to]->_name = to;
}

template <typename E, typename V, typename INFO>
void graph<E, V, INFO>::push_vertex(V id)
{
	_vertecies[id] = std::shared_ptr<vertex> (new vertex);
}

#include "iterators/dfs.h"
#include "iterators/full_dfs.h"
#include "iterators/bfs.h"

#endif