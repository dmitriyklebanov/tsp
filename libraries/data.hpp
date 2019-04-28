#ifndef __DATA_HPP__
#define __DATA_HPP__

#include <vector>
#include <istream>
#include <cstdint>

struct Point {
	double x, y;
};

typedef std::vector<Point> Data;

inline std::istream& operator >>(std::istream& is, Data& value) {
	uint32_t number_of_points = 0;
	is >> number_of_points;
	value.resize(number_of_points);
	for (auto& v : value) {
		is >> v.x >> v.y;
	}
	return is;
}

#endif //__DATA_HPP__


