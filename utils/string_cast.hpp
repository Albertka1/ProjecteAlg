#ifndef utils_string_cast_hpp
#define utils_string_cast_hpp

#include <string>
#include <vector>

namespace utils {
	template <class T> std::string to_string(const T& val) { return (std::string)val; }
	template <> std::string to_string<int>(const int& i) { return std::to_string(i); }
	template <> std::string to_string<long>(const long& i) { return std::to_string(i); }
	template <> std::string to_string<long long>(const long long& i) { return std::to_string(i); }
	template <> std::string to_string<unsigned long>(const unsigned long& i) { return std::to_string(i); }
	template <> std::string to_string<unsigned long long>(const unsigned long long& i) { return std::to_string(i); }
	template <> std::string to_string<float>(const float& i) { return std::to_string(i); }
	template <> std::string to_string<double>(const double& i) { return std::to_string(i); }
	template <> std::string to_string<long double>(const long double& i) { return std::to_string(i); }
	template <> std::string to_string<bool>(const bool& val) { return val ? "TRUE" : "FALSE"; }
	template <> std::string to_string<char>(const char& c) { std::string s = ""; s += c; return s; }
	template <> std::string to_string<std::vector<std::string>>(const std::vector<std::string>& v) {
		std::string result = "";
		for (std::string s : v)
			result += s + "\n";
		return result;
	}
	
	template <class T> T from_string(const std::string& str) { return T(str); }
	template <> bool from_string<bool>(const std::string& str) {
		if (str.size() == 0) return false;
		if (str[0] == ' ') return from_string<bool>(str.substr(1, str.size()));
		return (
			str[0] == 't' || str[0] == 'T' &&
			str[1] == 'r' || str[1] == 'R' &&
			str[2] == 'u' || str[2] == 'U' &&
			str[3] == 'e' || str[3] == 'E'
		);
	}
	template <> char from_string<char>(const std::string& str) { return str[0]; }
	template <> int from_string<int>(const std::string& str) { return std::stoi(str); }
	template <> long from_string<long>(const std::string& str) { return std::stol(str); }
	template <> long long from_string<long long>(const std::string& str) { return std::stoll(str); }
	template <> unsigned long from_string<unsigned long>(const std::string& str) { return std::stoul(str); }
	template <> unsigned long long from_string<unsigned long long>(const std::string& str) { return std::stoull(str); }
	template <> float from_string<float>(const std::string& str) { return std::stof(str); }
	template <> double from_string<double>(const std::string& str) { return std::stod(str); }
	template <> long double from_string<long double>(const std::string& str) { return std::stold(str); }
}
	
#endif
