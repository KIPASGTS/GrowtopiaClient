#pragma once
#include <random>

#include "../Include/randutils.hpp"

namespace Utilities {
    namespace Random {
        template<typename T = int>
        T RandomGen(T min, T max) {
            static std::mt19937 rng(randutils::mt19937_rng{}.engine());
            static randutils::uniform_distribution<T> distribution(min, max);
            return T(distribution(rng));
        }
    } // namespace Random
} // namespace Utilities