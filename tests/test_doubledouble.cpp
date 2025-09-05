
#include <sstream>
#include <cstdio>
#include <vector>
#include <cmath>
#include "checkit.h"
#include "doubledouble.h"

using namespace doubledouble;


#define assert_isnan(test, d) \
    assert_true(test, std::isnan(d.upper), "upper is NAN"); \
    assert_true(test, std::isnan(d.lower), "lower is NAN"); \


//
// Some of the equality tests of the lower parts of the results might be
// too optimistic.  They might fail on different combinations of platforms,
// compilers, and compiler options.
//

void test_constructor(CheckIt& test)
{
    DoubleDouble d;
    assert_equal_fp(test, d.upper, 0.0, "DoubleDouble() (upper)");
    assert_equal_fp(test, d.lower, 0.0, "DoubleDouble() (lower)");

    auto three = DoubleDouble(3.0);
    assert_equal_fp(test, three.upper, 3.0, "DoubleDouble(3.0) (upper)");
    assert_equal_fp(test, three.lower, 0.0, "DoubleDouble(3.0) (lower)");

    auto x = DoubleDouble(10.0, 3e-18);
    assert_equal_fp(test, x.upper, 10.0, "DoubleDouble(10.0, 3e-18) (upper)");
    assert_equal_fp(test, x.lower, 3e-18, "DoubleDouble(10.0, 3e-18)  (lower)");

    // Check canonicalization.
    auto z = DoubleDouble(-1.0, 1.25);
    assert_equal_fp(test, z.upper, 0.25, "DoubleDouble(-1.0, 1.25) (upper)");
    assert_equal_fp(test, z.lower, 0.0, "DoubleDouble(-1.0, 1.25) (lower)");

    auto y = DoubleDouble(INFINITY, 0.0);
    assert_equal_fp(test, y.upper, (double) INFINITY, "DoubleDouble(INF, 0) (upper)");
    assert_equal_fp(test, y.lower, 0.0, "DoubleDouble(INF, 0) (lower)");

    auto bad = DoubleDouble((double) INFINITY, (double) -INFINITY);
    assert_true(test, std::isnan(bad.upper), "DoubleDouble(INF, -INF) (upper)");
    assert_true(test, std::isnan(bad.lower), "DoubleDouble(INF, -INF) (lower)");

    auto dd1 = DoubleDouble(NAN, 0.0);
    assert_isnan(test, dd1);

    auto dd2 = DoubleDouble(1.0, NAN);
    assert_isnan(test, dd2);

    auto dd3 = DoubleDouble(NAN);
    assert_isnan(test, dd3);
}

void test_add(CheckIt& test)
{
    auto z = DoubleDouble(1.0, 1e-18) + DoubleDouble(1.5, 5e-19);
    assert_equal_fp(test, z.upper, 2.5, "(1, 1e-18) + (1.5, 5e-19) (upper)");
    assert_close_fp(test, z.lower, 1.5e-18, 5e-16, "(1, 1e-18) + (1.5, 5e-19) (lower)");

    auto s1 = DoubleDouble(4.0, 4e-17) + NAN;
    assert_isnan(test, s1);

    auto s2 = DoubleDouble(4.0, 4e-17) + DoubleDouble(NAN);
    assert_isnan(test, s2);
}

void test_inplace_add(CheckIt& test)
{
    auto z = DoubleDouble(2.5, 1.5e-18);
    z += 3.5;
    assert_equal_fp(test, z.upper, 6.0, "Check z += 3.5 (upper)");
    assert_close_fp(test, z.lower, 1.5e-18, 5e-16, "Check z += 3.5 (lower)");

    z += DoubleDouble(3.0, 1e-18);
    assert_equal_fp(test, z.upper, 9.0, "Check z += (3.5, 1e-18) (upper)");
    assert_close_fp(test, z.lower, 2.5e-18, 5e-16, "Check z += (3.5, 1e-18) (lower)");

    z += NAN;
    assert_isnan(test, z);

    z = DoubleDouble(100.0, 1.3e-15);
    z += DoubleDouble(54.0, NAN);
    assert_isnan(test, z);
}

void test_subtract(CheckIt& test)
{
    DoubleDouble y;
    auto three = DoubleDouble(3.0);
    auto x = DoubleDouble(10.0, 3e-18);

    y = x - three;
    assert_equal_fp(test, y.upper, 7.0, "x - 3 (upper)");
    assert_equal_fp(test, y.lower, 3e-18, "x - 3 (lower)");

    y = three - x;
    assert_equal_fp(test, y.upper, -7.0, "3 - x (upper)");
    assert_equal_fp(test, y.lower, -3e-18, "3 - x (lower)");

    y = x - 10;
    assert_equal_fp(test, y.upper, 3e-18, "x - 10 (upper)");
    assert_equal_fp(test, y.lower, 0.0, "x - 10 (lower)");

    y = x - NAN;
    assert_isnan(test, y);

    y = x - DoubleDouble(5.0, NAN);
    assert_isnan(test, y);
}

void test_inplace_subtract(CheckIt& test)
{
    DoubleDouble y;

    y = DoubleDouble(2.5);
    y -= 4.5e-18;
    assert_equal_fp(test, y.upper, 2.5, "test_inplace_subtract (upper)");
    assert_equal_fp(test, y.lower, -4.5e-18, "test_inplace_subtract (lower)");

    y -= 1.5;
    assert_equal_fp(test, y.upper, 1.0, "test_inplace_subtract (upper)");
    assert_equal_fp(test, y.lower, -4.5e-18, "test_inplace_subtract (lower)");

    y -= 1.0;
    assert_equal_fp(test, y.upper, -4.5e-18, "test_inplace_subtract (upper)");
    assert_equal_fp(test, y.lower, 0.0, "test_inplace_subtract (lower)");

    /*
    // See https://bugs.llvm.org/show_bug.cgi?id=43124
    y = DoubleDouble(17.5, -2.25e-21);
    y -= y;
    assert_equal_fp(test, y.upper, 0.0, "test_inplace_subtract (upper)");
    assert_equal_fp(test, y.lower, 0.0, "test_inplace_subtract (lower)");
    */

    y -= NAN;
    assert_isnan(test, y);

    y = DoubleDouble(100.0, 1.3e-15);
    y -= DoubleDouble(54.0, NAN);
    assert_isnan(test, y);
}

void test_multiply(CheckIt& test)
{
    DoubleDouble y;
    auto x = DoubleDouble(10.0, 3e-18);

    y = x*x;
    assert_equal_fp(test, y.upper, 100.0, "Check x*x (upper)");
    assert_equal_fp(test, y.lower, 6e-17, "Check x*x (lower)");

    y = x*NAN;
    assert_isnan(test, y);

    y = x*DoubleDouble(NAN);
    assert_isnan(test, y);
}

void test_inplace_multiply(CheckIt& test)
{
    auto z = DoubleDouble(9.0, 2.5e-18);
    z *= -3.0;
    assert_equal_fp(test, z.upper, -27.0, "Check z *= -3.0 (upper)");
    assert_equal_fp(test, z.lower, -7.5e-18, "Check z *= -3.0 (lower)");

    z *= DoubleDouble(-2.0, 2e-18);
    assert_equal_fp(test, z.upper, 54.0, "Check z *= (-2.0, 2e-18) (upper)");
    assert_close_fp(test, z.lower, -27*2e-18 + -7.5e-18*-2.0, 5e-16, "Check z *= (-2.0, 2e-18) (lower)");

    z *= NAN;
    assert_isnan(test, z);

    z = DoubleDouble(3.0);
    z *= DoubleDouble(NAN);
    assert_isnan(test, z);
}

void test_divide(CheckIt &test)
{
    auto three = DoubleDouble(3.0);
    auto y = 1.0 / three;
    assert_equal_fp(test, y.upper, 0.3333333333333333, "1/3 (upper)");
    assert_equal_fp(test, y.lower, 1.850371707708594e-17, "1/3 (lower)");

    y = three/NAN;
    assert_isnan(test, y);

    y = three/DoubleDouble(NAN);
    assert_isnan(test, y);
}

void test_inplace_divide(CheckIt& test)
{
    DoubleDouble z;

    z = DoubleDouble(5.0, 4e-21);
    z /= 5.0;
    assert_equal_fp(test, z.upper, 1.0, "Check z /= 5.0 (upper)");
    assert_close_fp(test, z.lower, 8e-22, 5e-16, "Check z /= 5.0 (lower)");

    z = DoubleDouble(5.0, 4e-21);
    z /= DoubleDouble(5.0, 1e-20);
    assert_equal_fp(test, z.upper, 1.0, "Check z /= (5.0, 1e-20) (upper)");
    assert_close_fp(test, z.lower, -1.2e-21, 5e-16, "Check z /= (5.0, 1e-20) (lower)");

    z /= NAN;
    assert_isnan(test, z);

    z = DoubleDouble(3.0);
    z /= DoubleDouble(NAN);
    assert_isnan(test, z);
}

void test_expressions(CheckIt& test)
{
    DoubleDouble y;
    DoubleDouble one{1.0};
    DoubleDouble p6{3.14159, 0.0};

    y = one - DoubleDouble(2.0)/3;
    assert_equal_fp(test, y.upper, 0.3333333333333333, "Check 1 - 2/3 (upper)");
    assert_close_fp(test, y.lower, 1.850371707708594e-17, 5e-16, "Check 1 - 2/3 (lower)");

    y = (dd_pi - p6)*((dd_pi + p6)/2);
    assert_equal_fp(test, y.upper, 8.336494679678182e-06, "Check (pi - p6)*((pi + p6)/2) (upper)");
    assert_close_fp(test, y.lower, -1.4573851200099609e-22, 5e-16, "Check (pi - p6)*((pi + p6)/2) (lower)");
}

void test_comparisons(CheckIt& test)
{
    DoubleDouble w, x, y, z;

    w = DoubleDouble(-3.0);
    x = DoubleDouble(13.5, 1e-20);
    y = DoubleDouble(13.5, -3e-18);
    z = DoubleDouble(99.0, -3e-18);

    assert_true(test, x == x, "x == x");
    assert_true(test, !(x == 2.0), "!(x == 2.0)");
    assert_true(test, !(x == y), "!(x == y)");
    assert_true(test, !(x == z), "!(x == z)");
    assert_true(test, w == -3.0, "w == -3.0");
    assert_true(test, -3.0 == w, "-3.0 == w");

    assert_true(test, x != y, "x != y");
    assert_true(test, x != z, "x != z");
    assert_true(test, x != 1.0, "x != 1.0");
    assert_true(test, 1.0 != x, "1.0 != x");

    assert_true(test, y < x, "y < x");
    assert_true(test, y < 14.0, "y < 14.0");
    assert_true(test, y < 13.5, "y < 13.5");
    assert_true(test, 13.5 < x, "13.5 < x");

    assert_true(test, y <= x, "y <= x");
    assert_true(test, y <= y, "y <= x");
    assert_true(test, y <= 14.0, "y <= 14.0");
    assert_true(test, y <= 13.5, "y <= 13.5");
    assert_true(test, 13.5 <= x, "13.5 <= x");
    assert_true(test, -3.0 <= w, "-3.0 <= w");

    assert_true(test, x > y, "x > y");
    assert_true(test, x > 12.0, "x > 12.0");
    assert_true(test, x > 13.5, "x > 13.5");
    assert_true(test, 100.0 > z, "100.0 > z");

    assert_true(test, x >= y, "x >= y");
    assert_true(test, x >= x, "x >= x");
    assert_true(test, x >= 12.0, "x >= 12.0");
    assert_true(test, x >= 13.5, "x >= 13.5");
    assert_true(test, 100.0 >= z, "100.0 >= z");
    assert_true(test, -3.0 >= w, "-3.0 >= w");

    assert_true(test, DoubleDouble(NAN) != DoubleDouble(NAN), "check NAN != NAN is true");
    assert_false(test, DoubleDouble(NAN) == DoubleDouble(NAN), "NAN == NAN");
    assert_false(test, DoubleDouble(NAN) < DoubleDouble(NAN), "NAN < NAN");
    assert_false(test, DoubleDouble(NAN) > DoubleDouble(NAN), "NAN > NAN");
    assert_false(test, DoubleDouble(NAN) <= DoubleDouble(NAN), "NAN <= NAN");
    assert_false(test, DoubleDouble(NAN) >= DoubleDouble(NAN), "NAN >= NAN");
}


void test_abs(CheckIt& test)
{
    DoubleDouble a;
    auto x = DoubleDouble(-7.0, -3e-18);

    a = x.abs();
    assert_equal_fp(test, a.upper, 7.0, "abs(3 - x) (upper)");
    assert_equal_fp(test, a.lower, 3e-18, "abs(3 - x) (lower)");

    a = DoubleDouble(NAN).abs();
    assert_isnan(test, a);
}

void test_powi(CheckIt& test)
{
    auto x = DoubleDouble(10.0, 3e-18);
    auto y = x.powi(4);
    assert_equal_fp(test, y.upper, 10000.0, "Check x**4 (upper)");
    assert_close_fp(test, y.lower, 1.2e-14, 5e-16, "Check x**4 (lower)");

    y = DoubleDouble(NAN).powi(0);
    assert_true(test, y == 1.0, "NAN**0 is 1");
    y = DoubleDouble(NAN).powi(1);
    assert_isnan(test, y);
    y = DoubleDouble(NAN).powi(2);
    assert_isnan(test, y);
    y = DoubleDouble(NAN).powi(3);
    assert_isnan(test, y);
}

void test_sqrt(CheckIt& test)
{
    DoubleDouble y;
    DoubleDouble x{0.0026, 2e-20};

    y = x.sqrt();
    assert_equal_fp(test, y.upper, 0.05099019513592785, "sqrt((1.0026, 5e-17)) (upper)");
    assert_close_fp(test, y.lower, -8.798476828364278e-20, 5e-16, "sqrt((1.0026, 5e-17)) (lower)");

    y = DoubleDouble(101.0, 7.046e-15).sqrt();
    assert_equal_fp(test, y.upper, 10.04987562112089, "sqrt((101, 7.046e-15)) (upper)");
    assert_close_fp(test, y.lower, 6.749565974288905e-16, 5e-16, "sqrt((101, 7.046e-15)) (lower)");

    y = DoubleDouble(0.0, 0.0).sqrt();
    assert_equal_fp(test, y.upper, 0.0, "sqrt((0, 0)) (upper)");
    assert_equal_fp(test, y.lower, 0.0, "sqrt((0, 0)) (lower)");

    y = DoubleDouble(NAN).sqrt();
    assert_isnan(test, y);
}

void test_log(CheckIt& test)
{
    DoubleDouble y;
    auto x = DoubleDouble(10.0, 3e-18);

    y = x.log();
    assert_equal_fp(test, y.upper, 2.302585092994046, "Check log(x) (upper)");
    assert_equal_fp(test, y.lower, -2.1677562233822494e-16, "Check log(x) (lower)");

    y = dd_e.log();
    assert_equal_fp(test, y.upper, 1.0, "log(e) (upper)");
    assert_equal_fp(test, y.lower, 0.0, "log(e) (lower)");

    // The actual relative error for log(1.000000099005) is approx. 8.7e-10.
    // Can we do better?
    y = DoubleDouble(1.000000099005, 0.0).log();
    assert_equal_fp(test, y.upper, 9.900499507506536e-08, "log(1.000000099005) (upper)");
    assert_close_fp(test, y.lower, 4.563816054961034e-24, 2e-9, "log(1.000000099005) (lower)");

    y = DoubleDouble(1.1, 0.0).log();
    assert_equal_fp(test, y.upper, 0.09531017980432493, "log(1.1) (upper)");
    assert_close_fp(test, y.lower, 5.927240202146761e-18, 8e-16, "log(1.1) (lower)");

    y = DoubleDouble(1e-21, 3.5e-43).log();
    assert_equal_fp(test, y.upper, -48.35428695287496, "log((1e-21, 3.5e-43)) (upper)");
    assert_close_fp(test, y.lower, -1.7511230665702564e-15, 5e-16, "log((1e-21, 3.5e-43)) (lower)");

    y = DoubleDouble(NAN).log();
    assert_isnan(test, y);
}

struct log1p_case {
    // Acceptable relative error in the lower part of the result.
    double reltol;

    double xhi, xlo, yhi, ylo;
};

void test_log1p(CheckIt& test)
{
    struct log1p_case samples[] = {
        {5e-16, 3.1e-50, 7e-68, 3.1e-50, 7e-68},
        {5e-16, 1.4e-30, 5e-48, 1.4e-30, 4.9999999999990196e-48},

        {2e-15, 7.7e-16, 8.6e-34, 7.699999999999997e-16, 2.328394578795871e-34},
        {5e-16, 3e-8, 0.0, 2.9999999550000006e-08, 4.44602137450418e-26},
        {5e-16, 5e-7, 0.0, 4.999998750000417e-07, -1.9801357223218586e-23},
        {5e-16, 2e-6, 0.0, 1.9999980000026667e-06, -1.6536017667488005e-22},
        {4e-12, 2e-5, 0.0, 1.9999800002666627e-05, 1.3694019941860543e-21},
        {5e-12, 7e-4, 0.0, 0.0006997551142733419, 8.045441159182502e-21},
        {8e-14, 2.6e-3, 2e-20, 0.002596625847265978, 1.1029570690454358e-19},
        {8e-14, -2.6e-3, -3e-21, -0.002603385870114881, 1.2070558387143255e-20},

        {5e-16, -1.0, 2e-20, -45.35855467932097, -1.1302366343060957e-15},
        {5e-16, -0.5, -4.7e-18, -0.6931471805599453, -3.2590468138462995e-17},
        {5e-16, 1.5, 5e-19, 0.9162907318741551, -4.121195369011963e-17},
        {5e-16, 3.0, 0.0, 1.3862943611198906, 4.638093627692599e-17}
    };
    
    for (size_t i = 0; i < sizeof(samples)/sizeof(struct log1p_case); ++i) {
        struct log1p_case sample = samples[i];
        DoubleDouble x{sample.xhi, sample.xlo};
        auto y = x.log1p();
        std::stringstream s1, s2;
        s1 << "log1p case " << i << "  (upper)";
        s2 << "log1p case " << i << "  (lower)";
        assert_equal_fp(test, y.upper, sample.yhi, s1.str());
        assert_close_fp(test, y.lower, sample.ylo, sample.reltol, s2.str());
    }

    auto y = DoubleDouble(NAN).log1p();
    assert_isnan(test, y);
}

void test_exp(CheckIt& test)
{
    DoubleDouble y;
    auto x = DoubleDouble(10.0, 3e-18);
    auto t = DoubleDouble(3.7500950075008e-15, 0.0);

    y = DoubleDouble(0.0).exp();
    assert_equal_fp(test, y.upper, 1.0, "exp(0) (upper)");
    assert_equal_fp(test, y.lower, 0.0, "exp(0) (lower)");

    y = DoubleDouble(1.0).exp();
    assert_equal_fp(test, y.upper, 2.718281828459045, "exp(0) (upper)");
    assert_equal_fp(test, y.lower, 1.4456468917292502e-16, "exp(0) (lower)");

    y = x.exp();
    assert_equal_fp(test, y.upper, 22026.465794806718, "exp(x) (upper)");
    assert_equal_fp(test, y.lower, -1.3119340726673171e-12, "exp(x) (lower)");

    y = dd_pi.exp();
    assert_equal_fp(test, y.upper, 23.14069263277927, "exp(pi) (upper)");
    assert_equal_fp(test, y.lower, -1.3488747091995788e-15, "exp(pi) (lower)");

    y = (x - 10.0).exp();
    assert_equal_fp(test, y.upper, 1.0, "exp(x - 10) - 1 (upper)");
    assert_equal_fp(test, y.lower, 3e-18, "exp(x - 10) - 1 (lower)");

    y = t.exp();
    assert_equal_fp(test, y.upper, 1.0000000000000038, "exp(t) (upper)");
    assert_close_fp(test, y.lower, -2.4663276224724858e-17, 5e-16, "exp(t) (lower)");

    // The actual relative error for exp(709).lower is approx. 2e-14.
    // Can we do better?
    y = DoubleDouble(709.0).exp();
    assert_equal_fp(test, y.upper, 8.218407461554972e+307, "exp(709) upper");
    assert_close_fp(test, y.lower, -1.955965507696277e+291, 1e-13, "exp(709) lower");

    y = DoubleDouble(710.0).exp();
    assert_true(test, std::isinf(y.upper), "isinf(exp(710).upper)");
    assert_equal_fp(test, y.lower, 0.0, "exp(710).lower == 0");

    y = DoubleDouble(NAN).exp();
    assert_isnan(test, y);
}

void test_expm1(CheckIt& test)
{
    DoubleDouble y;
    auto t = DoubleDouble(3.7500950075008e-15, 0.0);

    y = DoubleDouble(11.75).expm1();
    assert_equal_fp(test, y.upper, 126752.55900574342, "expm1(11.75) upper");
    assert_close_fp(test, y.lower, 2.550992675486234e-12, 1e-15, "expm1(11.75) lower");

    y = DoubleDouble(1.0).expm1();
    assert_equal_fp(test, y.upper, 1.7182818284590453, "expm1(1) upper");
    assert_equal_fp(test, y.lower, -7.747991575210629e-17, "expm1(1) lower");

    y = DoubleDouble(0.46875).expm1();
    assert_equal_fp(test, y.upper, 0.5979954499506333, "expm1(0.46875) upper");
    assert_equal_fp(test, y.lower, 1.6864630310268093e-17, "expm1(0.46875) lower");

    y = t.expm1();
    assert_equal_fp(test, y.upper, 3.7500950075008074e-15, "expm1(t) upper");
    assert_equal_fp(test, y.lower, -6.814186434788356e-32, "expm1(t) lower");

    y = DoubleDouble(-0.000244140625).expm1();
    assert_equal_fp(test, y.upper, -0.00024411082510278348, "expm1(-2**-12) upper");
    assert_equal_fp(test, y.lower, -7.227720384831839e-21, "expm1(-2**-12) lower");

    y = DoubleDouble(-0.46875).expm1();
    assert_equal_fp(test, y.upper, -0.37421599039540887, "expm1(-0.46875) upper");
    assert_equal_fp(test, y.lower, -7.658883125910196e-18, "expm1(-0.46875) lower");

    y = DoubleDouble(-17.5).expm1();
    assert_equal_fp(test, y.upper, -0.9999999748900085, "expm1(-17.5) upper");
    assert_equal_fp(test, y.lower, 2.6947836187301293e-17, "expm1(-17.5) lower");

    y = DoubleDouble(NAN).expm1();
    assert_isnan(test, y);
}

void test_hypot(CheckIt& test)
{
    DoubleDouble x, y, h;

    x = DoubleDouble(3.0, 0.0);
    y = DoubleDouble(4.0, 0.0);
    h = hypot(x, y);
    assert_equal_fp(test, h.upper, 5.0, "hypot((3, 0), (4, 0)) (upper)");
    assert_equal_fp(test, h.lower, 0.0, "hypot((3, 0), (4, 0)) (lower)");

    x = DoubleDouble(0.0, 0.0);
    y = DoubleDouble(0.0, 0.0);
    h = hypot(x, y);
    assert_equal_fp(test, h.upper, 0.0, "hypot((0, 0), (0, 0)) (upper)");
    assert_equal_fp(test, h.lower, 0.0, "hypot((0, 0), (0, 0)) (lower)");

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
    assert_equal_fp(test, h.upper, 2.0615528128088305e+251, "hypot(BIG1, BIG2) (upper)");
    assert_close_fp(test, h.lower, -9.087579332326068e+234, 5e-16, "hypot(BIG1, BIG2) (lower)");

    x = DoubleDouble(3e-165, 5e-185);
    y = DoubleDouble(7.5e-165, -8e-184);
    h = hypot(x, y);
    assert_equal_fp(test, h.upper, 8.077747210701756e-165, "hypot(TINY1, TINY2) (upper)");
    assert_close_fp(test, h.lower, -5.79126190591365e-182, 1e-15, "hypot(TINY1, TINY2) (lower)");

    x = DoubleDouble(-17.25, -2.25e-19);
    y = DoubleDouble(0.0, 0.0);
    h = hypot(x, y);
    assert_equal_fp(test, h.upper, -x.upper, "hypot(x, (0, 0)) (upper)");
    assert_equal_fp(test, h.lower, -x.lower, "hypot(x, (0, 0)) (lower)");

    x = DoubleDouble(INFINITY);
    y = DoubleDouble(NAN);
    h = hypot(x, y);
    assert_equal_fp(test, h.upper, (double) INFINITY, "hypot(INF, NAN) is INF");
    h = hypot(y, x);
    assert_equal_fp(test, h.upper, (double) INFINITY, "hypot(INF, NAN) is INF");
}

void test_dsum(CheckIt& test)
{
    double data1[]{1.0, 3.0, 99.0};
    double s1 = dsum(3, data1);
    assert_equal_fp(test, s1, 103.0, "Test dsum([1.0, 2.0, 99.0])");

    double data2[]{1.0, 2.0, 2e-17, -2.0, 10.0, -1.0, -10.0};
    double s2 = dsum(7, data2);
    assert_equal_fp(test, s2, 2e-17, "Test of dsum()");

    std::vector<double> data3{1.0, 2.0, 2e-17, -2.0, 10.0, -1.0, -10.0};
    double s3 = dsum(data3);
    assert_equal_fp(test, s3, 2e-17, "Test of dsum(vector)");

    std::array<double, 7> data4{1.0, 2.0, 2e-17, -2.0, 10.0, -1.0, -10.0};
    double s4 = dsum(data4);
    assert_equal_fp(test, s4, 2e-17, "Test of dsum(vector)");
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
    test_comparisons(test);
    test_abs(test);
    test_powi(test);
    test_sqrt(test);
    test_log(test);
    test_log1p(test);
    test_exp(test);
    test_expm1(test);
    test_hypot(test);
    test_dsum(test);

    return test.print_summary("Summary: ");
}
