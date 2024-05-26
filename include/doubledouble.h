//
// A double-double class.
// Copyright © 2022 Warren Weckesser
//
// MIT license:
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the “Software”), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//
// This is an incomplete translation and extension of the Python code
// "doubledouble.py" by Juraj Sukop (https://github.com/sukop/doubledouble).
// That file has the following license and copyright notice:
// # Copyright (c) 2017, Juraj Sukop
// #
// # Permission to use, copy, modify, and/or distribute this software for any
// # purpose with or without fee is hereby granted, provided that the above
// # copyright notice and this permission notice appear in all copies.
// #
// # THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
// # REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
// # AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
// # INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
// # LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
// # OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
// # PERFORMANCE OF THIS SOFTWARE.
//

#ifndef DOUBLEDOUBLE_H
#define DOUBLEDOUBLE_H

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cfloat>
#include <cstdint>
#include <array>
#include <vector>

namespace doubledouble {

class DoubleDouble
{
public:

    double upper{0.0};
    double lower{0.0};

    constexpr
    DoubleDouble() {}

    constexpr
    DoubleDouble(double upper, double lower) : upper(upper), lower(lower)
    {}

    constexpr
    DoubleDouble(double upper) : upper(upper)
    {}

    DoubleDouble operator-() const;
    DoubleDouble operator+(double x) const;
    DoubleDouble operator+(const DoubleDouble& x) const;
    DoubleDouble operator-(double x) const;
    DoubleDouble operator-(const DoubleDouble& x) const;
    DoubleDouble operator*(double x) const;
    DoubleDouble operator*(const DoubleDouble& x) const;
    DoubleDouble operator/(double x) const;
    DoubleDouble operator/(const DoubleDouble& x) const;

    DoubleDouble& operator+=(double x);
    DoubleDouble& operator+=(const DoubleDouble& x);
    DoubleDouble& operator-=(double x);
    DoubleDouble& operator-=(const DoubleDouble& x);
    DoubleDouble& operator*=(double x);
    DoubleDouble& operator*=(const DoubleDouble& x);
    DoubleDouble& operator/=(double x);
    DoubleDouble& operator/=(const DoubleDouble& x);

    bool operator==(const DoubleDouble& x) const;
    bool operator<(double x) const;
    bool operator<(const DoubleDouble& x) const;
    bool operator>(double x) const;
    bool operator>(const DoubleDouble& x) const;

    DoubleDouble powi(int n) const;
    DoubleDouble exp() const;
    DoubleDouble expm1() const;
    DoubleDouble log() const;
    DoubleDouble sqrt() const;
    DoubleDouble abs() const;
};

//
// Assorted predefined constants.
//

// sqrt(2)
inline const DoubleDouble dd_sqrt2{1.4142135623730951, -9.667293313452913e-17};
// sqrt(1/2)
inline const DoubleDouble dd_sqrt1_2{0.7071067811865476, -4.833646656726457e-17};
// e
inline const DoubleDouble dd_e{2.7182818284590452, 1.44564689172925013472e-16};
// ln(2)
inline const DoubleDouble dd_ln2{0.6931471805599453, 2.3190468138462996e-17};
// pi
inline const DoubleDouble dd_pi{3.1415926535897932, 1.22464679914735317636e-16};
// pi/2
inline const DoubleDouble dd_pi_2{1.5707963267948966, 6.123233995736766e-17};
// 1/pi
inline const DoubleDouble dd_1_pi{0.3183098861837907, -1.9678676675182486e-17};
// 1/sqrt(pi)
inline const DoubleDouble dd_1_sqrtpi{0.5641895835477563,7.66772980658294e-18};
// 2/sqrt(pi)
inline const DoubleDouble dd_2_sqrtpi{1.1283791670955126, 1.533545961316588e-17};
// sqrt(pi/2)
inline const DoubleDouble dd_sqrt_pi_2{1.2533141373155003, -9.164289990229583e-17};
// sqrt(2/pi)
inline const DoubleDouble dd_sqrt_2_pi{0.7978845608028654, -4.98465440455546e-17};


inline DoubleDouble two_sum_quick(double x, double y)
{
    double r = x + y;
    double e = y - (r - x);
    return DoubleDouble(r, e);
}


inline DoubleDouble two_sum(double x, double y)
{
    double r = x + y;
    double t = r - x;
    double e = (x - (r - t)) + (y - t);
    return DoubleDouble(r, e);
}


inline DoubleDouble two_difference(double x, double y)
{
    double r = x - y;
    double t = r - x;
    double e = (x - (r - t)) - (y + t);
    return DoubleDouble(r, e);
}


inline DoubleDouble two_product(double x, double y)
{
    double r = x*y;
    double e = fma(x, y, -r);
    return DoubleDouble(r, e);
}


inline DoubleDouble DoubleDouble::operator-() const
{
    return DoubleDouble(-upper, -lower);
}

inline DoubleDouble DoubleDouble::operator+(double x) const
{
    DoubleDouble re = two_sum(upper, x);
    re.lower += lower;
    return two_sum_quick(re.upper, re.lower);
}

inline DoubleDouble operator+(double x, const DoubleDouble& y)
{
    return y + x;
}

inline DoubleDouble DoubleDouble::operator+(const DoubleDouble& x) const
{
    DoubleDouble re = two_sum(upper, x.upper);
    re.lower += lower + x.lower;
    return two_sum_quick(re.upper, re.lower);
}

inline DoubleDouble DoubleDouble::operator-(double x) const
{
    DoubleDouble re = two_difference(upper, x);
    re.lower += lower;
    return two_sum_quick(re.upper, re.lower);
}

inline DoubleDouble operator-(double x, const DoubleDouble& y)
{
    return -y + x;
}

inline DoubleDouble DoubleDouble::operator-(const DoubleDouble& x) const
{
    DoubleDouble re = two_difference(upper, x.upper);
    re.lower += lower - x.lower;
    return two_sum_quick(re.upper, re.lower);
}

inline DoubleDouble DoubleDouble::operator*(double x) const
{
    DoubleDouble re = two_product(upper, x);
    re.lower += lower * x;
    return two_sum_quick(re.upper, re.lower);
}

inline DoubleDouble operator*(double x, const DoubleDouble& y)
{
    return y * x;
}

inline DoubleDouble DoubleDouble::operator*(const DoubleDouble& x) const
{
    DoubleDouble re = two_product(upper, x.upper);
    re.lower += upper*x.lower + lower*x.upper;
    return two_sum_quick(re.upper, re.lower);
}

inline DoubleDouble DoubleDouble::operator/(double x) const
{
    double r = upper/x;
    DoubleDouble sf = two_product(r, x);
    double e = (upper - sf.upper - sf.lower + lower)/x;
    return two_sum_quick(r, e);
}

inline DoubleDouble operator/(double x, const DoubleDouble& y)
{
    return DoubleDouble(x, 0.0) / y;
}

inline DoubleDouble DoubleDouble::operator/(const DoubleDouble& x) const
{
    double r = upper/x.upper;
    DoubleDouble sf = two_product(r, x.upper);
    double e = (upper - sf.upper - sf.lower + lower - r*x.lower)/x.upper;
    return two_sum_quick(r, e);
}

inline DoubleDouble& DoubleDouble::operator+=(double x)
{
    DoubleDouble re = two_sum(upper, x);
    re.lower += lower;
    *this = two_sum_quick(re.upper, re.lower);
    return *this;
}

inline DoubleDouble& DoubleDouble::operator+=(const DoubleDouble& x)
{
    DoubleDouble re = two_sum(upper, x.upper);
    re.lower += lower + x.lower;
    *this = two_sum_quick(re.upper, re.lower);
    return *this;
}

inline DoubleDouble& DoubleDouble::operator-=(double x)
{
    DoubleDouble re = two_difference(upper, x);
    re.lower += lower;
    *this = two_sum_quick(re.upper, re.lower);
    return *this;
}

inline DoubleDouble& DoubleDouble::operator-=(const DoubleDouble& x)
{
    DoubleDouble re = two_difference(upper, x.upper);
    re.lower += lower - x.lower;
    *this = two_sum_quick(re.upper, re.lower);
    return *this;
}

inline DoubleDouble& DoubleDouble::operator*=(double x)
{
    DoubleDouble re = two_product(upper, x);
    re.lower += lower * x;
    *this = two_sum_quick(re.upper, re.lower);
    return *this;
}

inline DoubleDouble& DoubleDouble::operator*=(const DoubleDouble& x)
{
    DoubleDouble re = two_product(upper, x.upper);
    re.lower += upper*x.lower + lower*x.upper;
    *this = two_sum_quick(re.upper, re.lower);
    return *this;
}

inline DoubleDouble& DoubleDouble::operator/=(double x)
{
    double r = upper/x;
    DoubleDouble sf = two_product(r, x);
    double e = (upper - sf.upper - sf.lower + lower)/x;
    *this = two_sum_quick(r, e);
    return *this;
}

inline DoubleDouble& DoubleDouble::operator/=(const DoubleDouble& x)
{
    double r = upper/x.upper;
    DoubleDouble sf = two_product(r, x.upper);
    double e = (upper - sf.upper - sf.lower + lower - r*x.lower)/x.upper;
    *this = two_sum_quick(r, e);
    return *this;
}

inline bool DoubleDouble::operator==(const DoubleDouble& x) const
{
    // XXX Do the (upper, lower) representations need to be canonicalized first?
    return (upper == x.upper) && (lower == x.lower);
}

inline bool DoubleDouble::operator<(const DoubleDouble& x) const
{
    // XXX Do the (upper, lower) representations need to be canonicalized first?
    return (upper < x.upper) || ((upper == x.upper) && (lower < x.lower));
}

inline bool DoubleDouble::operator<(double x) const
{
    // XXX Do the (upper, lower) representations need to be canonicalized first?
    return (upper < x) || ((upper == x) && (lower < 0.0));
}


inline bool DoubleDouble::operator>(const DoubleDouble& x) const
{
    // XXX Do the (upper, lower) representations need to be canonicalized first?
    return (upper > x.upper) || ((upper == x.upper) && (lower > x.lower));
}

inline bool DoubleDouble::operator>(double x) const
{
    // XXX Do the (upper, lower) representations need to be canonicalized first?
    return (upper > x) || ((upper == x) && (lower > 0.0));
}

inline DoubleDouble DoubleDouble::powi(int n) const
{
    int i = std::abs(n);
    DoubleDouble b = *this;
    DoubleDouble r(1);
    while (1) {
        if ((i & 1) == 1) {
            r = r * b;
        }
        if (i <= 1) {
            break;
        }
        i >>= 1;
        b = b*b;
    }
    if (n < 0) {
        return DoubleDouble(1) / r;
    }
    return r;
}


inline DoubleDouble DoubleDouble::exp() const
{
    if (upper > 709.782712893384) {
        return DoubleDouble(INFINITY, 0.0);
    }
    int n = int(round(upper));
    DoubleDouble x(upper - n, lower);
    DoubleDouble u = (((((((((((x +
                     156)*x + 12012)*x +
                     600600)*x + 21621600)*x +
                     588107520)*x + 12350257920)*x +
                     201132771840)*x + 2514159648000)*x +
                     23465490048000)*x + 154872234316800)*x +
                     647647525324800)*x + 1295295050649600;
    DoubleDouble v = (((((((((((x -
                     156)*x + 12012)*x -
                     600600)*x + 21621600)*x -
                     588107520)*x + 12350257920)*x -
                     201132771840)*x + 2514159648000)*x -
                     23465490048000)*x + 154872234316800)*x -
                     647647525324800)*x + 1295295050649600;
    return dd_e.powi(n) * (u / v);
}

inline DoubleDouble DoubleDouble::sqrt() const
{
    double r = std::sqrt(upper);
    DoubleDouble sf = two_product(r, r);
    double e = (upper - sf.upper - sf.lower + lower) * 0.5 / r;
    return two_sum_quick(r, e);
}

inline DoubleDouble DoubleDouble::log() const
{
    DoubleDouble r(std::log(upper));
    DoubleDouble u = r.exp();
    r = r - DoubleDouble(2.0)*(u - *this)/(u + *this);
    return r;
}

inline DoubleDouble DoubleDouble::abs() const
{
    if (*this < 0.0) {
        return -*this;
    }
    else {
        return *this;
    }
}

static const std::array<DoubleDouble, 10> numer{
    DoubleDouble(-0.028127670288085938, 1.46e-37),
    DoubleDouble(0.5127815691121048, -4.248816580490825e-17),
    DoubleDouble(-0.0632631785207471, 4.733650586348708e-18),
    DoubleDouble(0.01470328560687425, -4.57569727474415e-20),
    DoubleDouble(-0.0008675686051689528, 2.340010361165805e-20),
    DoubleDouble(8.812635961829116e-05, 2.619804163788941e-21),
    DoubleDouble(-2.596308786770631e-06, -1.6196413688647164e-22),
    DoubleDouble(1.422669108780046e-07, 1.2956999470135368e-23),
    DoubleDouble(-1.5995603306536497e-09, 5.185121944095551e-26),
    DoubleDouble(4.526182006900779e-11, -1.9856249941108077e-27)
};

static const std::array<DoubleDouble, 11> denom{
    DoubleDouble(1.0, 0.0),
    DoubleDouble(-0.4544126470907431, -2.2553855773661143e-17),
    DoubleDouble(0.09682713193619222, -4.961446925746919e-19),
    DoubleDouble(-0.012745248725908178, -6.0676821249478945e-19),
    DoubleDouble(0.001147361387158326, 1.3575817248483204e-20),
    DoubleDouble(-7.370416847725892e-05, 3.720369981570573e-21),
    DoubleDouble(3.4087499397791556e-06, -3.3067348191741576e-23),
    DoubleDouble(-1.1114024704296196e-07, -3.313361038199987e-24),
    DoubleDouble(2.3987051614110847e-09, 1.102474920537503e-25),
    DoubleDouble(-2.947734185911159e-11, -9.4795654767864e-28),
    DoubleDouble(1.32220659910223e-13, 6.440648413523595e-30)
};


//
// Rational approximation of expm1(x) for -1/2 < x < 1/2
//
inline DoubleDouble expm1_rational_approx(const DoubleDouble& x)
{
    const DoubleDouble Y = DoubleDouble(1.028127670288086, 0.0);
    const DoubleDouble num = (((((((((numer[9]*x + numer[8])
                                              *x + numer[7])
                                              *x + numer[6])
                                              *x + numer[5])
                                              *x + numer[4])
                                              *x + numer[3])
                                              *x + numer[2])
                                              *x + numer[1])
                                              *x + numer[0]);
    const DoubleDouble den = ((((((((((denom[10]*x + denom[9])
                                               *x + denom[8])
                                               *x + denom[7])
                                               *x + denom[6])
                                               *x + denom[5])
                                               *x + denom[4])
                                               *x + denom[3])
                                               *x + denom[2])
                                               *x + denom[1])
                                               *x + denom[0]);
    return x*Y + x * num/den;;
}


//
// This is a translation of Boost's `expm1_imp` for quad precision
// for use with DoubleDouble.
//

#define LOG_MAX_VALUE 709.782712893384

inline DoubleDouble DoubleDouble::expm1() const
{
    DoubleDouble a = (*this).abs();
    if (a.upper > 0.5) {
        if (a.upper > LOG_MAX_VALUE) {
            if (this->upper > 0) {
                // XXX Set overflow, and then return...
                return DoubleDouble(INFINITY, 0.0);
            }
            return DoubleDouble(-1.0, 0.0);
        }
        return (*this).exp() - 1.0;
    }
    // XXX Figure out the correct bound to use here...
    // if (a.upper < DOUBLEDOUBLE_EPSILON) {
    //    return (*this);
    // }
    return expm1_rational_approx(*this);
}

inline constexpr DoubleDouble operator "" _dd (long double x)
{
    return DoubleDouble(x, x - double(x));
}

//
// dsum() sums an array of doubles. DoubleDouble is used internally.
//

double dsum(size_t n, const double *x)
{
    DoubleDouble sum{0.0, 0.0};
    for (size_t i = 0; i < n; ++i) {
        sum = sum + x[i];
    }
    return sum.upper;
}

double dsum(const std::vector<double>& x)
{
    return dsum(x.size(), &x[0]);
}

template <std::size_t N>
double dsum(const std::array<double, N>& x)
{
    return dsum(x.size(), &x[0]);
}

} // namespace

#endif
