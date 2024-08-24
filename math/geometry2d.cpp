struct point{
    long double x, y;
    point(long double x = 0, long double y = 0) : x(x), y(y) {}

    point operator+ (point p){
        return {x + p.x, y + p.y};
    }

    point operator+= (point p){
        x += p.x;
        y += p.y;
        return *this;
    }

    point operator- (point p){
        return {x - p.x, y - p.y};
    }

    point operator-= (point p){
        x -= p.x;
        y -= p.y;
        return *this;
    }

    point operator* (long double k){
        return {x * k, y * k};
    }

    point operator*= (long double k){
        x *= k;
        y *= k;
        return *this;
    }

    point operator/ (long double k){
        return {x / k, y / k};
    }

    point operator/= (long double k){
        x /= k;
        y /= k;
        return *this;
    }

    // 偏角[rad]
    long double arg_rad(){
        return atan2(y, x);
    }

    // 偏角[°]
    long double arg(){
        return arg_rad() * 180 / M_PI;
    }

    point rotate_rad(long double theta) {
        long double cos_theta = cos(theta);
        long double sin_theta = sin(theta);
        return {x * cos_theta - y * sin_theta, x * sin_theta + y * cos_theta};
    }

    point rotate(long double theta) {
        return rotate_rad(theta * M_PI / 180);
    }
};

long double dot(point a, point b){
    return a.x * b.x + a.y * b.y;
}

long double det(point a, point b){
    return a.x * b.y - a.y * b.x;
}

long double dist(point a, point b){
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}
