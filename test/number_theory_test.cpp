#include <bits/stdc++.h>
using namespace std;

#include "../math/number_theory.hpp"

void test_trial_division() {
    const vector<pair<long long, long long>> factors = {
        {2, 3},
        {3, 2},
        {5, 1}
    };
    assert(number_theory::factorize(360) == factors);

    const vector<long long> divisors = {
        1, 2, 3, 4, 5, 6, 8, 9, 10, 12, 15, 18,
        20, 24, 30, 36, 40, 45, 60, 72, 90, 120,
        180, 360
    };
    assert(number_theory::divisors(360) == divisors);
}

void test_eratosthenes_zero() {
    number_theory::Eratosthenes sieve(0);

    assert(!sieve.get_is_prime(0));
    assert(sieve.primes.empty());
    assert(sieve.spf[0] == 1);
    assert(sieve.get_mu(0) == 0);
    assert(sieve.get_phi(0) == 0);
}

void test_eratosthenes_one() {
    number_theory::Eratosthenes sieve(1);

    assert(!sieve.get_is_prime(0));
    assert(!sieve.get_is_prime(1));
    assert(sieve.primes.empty());
    assert(sieve.spf[0] == 1);
    assert(sieve.spf[1] == 1);
    assert(sieve.get_mu(0) == 0);
    assert(sieve.get_mu(1) == 1);
    assert(sieve.get_phi(0) == 0);
    assert(sieve.get_phi(1) == 1);
}

void test_eratosthenes_divisors() {
    number_theory::Eratosthenes sieve(50000);

    const vector<int> prime = {1, 46349};
    assert(sieve.divisors(46349) == prime);

    const vector<int> composite = {
        1, 2, 3, 4, 5, 6, 8, 9, 10, 12, 15, 18,
        20, 24, 30, 36, 40, 45, 60, 72, 90, 120,
        180, 360
    };
    assert(sieve.divisors(360) == composite);

    const vector<int> prime_power = {
        1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024
    };
    assert(sieve.divisors(1024) == prime_power);
}

void test_miller_rabin() {
    assert(!number_theory::is_prime(-1));
    assert(!number_theory::is_prime(0));
    assert(!number_theory::is_prime(1));
    assert(number_theory::is_prime(2));
    assert(number_theory::is_prime(37));
    assert(!number_theory::is_prime(561));
    assert(!number_theory::is_prime(341550071728321LL));
    assert(number_theory::is_prime(2305843009213693951LL));
    assert(number_theory::is_prime(9223372036854775783LL));
    assert(!number_theory::is_prime(9223372036854775807LL));
}

void test_pollard_rho() {
    assert(number_theory::factorize_pollard_rho(1).empty());

    const vector<pair<long long, long long>> power_of_two = {
        {2, 62}
    };
    assert(number_theory::factorize_pollard_rho(1LL << 62) == power_of_two);

    const long long semiprime = 1000000007LL * 1000000009LL;
    const vector<pair<long long, long long>> semiprime_factors = {
        {1000000007LL, 1},
        {1000000009LL, 1}
    };
    assert(number_theory::factorize_pollard_rho(semiprime) == semiprime_factors);

    const vector<pair<long long, long long>> max_factors = {
        {7, 2},
        {73, 1},
        {127, 1},
        {337, 1},
        {92737, 1},
        {649657, 1}
    };
    assert(number_theory::factorize_pollard_rho(LLONG_MAX) == max_factors);
}

void test_segmented_eratosthenes() {
    number_theory::SegmentedEratosthenes sieve(0, 30);
    const vector<long long> primes = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29
    };
    assert(sieve.primes == primes);
    for (long long n = 0; n <= 30; n ++) {
        assert(sieve.get_is_prime(n) ==
               binary_search(primes.begin(), primes.end(), n));
    }

    number_theory::SegmentedEratosthenes singleton(2, 2);
    assert(singleton.get_is_prime(2));
    assert(singleton.primes == vector<long long>{2});
}

int main() {
    test_trial_division();
    test_eratosthenes_zero();
    test_eratosthenes_one();
    test_eratosthenes_divisors();
    test_miller_rabin();
    test_pollard_rho();
    test_segmented_eratosthenes();
}
