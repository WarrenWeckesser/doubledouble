#include "doubledouble.h"

#include <charconv>
#include <cstdio>
#include <system_error>

using namespace std;
using namespace doubledouble;

void
print_double(const double x)
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

int
main()
{
    DoubleDouble x = DoubleDouble(1.0) / 3.0;

    print_double(x.upper);
    printf(" + ");
    print_double(x.lower);
    printf("\n");
}
