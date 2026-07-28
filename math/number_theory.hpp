/**
 * @file number_theory.hpp
 * @brief 整数論
 * @author m1ffyz
 * @date 2026-07-28
 * @see https://github.com/m1ffyz/library/blob/main/math/number_theory.hpp
 */

namespace number_theory {
    namespace internal {
        using u64 = unsigned long long;
        using u128 = __uint128_t;

        static u64 multiply_mod(u64 a, u64 b, u64 mod) {
            return (u64) ((u128) a * b % mod);
        }

        static u64 power_mod(u64 a, u64 n, u64 mod) {
            u64 res = 1;
            while (n > 0) {
                if (n & 1) {
                    res = multiply_mod(res, a, mod);
                }
                a = multiply_mod(a, a, mod);
                n >>= 1;
            }
            return res;
        }

        static bool miller_rabin(u64 n) {
            if (n < 2) return false;

            static const u64 small_primes[] = {
                2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37
            };
            for (u64 p : small_primes) {
                if (n % p == 0) {
                    return n == p;
                }
            }

            u64 d = n - 1;
            int s = 0;
            while ((d & 1) == 0) {
                s ++;
                d >>= 1;
            }

            static const u64 bases[] = {
                2, 325, 9375, 28178, 450775, 9780504, 1795265022
            };
            for (u64 a : bases) {
                if (a % n == 0) continue;

                u64 x = power_mod(a % n, d, n);
                if (x == 1 || x == n - 1) continue;

                bool is_composite = true;
                for (int r = 1; r < s; r ++) {
                    x = multiply_mod(x, x, n);
                    if (x == n - 1) {
                        is_composite = false;
                        break;
                    }
                }

                if (is_composite) return false;
            }

            return true;
        }

        static u64 pollard_rho(u64 n) {
            if (n % 2 == 0) return 2;
            if (n % 3 == 0) return 3;

            static mt19937_64 engine(
                chrono::steady_clock::now().time_since_epoch().count()
            );

            while (true) {
                u64 c = uniform_int_distribution<u64>(1, n - 1)(engine);
                u64 x = uniform_int_distribution<u64>(0, n - 1)(engine);
                u64 y = x;
                u64 divisor = 1;

                auto f = [n, c](u64 value) -> u64 {
                    return (u64) (((u128) multiply_mod(value, value, n) + c) % n);
                };

                while (divisor == 1) {
                    x = f(x);
                    y = f(f(y));
                    u64 difference = x > y ? x - y : y - x;
                    divisor = std::gcd(difference, n);
                }

                if (divisor != n) {
                    return divisor;
                }
            }
        }

        static void factorize_rec(u64 n, vector<u64> &factors) {
            if (n == 1) return;
            if (miller_rabin(n)) {
                factors.push_back(n);
                return;
            }

            u64 divisor = pollard_rho(n);
            factorize_rec(divisor, factors);
            factorize_rec(n / divisor, factors);
        }
    }

    // 素因数分解 (試し割り法)
    static vector<pair<long long, long long>> factorize(long long n) {
        vector<pair<long long, long long>> res;
        for (long long p = 2; p <= n / p; p ++) {
            if (n % p == 0) {
                long long count = 0;
                while (n % p == 0) {
                    count ++;
                    n /= p;
                }
                res.push_back({p, count});
            }
        }

        if (n > 1) {
            res.push_back({n, 1});
        }

        return res;
    }

    // 約数列挙 (試し割り法)
    static vector<long long> divisors(long long n) {
        vector<long long> res;
        for (long long i = 1; i <= n / i; i ++) {
            if (n % i == 0) {
                res.push_back(i);
                if (i * i != n) {
                    res.push_back(n / i);
                }
            }
        }

        sort(res.begin(), res.end());
        return res;
    }

    // 素数判定 (Miller-Rabin 法)
    static bool is_prime(long long n) {
        if (n < 2) return false;
        return internal::miller_rabin((internal::u64) n);
    }

    // 素因数分解 (Pollard's rho 法)
    static vector<pair<long long, long long>> factorize_pollard_rho(long long n) {
        if (n <= 1) return {};

        vector<internal::u64> factors;
        internal::factorize_rec((internal::u64) n, factors);
        sort(factors.begin(), factors.end());

        vector<pair<long long, long long>> res;
        for (internal::u64 p : factors) {
            if (res.empty() || res.back().first != (long long) p) {
                res.push_back({(long long) p, 1});
            } else {
                res.back().second ++;
            }
        }

        return res;
    }

    struct Eratosthenes {
        int MAX_N;
        vector<bool> is_prime;
        vector<int> primes;
        vector<int> spf; // 最小素因数
        vector<int> mu; // Möbius 関数 μ(n)
        vector<int> phi; // Euler 関数 Φ(n)

        Eratosthenes(int n) : MAX_N(n), is_prime(n + 1, true), spf(n + 1), mu(n + 1, 1), phi(n + 1) {
            assert(n >= 0);
            for (int i = 0; i <= n; i ++) {
                phi[i] = i;
                spf[i] = i;
            }
            is_prime[0] = false;
            spf[0] = 1;
            mu[0] = 0;
            if (n >= 1) {
                is_prime[1] = false;
                spf[1] = 1;
                mu[1] = 1;
            }

            for (int p = 2; p <= n; p ++) {
                if (is_prime[p]) {
                    primes.push_back(p);
                    for (int j = p; j <= n; j += p) {
                        if (j > p) {
                            is_prime[j] = false;
                        }
                        if (spf[j] == j) {
                            spf[j] = p;
                        }

                        phi[j] -= phi[j] / p;

                        if ((j / p) % p == 0) {
                            mu[j] = 0;
                        } else {
                            mu[j] = -mu[j];
                        }
                    }
                }
            }
        }

        // 素因数分解 (OSA-K 法)
        vector<pair<int, int>> factorize(int x) {
            vector<pair<int, int>> res;
            while (x > 1) {
                int p = spf[x];
                int count = 0;
                while (x % p == 0) {
                    count ++;
                    x /= p;
                }
                res.push_back({p, count});
            }
            return res;
        }

        // 約数列挙 (OSA-K 法)
        vector<int> divisors(int x) {
            vector<pair<int, int>> factors = factorize(x);
            vector<int> res = {1};

            for (auto [p, e] : factors) {
                int sz = (int) res.size();
                int p_pow = 1;
                for (int i = 0; i < e; i ++) {
                    p_pow *= p;
                    for (int j = 0; j < sz; j ++) {
                        res.push_back(res[j] * p_pow);
                    }
                }
            }

            sort(res.begin(), res.end());
            return res;
        }

        long long count_divisors(int x) {
            long long res = 1;
            for (auto [p, e] : factorize(x)) {
                res *= (e + 1);
            }
            return res;
        }

        long long sum_divisors(int x) {
            long long res = 1;
            for (auto [p, e] : factorize(x)) {
                long long sum = 0;
                long long p_pow = 1;
                for (int i = 0; i <= e; i ++) {
                    sum += p_pow;
                    p_pow *= p;
                }
                res *= sum;
            }
            return res;
        }

        bool get_is_prime(int x) {
            return is_prime[x];
        }

        int get_mu(int x) {
            return mu[x];
        }

        int get_phi(int x) {
            return phi[x];
        }
    };

    // 区間 [min_n, max_n] の素数を列挙する区間篩
    struct SegmentedEratosthenes {
        long long MIN_N;
        long long MAX_N;
        vector<bool> is_prime;
        vector<long long> primes;

        SegmentedEratosthenes(long long min_n, long long max_n) : MIN_N(min_n), MAX_N(max_n) {
            assert(0 <= MIN_N && MIN_N <= MAX_N);
            is_prime.assign((size_t) (MAX_N - MIN_N) + 1, true);

            long long limit = (long long) sqrtl((long double) MAX_N);
            while (limit + 1 <= MAX_N / (limit + 1)) {
                limit ++;
            }
            while (limit > 0 && limit > MAX_N / limit) {
                limit --;
            }

            vector<bool> base_is_prime((size_t) limit + 1, true);
            base_is_prime[0] = false;
            if (limit >= 1) base_is_prime[1] = false;

            for (long long p = 2; p <= limit / p; p ++) {
                if (!base_is_prime[p]) continue;
                for (long long multiple = p * p; multiple <= limit; multiple += p) {
                    base_is_prime[multiple] = false;
                }
            }

            if (MIN_N == 0) {
                is_prime[0] = false;
            }
            if (MIN_N <= 1 && 1 <= MAX_N) {
                is_prime[1 - MIN_N] = false;
            }

            for (long long p = 2; p <= limit; p ++) {
                if (!base_is_prime[p]) continue;

                long long start = MIN_N / p * p;
                if (start < MIN_N) {
                    if (start > MAX_N - p) continue;
                    start += p;
                }
                start = max(start, p * p);

                for (long long multiple = start; multiple <= MAX_N; multiple += p) {
                    is_prime[multiple - MIN_N] = false;
                    if (multiple > MAX_N - p) break;
                }
            }

            for (long long n = max(2LL, MIN_N); n <= MAX_N; n ++) {
                if (is_prime[n - MIN_N]) {
                    primes.push_back(n);
                }
                if (n == MAX_N) break;
            }
        }

        bool get_is_prime(long long x) {
            assert(MIN_N <= x && x <= MAX_N);
            return is_prime[x - MIN_N];
        }
    };
}
