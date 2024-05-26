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