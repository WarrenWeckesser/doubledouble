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
    // Compute the volume of a cone with DoubleDouble inputs.

    // Radius of the base of the cone.
    DoubleDouble r{2.0};

    // Height of the cone.
    DoubleDouble h{7.5};

    // Volume is pi*r^2*h/3.
    auto volume = dd_pi * r.powi(2) * h / 3;

    printf("Volume of cone is ");
    print_double(volume.upper);
    printf(" + ");
    print_double(volume.lower);
    printf("\n");
}
