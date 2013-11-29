#ifndef _FULL_DFS
#define _FULL_DFS

#include <unordered_map>
#include <stack>

#include "../graph.h"

template <typename E, typename V, typename INFO>
struct graph<E, V, INFO>::full_dfs
{
    graph * graphPtr;
    std::unordered_map<std::shared_ptr<vertex>, unsigned char> state;
    std::stack<std::shared_ptr<vertex>> path;

    full_dfs(graph * g) : graphPtr(g) {
        if (graphPtr != NULL) {
            path.push(graphPtr->_vertecies.begin()->second);
            for (auto i : graphPtr->_vertecies) state[i.second] = 0;
        	state[graphPtr->_vertecies.begin()->second] = 1;
        }
    }

    full_dfs(graph * g, V start) : graphPtr(g) {
        if (graphPtr != NULL) {
            if (graphPtr->_vertecies.count(start) == 1) {
                path.push(graphPtr->_vertecies[start]);
                for (auto i : graphPtr->_vertecies) state[i.second] = 0;
                state[graphPtr->_vertecies[start]] = 1;
            } else {
                throw "There is no vertex with this key\n"; //заменить на исключение
            }
        }
    }

    full_dfs(const full_dfs & iter) {
        graphPtr = iter.graphPtr;
        state = iter.state;
        path = iter.path;
    }

    full_dfs & operator = (const full_dfs & iter) const {
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

    full_dfs & operator ++ ()
    {
        if (path.empty()) return *this;

        if (state[path.top()] == 1) {
            for (auto i : path.top()->_out) {
                std::shared_ptr<vertex> shptr = i.lock()->to();
                if (state[shptr] == 0) {
                    path.push(shptr);
                    state[shptr] = 1;
                    return *this;
                }
            }
        	state[path.top()] = 2;
        	return *this;
        } else {
        	path.pop();
    		if (path.empty()) {
				for (auto i : state) {
					if (i.second == 0) {
						path.push(i.first);
						state[path.top()] = 1;
						return *this;
					}
				}
				return *this;
			}
        	for (auto i : path.top()->_out) {
                std::shared_ptr<vertex> shptr = i.lock()->to();
                if (state[shptr] == 0) {
                    path.push(shptr);
                    state[shptr] = 1;
                    return *this;
                }
            }
        	state[path.top()] = 2;
        	return *this;
        }
    }

    full_dfs operator ++ (int)
    {
        if (path.empty()) return *this;
        
        full_dfs temp = *this;
        
        if (state[path.top()] == 1) {
            for (auto i : path.top()->_out) {
                std::shared_ptr<vertex> shptr = i.lock()->to();
                if (state[shptr] == 0) {
                    path.push(shptr);
                    state[shptr] = 1;
                    return temp;
                }
            }
        	state[path.top()] = 2;
        	return temp;
        } else {
        	path.pop();
        	if (path.empty()) {
				for (auto i : state) {
					if (i.second == 0) {
						path.push(i.first);
						state[path.top()] = 1;
						return temp;
					}
				}
				return temp;
			}
        	for (auto i : path.top()->_out) {
                std::shared_ptr<vertex> shptr = i.lock()->to();
                if (state[shptr] == 0) {
                    path.push(shptr);
                    state[shptr] = 1;
                    return temp;
                }
            }
        	state[path.top()] = 2;
        	return temp;
        }
    }

    bool operator == (const full_dfs & iter) const
    {
        if (path.empty() || iter.path.empty()) return (path.empty() == iter.path.empty());
        return (path.top() == iter.path.top());
    }

    bool operator != (const full_dfs & iter) const
    {
        if (path.empty() || iter.path.empty()) return (path.empty() != iter.path.empty());
        return (path.top() != iter.path.top());
    }

    unsigned char color()
    {
    	return state[path.top()];
    }
};

template <typename E, typename V, typename INFO>
typename graph<E, V, INFO>::full_dfs graph<E, V, INFO>::full_dfs_begin() {
    full_dfs temp(this);
    return temp;
}

template <typename E, typename V, typename INFO>
typename graph<E, V, INFO>::full_dfs graph<E, V, INFO>::full_dfs_end() {
    full_dfs temp(NULL);
    return temp;
}

#endif
