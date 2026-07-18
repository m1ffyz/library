/**
 * @file geometry_2d.hpp
 * @brief 平面幾何
 * @author m1ffyz
 * @date 2026-07-18
 * @see https://github.com/m1ffyz/library/blob/main/math/geometry_2d.hpp
 */

using Real = long double;
const Real EPS = 1e-15L;
const Real PI = acos(-1.0L);

// 絶対誤差による符号判定
int sgn(Real a) {
    if (a < -EPS) {
        return -1;
    } else if (a > EPS) {
        return 1;
    } else {
        return 0;
    }
}

// scale と同じ次元を持つ量に対する相対誤差判定
int sgn_scaled(Real a, Real scale) {
    Real tolerance = EPS * std::abs(scale);
    if (a < -tolerance) {
        return -1;
    } else if (a > tolerance) {
        return 1;
    } else {
        return 0;
    }
}

// 同じ次元を持つ2量を絶対誤差と相対誤差の両方を考慮して比較
int cmp(Real a, Real b) {
    Real scale = std::max({1.0L, std::abs(a), std::abs(b)});
    return sgn_scaled(a - b, scale);
}

bool almost_equal(Real a, Real b) {
    return cmp(a, b) == 0;
}

struct point {
    Real x, y;

    point() : x(0), y(0) {}
    point(Real x, Real y) : x(x), y(y) {}

    point operator+() const {
        return *this;
    }

    point operator-() const {
        return {-x, -y};
    }

    point operator+(const point &p) const {
        return {x + p.x, y + p.y};
    }

    point operator-(const point &p) const {
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

    // sort 用の比較には EPS を使わない。
    bool operator<(const point &p) const {
        if (x != p.x) {
            return x < p.x;
        }
        return y < p.y;
    }

    // 幾何計算で使いやすいよう、operator== は EPS 付き比較とする。
    // sort、set、unique など、同値関係が必要な処理には使わないこと。
    bool operator==(const point &p) const {
        return almost_equal(x, p.x) and almost_equal(y, p.y);
    }

    bool operator!=(const point &p) const {
        return not (*this == p);
    }

    Real norm2() const {
        return x * x + y * y;
    }

    Real abs() const {
        return std::hypot(x, y);
    }

    Real arg() const {
        return std::atan2(y, x);
    }

    static Real dot(const point &a, const point &b) {
        return a.x * b.x + a.y * b.y;
    }

    static Real cross(const point &a, const point &b) {
        return a.x * b.y - a.y * b.x;
    }
};

point operator*(Real k, const point &p) {
    return p * k;
}

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

bool almost_equal(const point &a, const point &b) {
    return a == b;
}

// 完全一致。同値関係が必要な処理で使用する。
bool exact_equal(const point &a, const point &b) {
    return a.x == b.x and a.y == b.y;
}

bool is_zero(const point &p) {
    return p.x == 0.0L and p.y == 0.0L;
}

// 外積と同じ次元のスケールを用いて符号を判定する。
int cross_sgn(const point &a, const point &b) {
    Real scale = a.abs() * b.abs();
    return sgn_scaled(point::cross(a, b), scale);
}

// 内積と同じ次元のスケールを用いて符号を判定する。
int dot_sgn(const point &a, const point &b) {
    Real scale = a.abs() * b.abs();
    return sgn_scaled(point::dot(a, b), scale);
}

// 原点を中心に反時計回りに90度回転
point rotate90(const point &p) {
    return {-p.y, p.x};
}

point midpoint(const point &a, const point &b) {
    return (a + b) / 2.0L;
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

bool is_degenerate(const line &l) {
    return l.p1 == l.p2;
}

void require_nondegenerate(const line &l) {
    if (is_degenerate(l)) {
        throw invalid_argument("geometry: a line requires two distinct points");
    }
}

// 2直線が平行か判定
bool is_parallel_ll(const line &l1, const line &l2) {
    require_nondegenerate(l1);
    require_nondegenerate(l2);
    return cross_sgn(l1.vec(), l2.vec()) == 0;
}

// 2直線が垂直か判定
bool is_orthogonal_ll(const line &l1, const line &l2) {
    require_nondegenerate(l1);
    require_nondegenerate(l2);
    return dot_sgn(l1.vec(), l2.vec()) == 0;
}

// 点 p を通り、直線 l に垂直な直線
line perpendicular_line(const line &l, const point &p) {
    require_nondegenerate(l);
    return {p, p + rotate90(l.vec())};
}

// 線分 s の垂直二等分線
line perpendicular_bisector(const segment &s) {
    require_nondegenerate(s);
    return perpendicular_line(s, midpoint(s.p1, s.p2));
}

//  1: a -> b -> c が左折（反時計回り）
// -1: a -> b -> c が右折（時計回り）
//  2: c -> a -> b が一直線上
// -2: a -> b -> c が一直線上
//  0: a -> c -> b が一直線上
int ccw(point a, point b, point c) {
    b -= a;
    c -= a;

    int turn = cross_sgn(b, c);
    if (turn != 0) {
        return turn;
    }
    if (dot_sgn(b, c) < 0) {
        return 2;
    }
    if (cmp(b.norm2(), c.norm2()) < 0) {
        return -2;
    }
    return 0;
}

// a -> b -> c の向きのみを -1, 0, 1 で返す。
int orientation(const point &a, const point &b, const point &c) {
    return cross_sgn(b - a, c - a);
}

// p が線分 s 上にあるか判定
bool on_segment(const segment &s, const point &p) {
    if (is_degenerate(s)) {
        return almost_equal(s.p1, p);
    }
    if (almost_equal(s.p1, p) or almost_equal(s.p2, p)) {
        return true;
    }

    point a = s.p1 - p;
    point b = s.p2 - p;
    return cross_sgn(a, b) == 0 and dot_sgn(a, b) <= 0;
}

// 2直線がただ1点で交差するか判定
// 平行または同一直線の場合は false
bool is_intersect_ll(const line &l1, const line &l2) {
    return not is_parallel_ll(l1, l2);
}

// 2直線の交点。交点がただ1つでない場合は例外を送出する。
point intersect_ll(const line &l1, const line &l2) {
    require_nondegenerate(l1);
    require_nondegenerate(l2);

    point v1 = l1.vec();
    point v2 = l2.vec();
    if (cross_sgn(v1, v2) == 0) {
        throw domain_error("geometry: lines do not have a unique intersection");
    }

    Real t = point::cross(l2.p1 - l1.p1, v2) / point::cross(v1, v2);
    return l1.p1 + v1 * t;
}

// 線分が交差するか判定。長さ0の線分も点として扱う。
bool is_intersect_ss(const segment &s1, const segment &s2) {
    int o1 = orientation(s1.p1, s1.p2, s2.p1);
    int o2 = orientation(s1.p1, s1.p2, s2.p2);
    int o3 = orientation(s2.p1, s2.p2, s1.p1);
    int o4 = orientation(s2.p1, s2.p2, s1.p2);

    if (o1 * o2 < 0 and o3 * o4 < 0) {
        return true;
    }
    if (o1 == 0 and on_segment(s1, s2.p1)) {
        return true;
    }
    if (o2 == 0 and on_segment(s1, s2.p2)) {
        return true;
    }
    if (o3 == 0 and on_segment(s2, s1.p1)) {
        return true;
    }
    if (o4 == 0 and on_segment(s2, s1.p2)) {
        return true;
    }
    return false;
}

// 垂線の足
point project(const line &l, const point &p) {
    require_nondegenerate(l);
    point base = l.vec();
    Real r = point::dot(p - l.p1, base) / base.norm2();
    return l.p1 + base * r;
}

// 直線を軸とした線対称な点
point reflect(const line &l, const point &p) {
    return p + (project(l, p) - p) * 2.0L;
}

Real dist_lp(const line &l, const point &p) {
    require_nondegenerate(l);
    return std::abs(point::cross(l.vec(), p - l.p1)) / l.vec().abs();
}

Real dist_sp(const segment &s, const point &p) {
    if (is_degenerate(s)) {
        return (p - s.p1).abs();
    }
    if (dot_sgn(s.vec(), p - s.p1) < 0) {
        return (p - s.p1).abs();
    }
    if (dot_sgn(s.p1 - s.p2, p - s.p2) < 0) {
        return (p - s.p2).abs();
    }
    return dist_lp(s, p);
}

Real dist_ss(const segment &s1, const segment &s2) {
    if (is_intersect_ss(s1, s2)) {
        return 0.0L;
    }
    return min({dist_sp(s1, s2.p1), dist_sp(s1, s2.p2),
                dist_sp(s2, s1.p1), dist_sp(s2, s1.p2)});
}


// 多角形
using polygon = vector<point>;

// 面積
Real area(const polygon &p) {
    Real res = 0.0L;
    for (int i = 0; i < static_cast<int>(p.size()); i++) {
        res += point::cross(p[i], p[(i + 1) % p.size()]);
    }
    return std::abs(res) / 2.0L;
}

// 凸包。一直線上の中間点は除く。
polygon convex_hull(vector<point> p) {
    sort(p.begin(), p.end());
    p.erase(unique(p.begin(), p.end(), exact_equal), p.end());

    int n = static_cast<int>(p.size());
    if (n <= 2) {
        return p;
    }

    polygon ch(2 * n);
    int k = 0;

    for (int i = 0; i < n; i++) {
        while (k >= 2 and orientation(ch[k - 2], ch[k - 1], p[i]) != 1) {
            k--;
        }
        ch[k++] = p[i];
    }

    for (int i = n - 2, t = k + 1; i >= 0; i--) {
        while (k >= t and orientation(ch[k - 2], ch[k - 1], p[i]) != 1) {
            k--;
        }
        ch[k++] = p[i];
    }

    ch.resize(k - 1);
    return ch;
}

// 点が多角形に含まれるか判定
// 2: 内部
// 1: 境界
// 0: 外部
int contains(const polygon &g, const point &p) {
    int n = static_cast<int>(g.size());
    if (n == 0) {
        return 0;
    }
    if (n == 1) {
        return almost_equal(g[0], p) ? 1 : 0;
    }
    if (n == 2) {
        return on_segment(segment(g[0], g[1]), p) ? 1 : 0;
    }

    bool in = false;
    for (int i = 0; i < n; i++) {
        point a = g[i] - p;
        point b = g[(i + 1) % n] - p;

        if (on_segment(segment(g[i], g[(i + 1) % n]), p)) {
            return 1;
        }
        if (a.y > b.y) {
            swap(a, b);
        }
        if (a.y <= 0.0L and 0.0L < b.y and cross_sgn(a, b) > 0) {
            in = not in;
        }
    }
    return in ? 2 : 0;
}


// 円
struct circle {
    point p;
    Real r;

    circle(point p, Real r) : p(p), r(r) {
        if (r < 0.0L) {
            throw invalid_argument("geometry: a circle radius must be nonnegative");
        }
    }
};

void require_valid_circle(const circle &c) {
    if (c.r < 0.0L) {
        throw invalid_argument("geometry: a circle radius must be nonnegative");
    }
}

vector<point> intersect_cl(const circle &c, const line &l) {
    require_valid_circle(c);
    require_nondegenerate(l);

    Real d = dist_lp(l, c.p);
    int relation = cmp(d, c.r);
    if (relation > 0) {
        return {};
    }

    point h = project(l, c.p);
    Real len2 = c.r * c.r - d * d;
    Real scale = std::max(c.r * c.r, d * d);
    if (relation == 0 or sgn_scaled(len2, scale) <= 0) {
        return {h};
    }

    Real len = std::sqrt(std::max(0.0L, len2));
    point dir = l.vec() / l.vec().abs();
    return {h - dir * len, h + dir * len};
}

enum class circle_intersection_type {
    none,
    one_point,
    two_points,
    infinite_points
};

struct circle_intersection_result {
    circle_intersection_type type;
    vector<point> points;
};

// 同一円を含む全ケースを区別する詳細版
circle_intersection_result intersect_cc_detailed(const circle &c1,
                                                 const circle &c2) {
    require_valid_circle(c1);
    require_valid_circle(c2);

    bool zero1 = c1.r == 0.0L;
    bool zero2 = c2.r == 0.0L;
    Real d = (c1.p - c2.p).abs();

    if (zero1 and zero2) {
        if (c1.p == c2.p) {
            return {circle_intersection_type::one_point, {c1.p}};
        }
        return {circle_intersection_type::none, {}};
    }
    if (zero1) {
        if (cmp(d, c2.r) == 0) {
            return {circle_intersection_type::one_point, {c1.p}};
        }
        return {circle_intersection_type::none, {}};
    }
    if (zero2) {
        if (cmp(d, c1.r) == 0) {
            return {circle_intersection_type::one_point, {c2.p}};
        }
        return {circle_intersection_type::none, {}};
    }

    if (d == 0.0L) {
        if (c1.r == c2.r) {
            return {circle_intersection_type::infinite_points, {}};
        }
        return {circle_intersection_type::none, {}};
    }

    Real outer = c1.r + c2.r;
    Real inner = std::abs(c1.r - c2.r);
    int outer_cmp = cmp(d, outer);
    int inner_cmp = cmp(d, inner);
    if (outer_cmp > 0 or inner_cmp < 0) {
        return {circle_intersection_type::none, {}};
    }

    point dir = (c2.p - c1.p) / d;
    Real x = (d * d + (c1.r - c2.r) * (c1.r + c2.r)) / (2.0L * d);
    point base = c1.p + dir * x;

    if (outer_cmp == 0 or inner_cmp == 0) {
        return {circle_intersection_type::one_point, {base}};
    }

    Real height2 = c1.r * c1.r - x * x;
    Real scale = std::max(c1.r * c1.r, x * x);
    if (sgn_scaled(height2, scale) <= 0) {
        return {circle_intersection_type::one_point, {base}};
    }

    Real height = std::sqrt(std::max(0.0L, height2));
    point normal(-dir.y, dir.x);
    return {circle_intersection_type::two_points,
            {base + normal * height, base - normal * height}};
}

// 既存API用。同一円では有限個の vector で表せないため例外を送出する。
vector<point> intersect_cc(const circle &c1, const circle &c2) {
    circle_intersection_result result = intersect_cc_detailed(c1, c2);
    if (result.type == circle_intersection_type::infinite_points) {
        throw domain_error("geometry: coincident circles have infinitely many intersections");
    }
    return result.points;
}


// 偏角ソート用。原点を最初に置き、偏角 [0, 2PI) の順に並べる。
int orthant(const point &p) {
    if (p.y > 0.0L or (p.y == 0.0L and p.x > 0.0L)) {
        return 0;
    }
    return 1;
}

bool arg_sort(const point &a, const point &b) {
    bool zero_a = is_zero(a);
    bool zero_b = is_zero(b);
    if (zero_a != zero_b) {
        return zero_a;
    }
    if (zero_a) {
        return false;
    }

    int oa = orthant(a);
    int ob = orthant(b);
    if (oa != ob) {
        return oa < ob;
    }

    Real cr = point::cross(a, b);
    if (cr != 0.0L) {
        return cr > 0.0L;
    }

    Real na = a.norm2();
    Real nb = b.norm2();
    if (na != nb) {
        return na < nb;
    }
    if (a.x != b.x) {
        return a.x < b.x;
    }
    return a.y < b.y;
}
