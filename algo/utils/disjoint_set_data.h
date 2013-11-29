#include <unordered_map>
#include <vector>

template <typename T>
class disjoint_set_data
{
	std::unordered_map<T, std::pair<T, size_t>> elements; //first - parent; second - depth

public:
	disjoint_set_data(std::vector<T> data) {
		for (auto i : data) insert(i);
	}

	void insert (T v) {
		elements[v].first = v;
		elements[v].second = 0;
	}
	 
	T find (T v) {
		if (v == elements.at(v).first) return v;
		return elements.at(v).first = find(elements.at(v).first);
	}
	 
	bool merge (T a, T b) { //false - a и b лежат в одном множестве (все осталось на своих местах), true - иначе (слияние всмысле изменения было)
		T aRoot = find(a);
		T bRoot = find(b);

		if (aRoot != bRoot) {
			if (elements[aRoot].second < elements[bRoot].second) {
				elements[aRoot].first = bRoot;
			} else if (elements[aRoot].second > elements[bRoot].second) {
				elements[bRoot].first = aRoot;
			} else {
				elements[bRoot].first = aRoot;
				++(elements[aRoot].second);
			}
			return true;
		} else {
			return false;
		}
	}
};
