#ifndef _BFS
#define _BFS

#include <unordered_map>
#include <queue>

#include "../graph.h"

template <typename E, typename V, typename INFO>
struct graph<E, V, INFO>::bfs
{
	graph * graphPtr;
	std::unordered_map<std::shared_ptr<vertex>, bool> state;
	std::queue<std::shared_ptr<vertex>> order;

	bfs(graph * g) : graphPtr(g) {
		if (graphPtr != NULL) {
			order.push(graphPtr->_vertecies.begin()->second);
			for (auto i : graphPtr->_vertecies) state[i.second] = 1;
			state[graphPtr->_vertecies.begin()->second] = 0;
		}
	}

	bfs(graph * g, V start) : graphPtr(g) {
		if (graphPtr != NULL) {
			if (graphPtr->_vertecies.count(start) == 1) {
				order.push(graphPtr->_vertecies[start]);
				for (auto i : graphPtr->_vertecies) state[i.second] = 1;
				state[graphPtr->_vertecies[start]] = 0;
			} else {
				throw "There is no vertex with this key\n";
			}
		}
	}

	bfs(const bfs & iter) {
		graphPtr = iter.graphPtr;
		state = iter.state;
		order = iter.order;
	}

	bfs & operator = (const bfs & iter) const {
		graphPtr = iter.graphPtr;
		state = iter.state;
		order = iter.order;
	}

	std::shared_ptr<vertex> operator -> () const
	{
		if (order.empty()) return NULL;
		return order.front();
	}

	vertex & operator * () const
	{
		if (order.empty()) return NULL;
		return *(order.front());
	}

	bfs & operator ++ ()
	{
		if (order.empty()) return *this;

		for (auto i : order.front()->_out) {
			std::shared_ptr<vertex> child = i.lock()->to();
			if (state[child]) {
				order.push(child);
				state[child] = 0;
			}

		}
		order.pop();
		if (order.empty()) {
			for (auto i : state) {
				if (i.second) {
					order.push(i.first);
					state[i.first] = 0;
					return *this;
				}
			}
		}

		return *this;
	}

	bfs operator ++ (int)
	{
		if (order.empty()) return *this;
		
		bfs temp = *this;

		for (auto i : order.front()->_out) {
			std::shared_ptr<vertex> child = i.lock()->to();
			if (state[child]) {
				order.push(child);
				state[child] = 0;
			}
		}
		order.pop();
		if (order.empty()) {
			for (auto i : state) {
				if (i.second) {
					order.push(i.first);
					state[i.first] = 0;
					return temp;
				}
			}
		}
		
		return temp;
	}

	bool operator == (const bfs & iter) const
	{
		if (order.empty() || iter.order.empty()) return (order.empty() == iter.order.empty());
		return (order.front() == iter.order.front());
	}

	bool operator != (const bfs & iter) const
	{
		if (order.empty() || iter.order.empty()) return (order.empty() != iter.order.empty());
		return (order.front() != iter.order.front());
	}
};

template <typename E, typename V, typename INFO>
typename graph<E, V, INFO>::bfs graph<E, V, INFO>::bfs_begin() {
	bfs temp(this);
	return temp;
}

template <typename E, typename V, typename INFO>
typename graph<E, V, INFO>::bfs graph<E, V, INFO>::bfs_end() {
	bfs temp(NULL);
	return temp;
}

#endif
