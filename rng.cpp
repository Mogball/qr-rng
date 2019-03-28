#include "is_prime.h"
#include <algorithm>
#include <iostream>
#include <vector>

static constexpr uint64_t N[] = {
    0, 1, 2, 3,
    4, 5, 6, 7,
    8, 9, 10, 11,
    12, 13, 14, 15,
    16, 17, 18, 19,
    20, 21, 22, 23,
    24, 25, 26, 27,
    28, 29, 30, 31,
};

static constexpr uint64_t K[] = {
    0, 0, 1, 1,
    5, 1, 5, 1,
    5, 9, 5, 9,
    5, 1, 21, 49,
    17, 1, 5, 1,
    5, 9, 17, 21,
    17, 49, 5, 781,
    57, 33, 41, 1,
};


template<typename InputIt0, typename InputIt1, typename T, typename TrinaryOp>
static constexpr bool double_accumulate(
        InputIt0 it0,
        InputIt0 end,
        InputIt1 it1,
        T init,
        TrinaryOp op) {
    for (; it0 != end; ++it0, ++it1) {
        init = op(std::move(init), *it0, *it1);
    }
    return init;
}

static constexpr auto check_mod_four = [](bool v, uint64_t n, uint64_t k) -> bool {
    uint64_t p = (1 << n) - k;
    return v && (p == 1 || p == 2 || p % 4 == 3);
};

static constexpr auto check_is_prime = [](bool v, uint64_t n, uint64_t k) -> bool {
    uint64_t p = (1ull << n) - k;
    return v && (p == 1 || miller::is_prime(p));
};

static_assert(std::size(N) == std::size(K));
static_assert(double_accumulate(std::begin(N), std::end(N), std::begin(K), true, check_mod_four));
static_assert(double_accumulate(std::begin(N), std::end(N), std::begin(K), true, check_is_prime));

int main() {
    double_accumulate(std::begin(N), std::end(N), std::begin(K), true, check_is_prime);
}
