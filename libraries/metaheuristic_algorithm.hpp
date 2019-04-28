#ifndef __METAHEURISTIC_ALGORITHM_HPP__
#define __METAHEURISTIC_ALGORITHM_HPP__

#include <random>

#include "history.hpp"
#include "probable_choice.hpp"

class MetaheuristicAlgorithm {
protected :
    uint32_t _random_seed_;
    std::mt19937 _rnd_;

    Data _input_;
    Solution _solution_;

public :
    inline MetaheuristicAlgorithm(const uint32_t random_seed = std::random_device()());
    virtual void loadConfig(std::istream& is) = 0;

    inline void setInput(const Data& input);
    inline void setRandom(const uint32_t random_seed);

    virtual void start(HISTORY_STREAM) = 0;
    inline uint32_t getSeed() const;
    inline Solution getSolution() const;
};

inline MetaheuristicAlgorithm::MetaheuristicAlgorithm(const uint32_t random_seed) :
    _random_seed_(random_seed), _rnd_(_random_seed_), _input_(0), _solution_() {}

inline void MetaheuristicAlgorithm::setInput(const Data& input) {
    _input_ = input;
}

inline void MetaheuristicAlgorithm::setRandom(const uint32_t random_seed) {
    _random_seed_ = random_seed;
}

inline uint32_t MetaheuristicAlgorithm::getSeed() const {
    return _random_seed_;
}

inline Solution MetaheuristicAlgorithm::getSolution() const {
    return _solution_;
}

#endif //__METAHEURISTIC_ALGORITHM_HPP__
