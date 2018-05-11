#ifndef utils_display_tabular_hpp
#define utils_display_tabular_hpp

#include <string>
#include <vector>

#include "string_cast.hpp"

namespace utils {
	std::vector<std::string> add(const std::string& tab, const std::vector<std::string>& a, const std::vector<std::string>& b) {
		unsigned i;
		std::vector<std::string> v = std::vector<std::string>();
		
		for (i = 0; i < a.size() && i < b.size(); i++)
			v.push_back(a[i] + tab + b[i]);
		for (; i < a.size(); i++)
			v.push_back(a[i] + tab + "NULL");
		for (; i < b.size(); i++)
			v.push_back("NULL" + tab + b[i]);
		
		return v;
	}
	template <class cont>
	std::vector<std::string> display_tabular(const std::string& tab, const cont& c) {
		std::vector<std::string> v = std::vector<std::string>();
		for(auto elem : c) v.push_back(to_string(elem));
		return v;
	}
	template <class cont, class ... conts>
	std::vector<std::string> display_tabular(const std::string& tab, const cont& c, conts ... cts) {
		std::vector<std::string> v1 = std::vector<std::string>();
		std::vector<std::string> v2 = display_tabular(tab, cts ...);
		for(auto elem : c) v1.push_back(to_string(elem));
		return add(tab, v1, v2);
	}
	
	template <class iter>
	std::string print_line(bool* empty, const std::string& tab, iter& a, iter& b) {
		if (a == b) {
			if (empty != NULL) *empty = true;
			return "NULL";
		}
		if (empty != NULL) *empty = false;
		std::string str = utils::to_string(*a);
		a++;
		return str;
	}
	template <class iter, class ... iters>
	std::string print_line(bool* empty, const std::string& tab, iter& a, iter& b, iters& ... its) {
		std::string rest = tab + print_line(empty, tab, its ...);
		if (a == b)
			return "NULL" + rest;
		if (empty != NULL) *empty = false;
		std::string str = utils::to_string(*a);
		a++;
		return str + rest;
	}
	template <class ... iters>
	std::vector<std::string> display_tabular_it(const std::string& tab, iters ... its) {
		std::vector<std::string> v = std::vector<std::string>();

		bool empty = false;
		while (!empty) v.push_back(print_line(&empty, tab, its ...));
		v.pop_back();

		return v;
	}
}

#endif