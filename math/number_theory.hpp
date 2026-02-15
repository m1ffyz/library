/**
 * @file number_theory.hpp
 * @brief 整数論
 * @author m1ffyz
 * @date 2026-02-15
 * @see https://github.com/m1ffyz/library/blob/main/math/number_theory.hpp
 */

namespace number_theory {
    // 素因数分解 (試し割り法)
    static vector<pair<long long, long long>> factorize(long long n) {
        vector<pair<long long, long long>> res;
        for (long long p = 2; p * p <= n; p ++) {
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
        for (long long i = 1; i * i <= n; i ++) {
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

    struct Eratosthenes {
        int MAX_N;
        vector<bool> is_prime;
        vector<int> primes;
        vector<int> spf; // 最小素因数
        vector<int> mu; // Möbius 関数 μ(n)
        vector<int> phi; // Euler 関数 Φ(n)

        Eratosthenes(int n) : MAX_N(n), is_prime(n + 1, true), spf(n + 1), mu(n + 1, 1), phi(n + 1) {
            is_prime[0] = false;
            is_prime[1] = false;
            spf[0] = 1;
            spf[1] = 1;
            mu[0] = 0;
            mu[1] = 1;
            for (int i = 0; i <= n; i ++) {
                phi[i] = i;
                spf[i] = i;
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
                int sz = res.size();
                int p_pow = p;
                for (int i = 0; i < e; i ++) {
                    for (int j = 0; j < sz; j ++) {
                        res.push_back(res[j] * p_pow);
                    }
                    p_pow *= p;
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
}
