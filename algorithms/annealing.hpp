#ifndef __ANNEALING_HPP__
#define __ANNEALING_HPP__

#include <cmath>

#include "../libraries/metaheuristic_algorithm.hpp"

class Annealing : public MetaheuristicAlgorithm {
private :
    double _min_temperature_;
    double _temperature_change_;
    double _max_temperature_;

    inline double getProbability(double delta_cost, double temperature);
    inline bool isTransition(double delta_cost, double temperature);
public :
    inline Annealing(const uint32_t random_seed = std::random_device()());
    inline void setConfig(const uint32_t number_of_parents,
                          const uint32_t number_of_childs,
                          const uint32_t number_of_iterations);

    inline void loadConfig(std::istream& is) override;
    inline void start(HISTORY_STREAM) override;
};

inline double Annealing::getProbability(double delta_cost, double temperature) {
    return std::exp(-delta_cost / temperature);
}

inline bool Annealing::isTransition(double delta_cost, double temperature) {
    if (delta_cost < 0) {
        return true;
    }
    double p = getProbability(delta_cost, temperature);
    p *= UINT32_MAX;
    return (_rnd_() <= p);
}

inline Annealing::Annealing(const uint32_t random_seed) :
    MetaheuristicAlgorithm(random_seed), _min_temperature_(5e-5),
    _temperature_change_(1), _max_temperature_(10) {}

inline void Annealing::loadConfig(std::istream& is) {
    uint32_t random_seed;
    is >> random_seed;
    if (random_seed ) {
        _random_seed_ = random_seed;
        _rnd_ = std::mt19937(_random_seed_);
    }
    is >> _min_temperature_;
    is >> _temperature_change_;
    is >> _max_temperature_;
}

inline void Annealing::start(HISTORY_STREAM) {
    HISTORY_START;
    _solution_.setInputData(&_input_);
    _solution_.init(_rnd_);
    Solution cur = _solution_;
    HISTORY(_solution_.getAns());
    double temperature = _max_temperature_;
    uint32_t i = 0;
    while (temperature > _min_temperature_) {
        cur.mutate(_rnd_);
        double delta = (cur.getCost() - _solution_.getCost()) / _solution_.getCost();
        if (isTransition(delta, temperature)) {
            if (_solution_.getCost() > cur.getCost()) {
                _solution_ = cur;
            }
        } else {
            cur = _solution_;
        }
        temperature = _max_temperature_ * _temperature_change_ / i;
        i++; HISTORY(_solution_.getAns());
    }
}

#endif //__ANNEALING_HPP__
