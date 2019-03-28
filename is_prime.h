#pragma once

#include <cstdint>

namespace miller {

    constexpr uint64_t mod_add(uint64_t a, uint64_t b, uint64_t m) {
        if (~a < b) {
            return (a + b) % m + ~m + 1;
        } else {
            return a + b;
        }
    }

    constexpr uint64_t mod_power(uint64_t a, int i, uint64_t m) {
        if (i < 1) {
            return a;
        } else {
            return mod_power(mod_add(a, a, m), i - 1, m);
        }
    }

    constexpr uint64_t hiword(uint64_t x) {
        return x >> 32;
    }

    constexpr uint64_t loword(uint64_t x) {
        return x & hiword(~0);
    }

    constexpr void mod_mul(uint64_t& a, uint64_t b, uint64_t m) {
        uint64_t x = 1;
        if (m < (x << 32)) {
            a = (a % m) * (b % m);
        } else {
            x = 0;
            x = mod_add(x, hiword(a) * hiword(b), m);
            x = mod_power(x, 32, m);
            x = mod_add(x, loword(a) * hiword(b), m);
            x = mod_add(x, hiword(a) * loword(b), m);
            x = mod_power(x, 32, m);
            a = mod_add(x, loword(a) * loword(b), m);
        }
    }

    constexpr bool even(uint64_t n) {
        return n % 2 == 0;
    }

    constexpr bool miller_test(uint64_t a, uint64_t s, uint64_t d, uint64_t n) {
        uint64_t x = 1;
        while (d != 0) {
            if (even(d)) {
                mod_mul(a, a, n);
                d /= 2;
            } else {
                mod_mul(x, a, n);
                d -= 1;
            }
        }
        if (x % n == 1) {
            return true;
        }
        for (uint64_t r = 0; r < s; ++r) {
            if (x % n == n - 1) {
                return true;
            }
            mod_mul(x, x, n);
        }
        return false;
    }

    constexpr bool is_prime(uint64_t n) {
        if (n < 2 || even(n)) {
            return n == 2;
        }
        uint64_t s = 0, d = n - 1;
        while (even(d)) {
            d /= 2;
            ++s;
        }
        uint64_t x[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
        for (uint64_t a : x) {
            if (a < n && !miller_test(a, s, d, n)) {
                return false;
            }
        }
        return true;
    }

}
