`DoubleDouble` is a C++ class that implements a double-double floating point type.

The `DoubleDouble` class defines:
* arithmetic operators: `+`, `-`, `*`, `/`
* inplace operators: `+=`, `-=`, `*=`, `/=`
* comparison operators: `==`, `!=` , `<`, `<=`, `>`, `>=`
* the functions: `abs`, `sqrt`, `powi`, `exp`, `expm1`, `log`
* several constants: `dd_e` (base of natural log), `dd_pi` (π),
  `dd_sqrt2` (sqrt(2)), and more.

C++17 is required to use the `DoubleDouble` class.

Example
-------

Here is a small program to compute the volume of a cone with base radius 2.0
and height 7.5.

```
#include <iostream>
#include <iomanip>
#include "doubledouble.h"

using namespace std;

// For DoubleDouble and dd_pi.
using namespace doubledouble;

int main()
{
    // Compute the volume of a cone with more precision than
    // is probably necessary.

    // Radius of the base of the cone.
    DoubleDouble r{2.0};

    // Height of the cone.
    DoubleDouble h{7.5};

    // Volume is pi*r^2*h/3.
    auto volume = dd_pi * r.powi(2) * h / 3;

    cout << setprecision(17) << "Volume of cone is " << volume.upper
         << " + " << scientific << volume.lower << endl;
}
```

When run, the program prints

```
Volume of cone is 31.415926535897931 + 1.22464679914735345e-15
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