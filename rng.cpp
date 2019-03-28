#include "is_prime.h"
#include <algorithm>
#include <iostream>
#include <unordered_set>
#include <vector>
#include <random>
#include <chrono>
#include <climits>

inline constexpr uint32_t clp2(uint32_t val) {
    return 1 << (sizeof(uint32_t) * CHAR_BIT - __builtin_clz(val - 1));
}

inline constexpr uint32_t flp2(uint32_t val) {
    return 1 << (sizeof(uint32_t) * CHAR_BIT - __builtin_clz(val) - 1);
}

inline constexpr uint32_t log2(uint32_t val) {
    return sizeof(uint32_t) * CHAR_BIT - 1 - __builtin_clz(val);
}

static constexpr uint32_t N[] = {
    0, 1, 2, 3,
    4, 5, 6, 7,
    8, 9, 10, 11,
    12, 13, 14, 15,
    16, 17, 18, 19,
    20, 21, 22, 23,
    24, 25, 26, 27,
    28, 29, 30, 31,
};

static constexpr uint32_t K[] = {
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

static constexpr auto check_mod_four = [](bool v, uint32_t n, uint32_t k) -> bool {
    uint32_t p = (1 << n) - k;
    return v && (p == 1 || p == 2 || p % 4 == 3);
};

static constexpr auto check_is_prime = [](bool v, uint32_t n, uint32_t k) -> bool {
    uint32_t p = (1ull << n) - k;
    return v && (p == 1 || miller::is_prime(p));
};

static_assert(std::size(N) == std::size(K));
static_assert(double_accumulate(std::begin(N), std::end(N), std::begin(K), true, check_mod_four));
static_assert(double_accumulate(std::begin(N), std::end(N), std::begin(K), true, check_is_prime));

class quadratic_residue {
    uint32_t m_seed;
    std::size_t m_i;
    uint64_t m_n, m_k, m_pt, m_p, m_hp;

    uint64_t m_lo_mask;

public:
    explicit quadratic_residue(uint32_t cycle, uint32_t seed = 0);

    uint32_t residual_map(uint32_t x);
    uint32_t shuffled_residual(uint32_t x);

    uint32_t operator()(uint32_t x) { return shuffled_residual(x); }
};

quadratic_residue::quadratic_residue(uint32_t cycle, uint32_t seed) :
    m_seed(seed),
    m_i(log2(flp2(cycle))),
    m_n(N[m_i]),
    m_k(K[m_i]),
    m_pt(1 << m_n),
    m_p(m_pt - m_k),
    m_hp(m_p >> 1),
    m_lo_mask(m_pt - 1) {}

uint32_t quadratic_residue::residual_map(uint32_t x) {
    if (x >= m_p) {
        return x;
    }
    uint64_t xsq = static_cast<uint64_t>(x) * x;
    uint64_t h = m_k * (xsq >> m_n) + (xsq & m_lo_mask);
    uint64_t j = m_k * (h >> m_n) + (h & m_lo_mask);
    if (j >= m_p) {
        j -= m_p;
    }

    uint32_t residue = static_cast<uint64_t>(j);
    return x <= m_hp ? residue : m_p - residue;
}

uint32_t quadratic_residue::shuffled_residual(uint32_t x) {
    constexpr uint32_t magic = 0x5bf03635;
    return residual_map(((residual_map(x) + m_seed) ^ magic) % m_pt);
}

int main() {
}
