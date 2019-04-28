#ifndef __CLONAL_SELECTION_HPP__
#define __CLONAL_SELECTION_HPP__

#include <algorithm>

#include "../libraries/metaheuristic_algorithm.hpp"

class ClonalSelection : public MetaheuristicAlgorithm {
private :
    uint32_t _number_of_parents_;
    uint32_t _number_of_childs_;
    uint32_t _number_of_iterations_;

public :
    inline ClonalSelection(const uint32_t random_seed = std::random_device()());
    inline void setConfig(const uint32_t number_of_parents,
                          const uint32_t number_of_childs,
                          const uint32_t number_of_iterations);

    inline void loadConfig(std::istream& is) override;
    inline void start(HISTORY_STREAM) override;
};

inline ClonalSelection::ClonalSelection(const uint32_t random_seed) :
    MetaheuristicAlgorithm(random_seed),  _number_of_parents_(10),
    _number_of_childs_(5000), _number_of_iterations_(250) {}

inline void ClonalSelection::setConfig(const uint32_t number_of_parents,
                                       const uint32_t number_of_childs,
                                       const uint32_t number_of_iterations) {
    _number_of_parents_    = number_of_parents;
    _number_of_childs_     = number_of_childs;
    _number_of_iterations_ = number_of_iterations;
}

inline void ClonalSelection::loadConfig(std::istream& is) {
    uint32_t random_seed;
    is >> random_seed;
    if (random_seed ) {
        _random_seed_ = random_seed;
        _rnd_ = std::mt19937(_random_seed_);
    }
    is >> _number_of_parents_;
    is >> _number_of_childs_;
    is >> _number_of_iterations_;
}

inline void ClonalSelection::start(HISTORY_STREAM) {
    HISTORY_START;
    std::vector<Solution> solutions(_number_of_parents_ + _number_of_childs_);
    for (uint32_t i = 0; i < _number_of_parents_; i++) {
        solutions[i].setInputData(&_input_);
        solutions[i].init(_rnd_);
    }
    HISTORY(solutions[0].getAns());
    ProbableChoice choice;
    for (uint32_t i = 1; i <= _number_of_iterations_; i++) {
        choice.setArray(solutions, _number_of_parents_, _rnd_);

        for (uint32_t c = 0; c < _number_of_childs_; c++) {
            solutions[_number_of_parents_ + c] = solutions[choice.getSolutionNumber(_rnd_)];
            solutions[_number_of_parents_ + c].mutate(_rnd_);
        }

        std::sort(solutions.begin(), solutions.end(),
            [&](const Solution& a, const Solution& b) {
                return a.getCost() < b.getCost();
            });
        uint32_t p = 1, j = 1;
        while ((p < _number_of_parents_) && (j < solutions.size())) {
            if (solutions[j] == solutions[j - 1]) {
                j++;
            }
            if (j != p) {
                solutions[p] = solutions[j];
            }
            j++; p++;
        }
        HISTORY(solutions[0].getAns());
    }
    _solution_ = solutions[0];
}

#endif //__CLONAL_SELECTION_HPP__
