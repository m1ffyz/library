/**
 * @file geometry_2d.hpp
 * @brief 平面幾何
 * @author m1ffyz
 * @date 2026-02-07
 * @see https://github.com/m1ffyz/library/blob/main/math/geometry_2d.hpp
 */

using Real = long double;
const Real EPS = 1e-15;
const Real PI = acos(-1.0L);

// 誤差判定用
int sgn(Real a) {
    if (a < -EPS) {
        return -1;
    } else if (a > EPS) {
        return 1;
    } else {
        return 0;
    }
}

struct point {
    Real x, y;

    point() : x(0), y(0) {}
    point(Real x, Real y) : x(x), y(y) {}

    point operator+(const point & p) const {
        return {x + p.x, y + p.y};
    }

    point operator-(const point & p) const {
        return {x - p.x, y - p.y};
    }

    point operator*(Real k) const {
        return {x * k, y * k};
    }

    point operator/(Real k) const {
        return {x / k, y / k};
    }

    point& operator+=(const point &p) {
        x += p.x;
        y += p.y;
        return *this;
    }

    point& operator-=(const point &p) {
        x -= p.x;
        y -= p.y;
        return *this;
    }

    point& operator*=(Real k) {
        x *= k;
        y *= k;
        return *this;
    }

    point& operator/=(Real k) {
        x /= k;
        y /= k;
        return *this;
    }

    bool operator<(const point &p) const {
        if(sgn(x - p.x) != 0) {
            return sgn(x - p.x) < 0;
        } else {
            return sgn(y - p.y) < 0;
        }
    }

    bool operator==(const point &p) const {
        return (sgn(x - p.x) == 0 and sgn(y - p.y) == 0);
    }

    Real norm2() const {
        return x * x + y * y;
    }

    Real abs() const {
        return sqrt(norm2());
    }

    Real arg() const {
        return atan2(y, x);
    }

    static Real dot(const point &a, const point &b) {
        return a.x * b.x + a.y * b.y;
    }

    static Real cross(const point &a, const point &b) {
        return a.x * b.y - a.y * b.x;
    }
};

istream& operator>>(istream& is, point &p) {
    return is >> p.x >> p.y;
}

ostream& operator<<(ostream& os, const point &p) {
    return os << fixed << setprecision(15) << p.x << " " << p.y;
}

Real norm2(point p) {
    return p.norm2();
}

Real abs(point p) {
    return p.abs();
}


// 直線
struct line {
    point p1, p2;
    line() = default;
    line(point p1, point p2) : p1(p1), p2(p2) {}

    point vec() const {
        return p2 - p1;
    }
};

// 線分
using segment = line;

//  1: a -> b -> c が左折 (反時計回り)
// -1: a -> b -> c が右折 (時計回り)
//  2: c -> a -> b が一直線上
// -2: a -> b -> c が一直線上
//  0: a -> c -> b が一直線上
int ccw(point a, point b, point c) {
    b -= a;
    c -= a;
    if (point::cross(b, c) > EPS) {
        return 1;
    } else if (point::cross(b, c) < -EPS) {
        return -1;
    } else if (point::dot(b, c) < -EPS) {
        return 2;
    } else if (b.norm2() < c.norm2()) {
        return -2;
    } else {
        return 0;
    }
}

// 直線が交差するか判定
// 直線が等しい場合も false となるため注意
bool is_intersect_ll (line l1, line l2) {
    return sgn(point::cross(l1.vec(), l2.vec())) != 0;
}

// 線分が交差するか判定
bool is_intersect_ss (segment s1, segment s2) {
    return (ccw(s1.p1, s1.p2, s2.p1) * ccw(s1.p1, s1.p2, s2.p2) <= 0 and ccw(s2.p1, s2.p2, s1.p1) * ccw(s2.p1, s2.p2, s1.p2) <= 0);
}

// 垂線の足
point project(line l, point p) {
    point base = l.vec();
    Real r = point::dot(p - l.p1, base);
    return l.p1 + base * r;
}

// 直線を軸とした線対称な点
point reflect(line l, point p) {
    return p + (project(l, p) - p) * 2.0;
}

Real dist_lp(line l, point p) {
    return abs(point::cross(l.vec(), p - l.p1) / l.vec().abs());
}

Real dist_sp(segment s, point p) {
    if (point::dot(s.vec(), p - s.p1) < -EPS) {
        return (p - s.p1).abs();
    } else if (point::dot(s.p1 - s.p2, p - s.p2) < -EPS) {
        return (p - s.p2).abs();
    } else {
        return dist_lp(s, p);
    }
}

Real dist_ss(segment s1, segment s2) {
    if (is_intersect_ss(s1, s2)) {
        return 0.0;
    } else {
        return min({dist_sp(s1, s2.p1), dist_sp(s1, s2.p2), dist_sp(s2, s1.p1), dist_sp(s2, s1.p2)});
    }
}


// 多角形
using polygon = vector<point>;

// 面積
Real area (const polygon &p) {
    Real res = 0;
    for (int i = 0; i < p.size(); i ++) {
        res += point::cross(p[i], p[(i + 1) % p.size()]);
    }
    return abs(res) / 2.0;
}

// 凸包
polygon convex_hull (vector<point> p) {
    int n = p.size();
    if (n < 3) {
        return p;
    }
    sort(p.begin(), p.end());

    polygon ch(2 * n);
    int k = 0;

    for (int i = 0; i < n; i ++) {
        while (k >= 2 and ccw(ch[k - 2], ch[k - 1], p[i]) != 1) {
            k --;
        }
        ch[k] = p[i];
        k ++;
    }
    for (int i = n - 2, t = k + 1; i >= 0; i --) {
        while (k >= t and ccw(ch[k - 2], ch[k - 1], p[i]) != 1) {
            k --;
        }
        ch[k] = p[i];
        k ++;
    }

    ch.resize(k - 1);
    return ch;
}


// 点が多角形に含まれるか判定
// 2: 内部
// 1: 境界
// 0: 外部
int contains(const polygon& g, point p) {
    bool in = false;
    for (int i = 0; i < g.size(); ++i) {
        point a = g[i] - p, b = g[(i + 1) % g.size()] - p;
        if (abs(point::cross(a, b)) < EPS and point::dot(a, b) < EPS) {
            return 1;
        }
        if (a.y > b.y) {
            swap(a, b);
        }
        if (a.y <= 0 and 0 < b.y and point::cross(a, b) > EPS) {
            in = !in;
        }
    }
    return in ? 2 : 0;
}


// 円
struct circle {
    point p;
    Real r;
    circle(point p, Real r) : p(p), r(r) {}
};

vector<point> intersect_cl(circle c, line l) {
    Real d = dist_lp(l, c.p);
    if (d > c.r + EPS) {
        return {};
    }

    point h = project(l, c.p);
    if(abs(d - c.r) < EPS) {
        return {h};
    }

    Real len = sqrt(max(0.0L, c.r * c.r - d * d));
    point dir = l.vec() / l.vec().abs();
    return {h - dir * len, h + dir * len};
}

vector<point> intersect_cc(circle c1, circle c2) {
    Real d = (c1.p - c2.p).abs();
    if (d > c1.r + c2.r + EPS or d < abs(c1.r - c2.r) - EPS) {
        return {};
    }

    Real cos_val = (c1.r * c1.r + d * d - c2.r * c2.r) / (2 * c1.r * d);
    if (cos_val < -1.0L) {
        cos_val = -1.0L;
    }
    if (cos_val > 1.0L) {
        cos_val = 1.0L;
    }
    Real a = acos(cos_val);

    Real t = (c2.p - c1.p).arg();
    return {c1.p + point(cos(t + a), sin(t + a)) * c1.r, c1.p + point(cos(t - a), sin(t - a)) * c1.r};
}


// 偏角ソート用
int orthant(point p) {
    if (p.y > EPS or (abs(p.y) < EPS and p.x > EPS)) {
        return 0;
    } else {
        return 1;
    }
}

bool arg_sort(point a, point b) {
    int oa = orthant(a), ob = orthant(b);
    if (oa != ob) {
        return oa < ob;
    } else {
        return point::cross(a, b) > 0;
    }
}
