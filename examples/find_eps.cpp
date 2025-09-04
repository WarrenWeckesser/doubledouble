#include <cstdio>
#include <charconv>
#include <system_error>
#include "doubledouble.h"

using namespace std;

// For DoubleDouble and dd_pi.
using namespace doubledouble;

void print_double(const double x)
{
    char buf[24];
    const to_chars_result res = to_chars(buf, buf + sizeof(buf), x);
    if (res.ec == errc{}) {
        printf("%.*s", static_cast<int>(res.ptr - buf), buf);
    }
    else {
        printf("<to_chars() failed!>");
    }
}

int main()
{
    const DoubleDouble one{1.0};
    DoubleDouble eps{1.0};
    DoubleDouble halfeps;

    halfeps = eps/2.0;
    while (one + halfeps != one) {
        eps = halfeps;
        halfeps /= 2.0;
    }

    printf("eps = ");
    print_double(eps.upper);
    printf(" + ");
    print_double(eps.lower);
    printf("\n");

    DoubleDouble epsp1 = one + eps;
    printf("1 + eps = ");
    print_double(epsp1.upper);
    printf(" + ");
    print_double(epsp1.lower);
    printf("\n");
}
