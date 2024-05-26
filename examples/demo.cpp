
#include <cstdio>
#include "doubledouble.h"

using namespace doubledouble;

void
print_doubledouble(const char *prefix, const DoubleDouble& x)
{
    printf("%s = (%25.17f, %25.17e)\n", prefix, x.upper, x.lower);
}

void
print_doubledouble_ee(const char *prefix, const DoubleDouble& x)
{
    printf("%s = (%25.17e, %25.17e)\n", prefix, x.upper, x.lower);
}


int main(int argc, char *argv[])
{
    auto x = 3.0_dd;
    auto y = DoubleDouble(1.5, 2.5e-18);

    print_doubledouble("x", x);
    print_doubledouble("y", y);

    auto z = x + y;

    print_doubledouble("   x + y", z);

    z = x - y;
    print_doubledouble("   x - y", z);

    z = -x + y;
    print_doubledouble("  -x + y", z);

    z = x * y;
    print_doubledouble("   x * y", z);

    z = x / y;
    print_doubledouble("   x / y", z);

    z = y / x;
    print_doubledouble("   y / x", z);

    z = DoubleDouble(1.0) / x;
    print_doubledouble("   1 / x", z);

    z = y / 3.0;
    print_doubledouble("   y / 3", z);

    z = 3.0 / y;
    print_doubledouble("   3 / y", z);

    z = y + 5.0;
    print_doubledouble("   y + 5", z);

    z = 5.0 + y;
    print_doubledouble("   5 + y", z);

    z = y - 5.0;
    print_doubledouble("   y - 5", z);

    z = 5.0 - y;
    print_doubledouble("   5 - y", z);

    z = y * 5.0;
    print_doubledouble("   y * 5", z);

    z = 5.0 * y;
    print_doubledouble("   5 * y", z);

    z = (x - y).abs();
    print_doubledouble(" |x - y|", z);

    z = (y - x).abs();
    print_doubledouble(" |y - x|", z);

    z = y.powi(3);
    print_doubledouble("    y**3", z);

    z = y.exp();
    print_doubledouble("  exp(y)", z);

    z = dd_pi.exp();
    print_doubledouble(" exp(pi)", z);

    z = dd_pi.log();
    print_doubledouble(" log(pi)", z);

    z = dd_pi.sqrt();
    print_doubledouble("sqrt(pi)", z);

    bool q = y == x;
    printf("y == x: %s\n", q ? "true" : "false");

    q = y == y;
    printf("y == y: %s\n", q ? "true" : "false");

    z = DoubleDouble(709.0).exp();
    print_doubledouble_ee(" exp(709)", z);

    z = DoubleDouble(710.0).exp();
    print_doubledouble_ee(" exp(710)", z);

    z = DoubleDouble(2.5e-16).expm1();
    print_doubledouble_ee(" expm1(2.5e-16)", z);

    return 0;
}
