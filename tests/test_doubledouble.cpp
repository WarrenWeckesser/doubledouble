
#include <cstdio>
#include <vector>
#include <cmath>
#include "checkit.h"
#include "doubledouble.h"

using namespace doubledouble;

//
// Some of the equality tests of the lower parts of the results might be
// too optimistic.  They might fail on different combinations of platforms,
// compilers, and compiler options.
//

void test_constructor(CheckIt& test)
{
    DoubleDouble d;
    check_equal_fp(test, d.upper, 0.0, "DoubleDouble() (upper)");
    check_equal_fp(test, d.lower, 0.0, "DoubleDouble() (lower)");

    auto three = DoubleDouble(3.0);
    check_equal_fp(test, three.upper, 3.0, "DoubleDouble(3.0) (upper)");
    check_equal_fp(test, three.lower, 0.0, "DoubleDouble(3.0) (lower)");

    auto x = DoubleDouble(10.0, 3e-18);
    check_equal_fp(test, x.upper, 10.0, "DoubleDouble(10.0, 3e-18) (upper)");
    check_equal_fp(test, x.lower, 3e-18, "DoubleDouble(10.0, 3e-18)  (lower)");

    // Check canonicalization.
    auto z = DoubleDouble(-1.0, 1.25);
    check_equal_fp(test, z.upper, 0.25, "DoubleDouble(-1.0, 1.25) (upper)");
    check_equal_fp(test, z.lower, 0.0, "DoubleDouble(-1.0, 1.25) (lower)");

    auto y = DoubleDouble(INFINITY, 0.0);
    check_equal_fp(test, y.upper, (double) INFINITY, "DoubleDouble(INF, 0) (upper)");
    check_equal_fp(test, y.lower, 0.0, "DoubleDouble(INF, 0) (lower)");

    auto bad = DoubleDouble((double) INFINITY, (double) -INFINITY);
    check_true(test, isnan(bad.upper), "DoubleDouble(INF, -INF) (upper)");
    check_true(test, isnan(bad.lower), "DoubleDouble(INF, -INF) (lower)");
}

void test_add(CheckIt& test)
{
    auto z = DoubleDouble(1.0, 1e-18) + DoubleDouble(1.5, 5e-19);
    check_equal_fp(test, z.upper, 2.5, "(1, 1e-18) + (1.5, 5e-19) (upper)");
    check_close_fp(test, z.lower, 1.5e-18, 5e-16, "(1, 1e-18) + (1.5, 5e-19) (lower)");
}

void test_inplace_add(CheckIt& test)
{
    auto z = DoubleDouble(2.5, 1.5e-18);
    z += 3.5;
    check_equal_fp(test, z.upper, 6.0, "Check z += 3.5 (upper)");
    check_close_fp(test, z.lower, 1.5e-18, 5e-16, "Check z += 3.5 (lower)");

    z += DoubleDouble(3.0, 1e-18);
    check_equal_fp(test, z.upper, 9.0, "Check z += (3.5, 1e-18) (upper)");
    check_close_fp(test, z.lower, 2.5e-18, 5e-16, "Check z += (3.5, 1e-18) (lower)");
}

void test_subtract(CheckIt& test)
{
    DoubleDouble y;
    auto three = DoubleDouble(3.0);
    auto x = DoubleDouble(10.0, 3e-18);

    y = x - three;
    check_equal_fp(test, y.upper, 7.0, "x - 3 (upper)");
    check_equal_fp(test, y.lower, 3e-18, "x - 3 (lower)");

    y = three - x;
    check_equal_fp(test, y.upper, -7.0, "3 - x (upper)");
    check_equal_fp(test, y.lower, -3e-18, "3 - x (lower)");

    y = x - 10;
    check_equal_fp(test, y.upper, 3e-18, "x - 10 (upper)");
    check_equal_fp(test, y.lower, 0.0, "x - 10 (lower)");
}

void test_inplace_subtract(CheckIt& test)
{
    DoubleDouble y;

    y = DoubleDouble(2.5);
    y -= 4.5e-18;
    check_equal_fp(test, y.upper, 2.5, "test_inplace_subtract (upper)");
    check_equal_fp(test, y.lower, -4.5e-18, "test_inplace_subtract (lower)");

    y -= 1.5;
    check_equal_fp(test, y.upper, 1.0, "test_inplace_subtract (upper)");
    check_equal_fp(test, y.lower, -4.5e-18, "test_inplace_subtract (lower)");

    y -= 1.0;
    check_equal_fp(test, y.upper, -4.5e-18, "test_inplace_subtract (upper)");
    check_equal_fp(test, y.lower, 0.0, "test_inplace_subtract (lower)");

    /*
    // See https://bugs.llvm.org/show_bug.cgi?id=43124
    y = DoubleDouble(17.5, -2.25e-21);
    y -= y;
    check_equal_fp(test, y.upper, 0.0, "test_inplace_subtract (upper)");
    check_equal_fp(test, y.lower, 0.0, "test_inplace_subtract (lower)");
    */
}

void test_multiply(CheckIt& test)
{
    DoubleDouble y;
    auto x = DoubleDouble(10.0, 3e-18);

    y = x*x;
    check_equal_fp(test, y.upper, 100.0, "Check x*x (upper)");
    check_equal_fp(test, y.lower, 6e-17, "Check x*x (lower)");
}

void test_inplace_multiply(CheckIt& test)
{
    auto z = DoubleDouble(9.0, 2.5e-18);
    z *= -3.0;
    check_equal_fp(test, z.upper, -27.0, "Check z *= -3.0 (upper)");
    check_equal_fp(test, z.lower, -7.5e-18, "Check z *= -3.0 (lower)");

    z *= DoubleDouble(-2.0, 2e-18);
    check_equal_fp(test, z.upper, 54.0, "Check z *= (-2.0, 2e-18) (upper)");
    check_close_fp(test, z.lower, -27*2e-18 + -7.5e-18*-2.0, 5e-16, "Check z *= (-2.0, 2e-18) (lower)");
}

void test_divide(CheckIt &test)
{
    auto three = DoubleDouble(3.0);
    auto y = 1.0 / three;
    check_equal_fp(test, y.upper, 0.3333333333333333, "1/3 (upper)");
    check_equal_fp(test, y.lower, 1.850371707708594e-17, "1/3 (lower)");
}

void test_inplace_divide(CheckIt& test)
{
    DoubleDouble z;

    z = DoubleDouble(5.0, 4e-21);
    z /= 5.0;
    check_equal_fp(test, z.upper, 1.0, "Check z /= 5.0 (upper)");
    check_close_fp(test, z.lower, 8e-22, 5e-16, "Check z /= 5.0 (lower)");

    z = DoubleDouble(5.0, 4e-21);
    z /= DoubleDouble(5.0, 1e-20);
    check_equal_fp(test, z.upper, 1.0, "Check z /= (5.0, 1e-20) (upper)");
    check_close_fp(test, z.lower, -1.2e-21, 5e-16, "Check z /= (5.0, 1e-20) (lower)");
}

void test_expressions(CheckIt& test)
{
    DoubleDouble y;
    DoubleDouble one{1.0};
    DoubleDouble p6{3.14159, 0.0};

    y = one - DoubleDouble(2.0)/3;
    check_equal_fp(test, y.upper, 0.3333333333333333, "Check 1 - 2/3 (upper)");
    check_close_fp(test, y.lower, 1.850371707708594e-17, 5e-16, "Check 1 - 2/3 (lower)");

    y = (dd_pi - p6)*((dd_pi + p6)/2);
    check_equal_fp(test, y.upper, 8.336494679678182e-06, "Check (pi - p6)*((pi + p6)/2) (upper)");
    check_close_fp(test, y.lower, -1.4573851200099609e-22, 5e-16, "Check (pi - p6)*((pi + p6)/2) (lower)");
}

void test_abs(CheckIt& test)
{
    DoubleDouble a;
    auto x = DoubleDouble(-7.0, -3e-18);

    a = x.abs();
    check_equal_fp(test, a.upper, 7.0, "abs(3 - x) (upper)");
    check_equal_fp(test, a.lower, 3e-18, "abs(3 - x) (lower)");
}

void test_powi(CheckIt& test)
{
    auto x = DoubleDouble(10.0, 3e-18);
    auto y = x.powi(4);
    check_equal_fp(test, y.upper, 10000.0, "Check x**4 (upper)");
    check_close_fp(test, y.lower, 1.2e-14, 5e-16, "Check x**4 (lower)");
}

void test_sqrt(CheckIt& test)
{
    DoubleDouble y;
    DoubleDouble x{0.0026, 2e-20};

    y = x.sqrt();
    check_equal_fp(test, y.upper, 0.05099019513592785, "sqrt((1.0026, 5e-17)) (upper)");
    check_close_fp(test, y.lower, -8.798476828364278e-20, 5e-16, "sqrt((1.0026, 5e-17)) (lower)");

    y = DoubleDouble(101.0, 7.046e-15).sqrt();
    check_equal_fp(test, y.upper, 10.04987562112089, "sqrt((101, 7.046e-15)) (upper)");
    check_close_fp(test, y.lower, 6.749565974288905e-16, 5e-16, "sqrt((101, 7.046e-15)) (lower)");

    y = DoubleDouble(0.0, 0.0).sqrt();
    check_equal_fp(test, y.upper, 0.0, "sqrt((0, 0)) (upper)");
    check_equal_fp(test, y.lower, 0.0, "sqrt((0, 0)) (lower)");
}

void test_log(CheckIt& test)
{
    DoubleDouble y;
    auto x = DoubleDouble(10.0, 3e-18);

    y = x.log();
    check_equal_fp(test, y.upper, 2.302585092994046, "Check log(x) (upper)");
    check_equal_fp(test, y.lower, -2.1677562233822494e-16, "Check log(x) (lower)");

    y = dd_e.log();
    check_equal_fp(test, y.upper, 1.0, "log(e) (upper)");
    check_equal_fp(test, y.lower, 0.0, "log(e) (lower)");

    // The actual relative error for log(1.000000099005) is approx. 8.7e-10.
    // Can we do better?
    y = DoubleDouble(1.000000099005, 0.0).log();
    check_equal_fp(test, y.upper, 9.900499507506536e-08, "log(1.000000099005) (upper)");
    check_close_fp(test, y.lower, 4.563816054961034e-24, 2e-9, "log(1.000000099005) (lower)");
}

/* log1p not implemented yet...
void test_log1p(CheckIt& test)
{
    DoubleDouble x{0.0026, 2e-20};

    auto y = x.log1p();
    check_equal_fp(test, y.upper, 0.002596625847265978, "log1p((1.0026, 5e-17)) (upper)");
    check_close_fp(test, y.lower, 1.1029570690454358e-19, 5e-16, "log1p((1.0026, 5e-17)) (lower)");
}
*/

void test_exp(CheckIt& test)
{
    DoubleDouble y;
    auto x = DoubleDouble(10.0, 3e-18);
    auto t = DoubleDouble(3.7500950075008e-15, 0.0);

    y = DoubleDouble(0.0).exp();
    check_equal_fp(test, y.upper, 1.0, "exp(0) (upper)");
    check_equal_fp(test, y.lower, 0.0, "exp(0) (lower)");

    y = DoubleDouble(1.0).exp();
    check_equal_fp(test, y.upper, 2.718281828459045, "exp(0) (upper)");
    check_equal_fp(test, y.lower, 1.4456468917292502e-16, "exp(0) (lower)");

    y = x.exp();
    check_equal_fp(test, y.upper, 22026.465794806718, "exp(x) (upper)");
    check_equal_fp(test, y.lower, -1.3119340726673171e-12, "exp(x) (lower)");

    y = dd_pi.exp();
    check_equal_fp(test, y.upper, 23.14069263277927, "exp(pi) (upper)");
    check_equal_fp(test, y.lower, -1.3488747091995788e-15, "exp(pi) (lower)");

    y = (x - 10.0).exp();
    check_equal_fp(test, y.upper, 1.0, "exp(x - 10) - 1 (upper)");
    check_equal_fp(test, y.lower, 3e-18, "exp(x - 10) - 1 (lower)");

    y = t.exp();
    check_equal_fp(test, y.upper, 1.0000000000000038, "exp(t) (upper)");
    check_close_fp(test, y.lower, -2.4663276224724858e-17, 5e-16, "exp(t) (lower)");

    // The actual relative error for exp(709).lower is approx. 2e-14.
    // Can we do better?
    y = DoubleDouble(709.0).exp();
    check_equal_fp(test, y.upper, 8.218407461554972e+307, "exp(709) upper");
    check_close_fp(test, y.lower, -1.955965507696277e+291, 1e-13, "exp(709) lower");

    y = DoubleDouble(710.0).exp();
    check_true(test, std::isinf(y.upper), "isinf(exp(710).upper)");
    check_equal_fp(test, y.lower, 0.0, "exp(710).lower == 0");
}

void test_expm1(CheckIt& test)
{
    DoubleDouble y;
    auto t = DoubleDouble(3.7500950075008e-15, 0.0);

    y = DoubleDouble(1.0).expm1();
    check_equal_fp(test, y.upper, 1.7182818284590453, "expm1(1) upper");
    check_equal_fp(test, y.lower, -7.747991575210629e-17, "expm1(1) lower");

    y = DoubleDouble(0.46875).expm1();
    check_equal_fp(test, y.upper, 0.5979954499506333, "expm1(0.46875) upper");
    check_equal_fp(test, y.lower, 1.6864630310268093e-17, "expm1(0.46875) lower");

    y = t.expm1();
    check_equal_fp(test, y.upper, 3.7500950075008074e-15, "expm1(t) upper");
    check_equal_fp(test, y.lower, -6.814186434788356e-32, "expm1(t) lower");

    y = DoubleDouble(-0.000244140625).expm1();
    check_equal_fp(test, y.upper, -0.00024411082510278348, "expm1(-2**-12) upper");
    check_equal_fp(test, y.lower, -7.227720384831839e-21, "expm1(-2**-12) lower");

    y = DoubleDouble(-0.46875).expm1();
    check_equal_fp(test, y.upper, -0.37421599039540887, "expm1(-0.46875) upper");
    check_equal_fp(test, y.lower, -7.658883125910196e-18, "expm1(-0.46875) lower");
}

void test_hypot(CheckIt& test)
{
    DoubleDouble x, y, h;

    x = DoubleDouble(3.0, 0.0);
    y = DoubleDouble(4.0, 0.0);
    h = hypot(x, y);
    check_equal_fp(test, h.upper, 5.0, "hypot((3, 0), (4, 0)) (upper)");
    check_equal_fp(test, h.lower, 0.0, "hypot((3, 0), (4, 0)) (lower)");

    x = DoubleDouble(0.0, 0.0);
    y = DoubleDouble(0.0, 0.0);
    h = hypot(x, y);
    check_equal_fp(test, h.upper, 0.0, "hypot((0, 0), (0, 0)) (upper)");
    check_equal_fp(test, h.lower, 0.0, "hypot((0, 0), (0, 0)) (lower)");

    //
    // Reference values for the overflow and underflow tests of hypot
    // were computed with mpmath in Python:
    //
    //    from mpath import mp
    //    mp.dps = 500
    //    x = mp.mpf(5e250) + mp.mpf(5e225)
    //    y = mp.mpf(2e251) + mp.mpf(6e220)
    //    h = mp.sqrt(x**2 + y**2)
    //    h_upper = float(h)
    //    h_lower = float(h - h_upper)
    //
    x = DoubleDouble(5e250, 5e225);
    y = DoubleDouble(2e251, 6e220);
    h = hypot(x, y);
    check_equal_fp(test, h.upper, 2.0615528128088305e+251, "hypot(BIG1, BIG2) (upper)");
    check_close_fp(test, h.lower, -9.087579332326068e+234, 5e-16, "hypot(BIG1, BIG2) (lower)");

    x = DoubleDouble(3e-165, 5e-185);
    y = DoubleDouble(7.5e-165, -8e-184);
    h = hypot(x, y);
    check_equal_fp(test, h.upper, 8.077747210701756e-165, "hypot(TINY1, TINY2) (upper)");
    check_close_fp(test, h.lower, -5.79126190591365e-182, 1e-15, "hypot(TINY1, TINY2) (lower)");

    x = DoubleDouble(-17.25, -2.25e-19);
    y = DoubleDouble(0.0, 0.0);
    h = hypot(x, y);
    check_equal_fp(test, h.upper, -x.upper, "hypot(x, (0, 0)) (upper)");
    check_equal_fp(test, h.lower, -x.lower, "hypot(x, (0, 0)) (lower)");
}

void test_dsum(CheckIt& test)
{
    double data1[]{1.0, 3.0, 99.0};
    double s1 = dsum(3, data1);
    check_equal_fp(test, s1, 103.0, "Test dsum([1.0, 2.0, 99.0])");

    double data2[]{1.0, 2.0, 2e-17, -2.0, 10.0, -1.0, -10.0};
    double s2 = dsum(7, data2);
    check_equal_fp(test, s2, 2e-17, "Test of dsum()");

    std::vector<double> data3{1.0, 2.0, 2e-17, -2.0, 10.0, -1.0, -10.0};
    double s3 = dsum(data3);
    check_equal_fp(test, s3, 2e-17, "Test of dsum(vector)");

    std::array<double, 7> data4{1.0, 2.0, 2e-17, -2.0, 10.0, -1.0, -10.0};
    double s4 = dsum(data4);
    check_equal_fp(test, s4, 2e-17, "Test of dsum(vector)");
}


int main(int argc, char *argv[])
{
    auto test = CheckIt(std::cerr);

    test_constructor(test);
    test_add(test);
    test_inplace_add(test);
    test_subtract(test);
    test_inplace_subtract(test);
    test_multiply(test);
    test_inplace_multiply(test);
    test_divide(test);
    test_inplace_divide(test);
    test_expressions(test);
    test_abs(test);
    test_powi(test);
    test_sqrt(test);
    test_log(test);
    // test_log1p(test);
    test_exp(test);
    test_expm1(test);
    test_hypot(test);
    test_dsum(test);

    return test.print_summary("Summary: ");
}
