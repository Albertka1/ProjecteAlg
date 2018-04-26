#ifndef utils_file_io_hpp
#define utils_file_io_hpp

#include <fstream>
#include <string>
#include <vector>

#include "string_cast.hpp"

namespace utils {
	template <class T>
	int write_f(const std::string& filename, const std::vector<T>& v){
		std::ofstream fs = std::ofstream(filename);
		if (!fs.is_open()) return -1;
		
		for (T elem : v) fs << utils::to_string(elem) << std::endl;
		fs.close();
		return 0;
	}
	
	template <class T>
	int read_f(const std::string& filename, std::vector<T>& v) {
		std::ifstream fs(filename);
		std::string elem;
		if (!fs.is_open()) return -1;

		while (getline(fs, elem)) v.push_back(utils::from_string<T>(elem));
		fs.close();
		return 0;
	}
}

#endif