`DoubleDouble` is a C++ class that implements a double-double floating point type.

The `DoubleDouble` class defines:
* arithmetic operators: `+`, `-`, `*`, `/`
* inplace operators: `+=`, `-=`, `*=`, `/=`
* comparison operators: `==`, `!=` , `<`, `<=`, `>`, `>=`
* the functions: `abs`, `sqrt`, `powi`, `exp`, `expm1`, `log`, `log1p`, `hypot`
* several constants: `dd_e` (base of natural log), `dd_pi` (π),
  `dd_sqrt2` (sqrt(2)), and more.

C++17 is required to use the `DoubleDouble` class.

The library must not be compiled with gcc's `-ffast-math` option or any
other compiler option that will allow the compiler to reorder or simplify
arithmetic expressions.

Example
-------

Here is a small program to compute the volume of a cone with base radius 2.0
and height 7.5.

```
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
```

When run, the program prints

```
Volume of cone is 31.41592653589793 + 1.2246467991473535e-15
```

For comparison, here is how that result could be computed in Python
with the [`mpmath`](https://mpmath.org/) library:

```
from mpmath import mp

mp.dps = 50
r = mp.mpf(2.0)
h = mp.mpf(7.5)
vol = mp.pi*r**2*h/3
print(float(vol), float(vol - float(vol)))
```
The Python script prints
```
31.41592653589793 1.2246467991473533e-15
```