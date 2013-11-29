#ifndef _DFS
#define _DFS

#include <unordered_map>
#include <stack>

#include "../graph.h"

template <typename E, typename V, typename INFO>
struct graph<E, V, INFO>::dfs
{
	graph * graphPtr;
	std::unordered_map<std::shared_ptr<vertex>, bool> state;
	std::stack<std::shared_ptr<vertex>> path;

	dfs(graph * g) : graphPtr(g) {
		if (graphPtr != NULL) {
			path.push(graphPtr->_vertecies.begin()->second);
			for (auto i : graphPtr->_vertecies) state[i.second] = 1;
			state[path.top()] = 0;
		}
	}

	dfs(graph * g, V start) : graphPtr(g) {
		if (graphPtr != NULL) {
			if (graphPtr->_vertecies.count(start) == 1) {
				path.push(graphPtr->_vertecies[start]);
				for (auto i : graphPtr->_vertecies) state[i.second] = 1;
				state[path.top()] = 0;
			} else {
				throw "There is no vertex with this key\n";
			}
		}
	}

	dfs(const dfs & iter) {
		graphPtr = iter.graphPtr;
		state = iter.state;
		path = iter.path;
	}

	dfs & operator = (const dfs & iter) const {
		graphPtr = iter.graphPtr;
		state = iter.state;
		path = iter.path;
	}

	std::shared_ptr<vertex> operator -> () const
	{
		if (path.empty()) return NULL;
		return path.top();
	}

	vertex & operator * () const
	{
		if (path.empty()) return NULL;
		return *(path.top());
	}

	dfs & operator ++ ()
	{
		if (path.empty()) return *this;

		do {
			bool flag = 1;
			for (auto i : path.top()->_out) {
				std::shared_ptr<vertex> shptr = i.lock()->to();
				if (state[shptr]) {
					path.push(shptr);
					state[path.top()] = 0;
					flag = 0;
					break;
				}
			}
			if (flag) {
				path.pop();
				if (path.empty()) {
					for (auto i : state) {
						if (i.second) {
							path.push(i.first);
							state[path.top()] = 0;
							return *this;
						}
					}
				}
			} else break;
		} while (!path.empty());

		return *this;
	}

	dfs operator ++ (int)
	{
		if (path.empty()) return *this;
		
		dfs temp = *this;

		do {
			bool flag = 1;
			for (auto i : path.top()->_out) {
				std::shared_ptr<vertex> shptr = i.lock()->to();
				if (state[shptr]) {
					path.push(shptr);
					state[path.top()] = 0;
					flag = 0;
					break;
				}
			}
			if (flag) {
				path.pop();
				if (path.empty()) {
					for (auto i : state) {
						if (i.second) {
							path.push(i.first);
							state[path.top()] = 0;
							return temp;
						}
					}
				}
			} else break;
		} while (!path.empty());
		
		return temp;
	}

	bool operator == (const dfs & iter) const
	{
		if (path.empty() || iter.path.empty()) return (path.empty() == iter.path.empty());
		return (path.top() == iter.path.top());
	}

	bool operator != (const dfs & iter) const
	{
		if (path.empty() || iter.path.empty()) return (path.empty() != iter.path.empty());
		return (path.top() != iter.path.top());
	}

	bool was(V id) const
	{
		return (!state[graphPtr->_vertecies[id]]);
	}
};

template <typename E, typename V, typename INFO>
typename graph<E, V, INFO>::dfs graph<E, V, INFO>::dfs_begin() const {
	dfs temp(this);
	return temp;
}

template <typename E, typename V, typename INFO>
typename graph<E, V, INFO>::dfs graph<E, V, INFO>::dfs_end() {
	dfs temp(NULL);
	return temp;
}

#endif
