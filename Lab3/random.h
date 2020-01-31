#ifndef RG3_RANDOM_H
#define RG3_RANDOM_H

#include <random>
#include <functional>

inline double random_double() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    static std::function<double()> rand_generator = std::bind(distribution, generator);
    return rand_generator();
}

#endif //RG3_RANDOM_H
