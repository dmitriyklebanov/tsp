#ifndef __PROBABLE_CHOICE_HPP__
#define __PROBABLE_CHOICE_HPP__

#include "solution.hpp"

class ProbableChoice {
private :
	std::vector<std::pair<double, int>> _probabilities_psum_;
	std::uniform_real_distribution<> dist;
public :
	inline ProbableChoice() :
		dist(std::uniform_real_distribution<>(0.0, 1.0)) {}
		
	inline void setArray(const std::vector<Solution>& solutions, uint32_t n, auto& rnd);
	inline uint32_t getSolutionNumber(auto& rnd);
};

inline void ProbableChoice::setArray(const std::vector<Solution>& solutions, uint32_t n, auto& rnd) {
	_probabilities_psum_.assign(n, {0, 0});
	
	double probabilities_sum = 0;
	for (uint32_t i = 0; i < n; i++) {
		_probabilities_psum_[i].first = 1.0 / solutions[i].getCost();
		probabilities_sum += _probabilities_psum_[i].first;
	}

	for (auto& probability : _probabilities_psum_) {
		probability.first /= probabilities_sum;
	}
	std::shuffle(_probabilities_psum_.begin(), _probabilities_psum_.end(), rnd);
	
	for (uint32_t i = 1; i < n; i++) {
		_probabilities_psum_[i].first += _probabilities_psum_[i - 1].first;
		_probabilities_psum_[i].second = i;
	}
}

inline uint32_t ProbableChoice::getSolutionNumber(auto& rnd) {
	auto it = lower_bound(_probabilities_psum_.begin(), _probabilities_psum_.end(), std::pair<double, int> (dist(rnd), 0));
	return it->second;
}

#endif //__PROBABLE_CHOICE_HPP__
