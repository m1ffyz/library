/**
 * @file fraction.hpp
 * @brief 分数の構造体
 * @author m1ffyz
 * @date 2026-02-03
 * @see https://github.com/m1ffyz/library/blob/main/math/fraction.hpp
 */

__int128_t gcd(__int128_t a, __int128_t b) {
    if (a < 0) {
        a = -a;
    }
    if (b < 0) {
        b = -b;
    }

    while (b != 0) {
        __int128_t tmp = a % b;
        a = b;
        b = tmp;
    }

    return a;
}

struct frac {
    long long num, den;

    frac(__int128_t n = 0, __int128_t d = 1) {
        normalize(n, d);
    }

    void normalize(__int128_t n, __int128_t d) {
        assert(!(n == 0 and d == 0));

        if (d == 0) {
            den = 0;
            if (n > 0) {
                num = 1;
            } else {
                num = -1;
            }
            return;
        }

        if (den < 0) {
            n = -n;
            d = -d;
        }

        __int128_t g = gcd(n, d);

        num = (long long) n / g;
        den = (long long) d / g;
    }

    frac operator+(const frac &other) const {
        if (den == 0 or other.den == 0) {
            return handle_inf_op(other, '+');
        }
        return frac((__int128_t) num * other.den + (__int128_t) other.num * den, (__int128_t) den * other.den);
    }

    frac operator-(const frac &other) const {
        if (den == 0 or other.den == 0) {
            return handle_inf_op(other, '-');
        }
        return frac((__int128_t) num * other.den - (__int128_t) other.num * den, (__int128_t) den * other.den);
    }

    frac operator*(const frac &other) const {
        if (den == 0 or other.den == 0) {
            return handle_inf_op(other, '*');
        }
        return frac((__int128_t) num * other.num, (__int128_t) den * other.den);
    }

    frac operator/(const frac &other) const {
        if (den == 0 or other.den == 0) {
            return handle_inf_op(other, '/');
        }
        return frac((__int128_t) num * other.den, (__int128_t) den * other.num);
    }

    frac operator+= (const frac &other) {
        *this = *this + other;
        return *this;
    }

    frac operator-= (const frac &other) {
        *this = *this - other;
        return *this;
    }

    frac operator*= (const frac &other) {
        *this = *this * other;
        return *this;
    }

    frac operator/= (const frac &other) {
        *this = *this / other;
        return *this;
    }

    frac operator-() const {
        return frac(-num, den);
    }

    frac inv() const {
        return frac(den, num);
    }

    frac abs() const {
        return frac(std::abs(num), den);
    }

    int sign() const {
        if (num > 0) {
            return 1;
        } else if (num < 0) {
            return -1;
        } else {
            return 0;
        }
    }

    bool operator<(const frac &other) const {
        if (den == 0 and other.den == 0) {
            return num < other.num;
        }
        return (__int128_t) num * other.den < (__int128_t) other.num * den;
    }

    bool operator>(const frac &other) const {
        return other < *this;
    }

    bool operator<= (const frac &other) const {
        return !(*this > other);
    }

    bool operator>= (const frac &other) const {
        return !(*this < other);
    }

    bool operator== (const frac &other) const {
        return num == other.num and den == other.den;
    }

    bool operator!= (const frac &other) const {
        return !(*this == other);
    }

    long double to_decimal() const {
        if (den == 0) {
            if (num > 0) {
                return 1e20;
            } else {
                return -1e20;
            }
        } else {
            return (long double) num / den;
        }
    }

    long long floor() const {
        if (den == 0) {
            if (num > 0) {
                return LONG_LONG_MAX;
            } else {
                return LONG_LONG_MIN;
            }
        }
        long long res = num / den;
        if (num % den != 0 and num < 0) {
            res --;
        }
        return res;
    }

    long long ceil() const {
        if (den == 0) {
            if (num > 0) {
                return LONG_LONG_MAX;
            } else {
                return LONG_LONG_MIN;
            }
        }
        long long res = num / den;
        if (num % den != 0 and num > 0) {
            res ++;
        }
        return res;
    }

    long long mod(long long MOD) const {
        assert(den != 0);
        auto modpow = [&](long long a, long long b, long long m) {
            long long res = 1;
            a %= m;
            while (b > 0) {
                if (b & 1) {
                    res = (__int128_t) res * a % m;
                }
                a = (__int128_t) a * a % m;
                b >>= 1;
            }
            return res;
        };

        long long n_mod = (num % MOD + MOD) % MOD;
        long long d_inv = modpow(den, MOD - 2, MOD);
        return (__int128_t) n_mod * d_inv % MOD;
    }

    friend std::ostream& operator<<(std::ostream& os, const frac& f) {
        if (f.den == 0) {
            if (f.num > 0) {
                os << "Inf";
            } else {
                os << "-inf";
            }
        } else if (f.den == 1) {
            os << f.num;
        } else {
            os << f.num << "/" << f.den;
        }
        return os;
    }

private:
    frac handle_inf_op(const frac &other, char op) const {
        if (op == '+') {
            if (den == 0 and other.den == 0 and num != other.num) {
                return frac(0, 0);
            }
            if (den == 0) {
                return *this;
            }
            return other;
        }
        if (op == '-') {
            if (den == 0) {
                return *this;
            }
            return frac(-other.num, other.den);
        }
        if (op == '*' or op == '/') {
            int s = sign() * other.sign();
            if (s == 0) {
                return frac(0, 0);
            }
            return frac(s, 0);
        }
        return frac(0, 0);
    }
};

// unorderd_map, unorderd_set が使えるようになるらしい (generated by Gemini 3.0 Pro)
namespace std {
    template <>
    struct hash<frac> {
        size_t operator()(const frac &f) const {
            size_t h1 = std::hash<long long>{}(f.num);
            size_t h2 = std::hash<long long>{}(f.den);
            return h1 ^ (h2 + 0x9e3779b9) + (h1 << 6) + (h1 >> 2);
        }
    };
}
