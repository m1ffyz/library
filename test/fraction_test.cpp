#include <bits/stdc++.h>
using namespace std;

#include "../math/fraction.hpp"

void assert_positive_denominator(const frac &value) {
    assert(value.den > 0);
}

int main() {
    const frac a(1, -2);
    const frac b(-1, -2);

    assert(a == frac(-1, 2));
    assert(b == frac(1, 2));
    assert_positive_denominator(a);
    assert_positive_denominator(b);

    const frac x(1, -2);
    const frac y(1, -3);

    const frac sum = x + y;
    const frac difference = x - y;
    const frac product = x * y;
    const frac quotient = x / y;

    assert(sum == frac(-5, 6));
    assert(difference == frac(-1, 6));
    assert(product == frac(1, 6));
    assert(quotient == frac(3, 2));

    assert_positive_denominator(sum);
    assert_positive_denominator(difference);
    assert_positive_denominator(product);
    assert_positive_denominator(quotient);
}
