#ifndef __SOLUTION_HPP__
#define __SOLUTION_HPP__

#include <cmath>
#include <ostream>
#include <algorithm>
#include <random>

#include "data.hpp"

class Solution {
private :
    Data* _input_data_;
    std::vector<uint32_t> _permutation_;
    std::uniform_int_distribution<> _dist_;

    double _cost_;
    double _ans_;

    inline double getDistance(uint32_t pos1, uint32_t pos2);

public :
    inline Solution();
    inline Solution(Data* input_data, auto& rnd);

    inline void init(auto& rnd);
    inline void setInputData(Data* input_data);

    inline void mutate(auto& rnd);

    inline double getCost() const;
    inline double getAns() const;

    friend inline std::ostream& operator <<(std::ostream& os, const Solution& value);

private :
    inline double makeCost(uint32_t pos1, uint32_t pos2);
    inline double makeAns(uint32_t pos1, uint32_t pos2);

    inline void editValue(double& value, auto makeValue, uint32_t pos1, uint32_t pos2);
    inline void generateValue(double& value, auto makeValue);

    friend bool operator ==(const Solution& a, const Solution& b);
};



inline double Solution::getDistance(uint32_t pos1, uint32_t pos2) {
    double ans = 0;
    ans += ((*_input_data_)[pos1].x - (*_input_data_)[pos2].x) *
           ((*_input_data_)[pos1].x - (*_input_data_)[pos2].x);
    ans += ((*_input_data_)[pos1].y - (*_input_data_)[pos2].y) *
           ((*_input_data_)[pos1].y - (*_input_data_)[pos2].y);

    return std::sqrt(ans);
}

inline Solution::Solution() :
    _input_data_(nullptr), _permutation_(0),
    _dist_(0, 0), _cost_(0), _ans_(0) {}

inline Solution::Solution(Data* input_data, auto& rnd) {
    setInputData(input_data); init(rnd);
}

inline void Solution::init(auto& rnd) {
    _dist_ = std::uniform_int_distribution<> (0, _input_data_->size() - 1);
    _permutation_.resize(_input_data_->size());
    for (std::size_t i = 0; i < _permutation_.size(); i++) {
        _permutation_[i] = i;
    }
    std::shuffle(_permutation_.begin(), _permutation_.end(), rnd);
    generateValue(_cost_, &Solution::makeCost);
    generateValue(_ans_, &Solution::makeAns);
}

inline void Solution::setInputData(Data* input_data) {
    _input_data_ = input_data;
}

inline void Solution::mutate(auto& rnd) {
    uint32_t pos1 = _dist_(rnd);
    uint32_t pos2 = _dist_(rnd);
    if (pos1 >= _input_data_->size()) throw "kokoko";
    if (pos2 >= _input_data_->size()) throw "kokoko";
    if (pos1 > pos2) {
        std::swap(pos1, pos2);
    }
    editValue(_cost_, &Solution::makeCost, pos1, pos2);
    editValue(_ans_, &Solution::makeAns, pos1, pos2);
    std::reverse(_permutation_.begin() + pos1, _permutation_.begin() + pos2 + 1);
}

inline double Solution::getCost() const {
    return _cost_;
}

inline double Solution::getAns() const {
    return _ans_;
}

inline double Solution::makeCost(uint32_t pos1, uint32_t pos2) {
    return getDistance(_permutation_[pos1], _permutation_[pos2]) *
           getDistance(_permutation_[pos1], _permutation_[pos2]);
}

inline double Solution::makeAns(uint32_t pos1, uint32_t pos2) {
    return getDistance(_permutation_[pos1], _permutation_[pos2]);
}

inline void Solution::editValue(double& value, auto makeValue, uint32_t pos1, uint32_t pos2) {
    uint32_t lpos1 = (pos1 + _permutation_.size() - 1) % _permutation_.size();
    uint32_t rpos2 = (pos2                        + 1) % _permutation_.size();

    if (lpos1 == pos2) {
        return;
    }

    value -= (this->*makeValue)(lpos1, pos1);
    value -= (this->*makeValue)(pos2, rpos2);

    value += (this->*makeValue)(lpos1, pos2);
    value += (this->*makeValue)(pos1, rpos2);

}

inline void Solution::generateValue(double& value, auto makeValue) {
    value = 0;
    for (std::size_t i = 1; i < _permutation_.size(); i++) {
        value += (this->*makeValue)(i, i - 1);
    }
    value += (this->*makeValue)(_permutation_.size() - 1, 0);
}

inline std::ostream& operator <<(std::ostream& os, const Solution& value) {
    for (auto v : value._permutation_) {
        os << v << " ";
    }
    return os;
}

bool operator ==(const Solution& a, const Solution& b) {
    return a._permutation_ == b._permutation_;
}

#endif //__SOLUTION_HPP__
