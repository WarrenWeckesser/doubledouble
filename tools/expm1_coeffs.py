import mpmath


mpmath.mp.dps = 200


def string_to_doubledouble(s):
    c = mpmath.mpf(s)
    upper = float(c)
    lower = float(c - float(c))
    return upper, lower


def print_doubledouble_list(dds):
    lines = ',\n'.join([f"    DoubleDouble({dd[0]}, {dd[1]})" for dd in dds])
    print(lines)


def print_doubledouble_array(name, dds):
    n = len(dds)
    print(f"static const std::array<DoubleDouble, {n}> {name}{{")
    print_doubledouble_list(dds)
    print('};')


def print_eval_poly(varname, n, arrayname, pad, suffix):
    expr = f"{arrayname}[{n-1}]"
    for k in range(n-2, -1, -1):
        expr = f"({expr}*x + {arrayname}[{k}])"
    # print(nexpr)
    parts = expr.split(')')[:-1]
    parts2 = [parts[0]] + [' '*(pad + n - 1 + len(arrayname)+3) + p
                           for p in parts[1:]]
    nexpr2 = ')\n'.join(parts2)
    nexpr2 = nexpr2 + ')' + suffix
    print(nexpr2)


# These are the coefficients for the rational approximation of the
# (detrended) function expm1(x) on the interval [-1/2, 1/2].

slope = string_to_doubledouble("0.10281276702880859375e1")

numer = [
    string_to_doubledouble("-0.28127670288085937499999999999999999854e-1"),
    string_to_doubledouble("0.51278156911210477556524452177540792214e0"),
    string_to_doubledouble("-0.63263178520747096729500254678819588223e-1"),
    string_to_doubledouble("0.14703285606874250425508446801230572252e-1"),
    string_to_doubledouble("-0.8675686051689527802425310407898459386e-3"),
    string_to_doubledouble("0.88126359618291165384647080266133492399e-4"),
    string_to_doubledouble("-0.25963087867706310844432390015463138953e-5"),
    string_to_doubledouble("0.14226691087800461778631773363204081194e-6"),
    string_to_doubledouble("-0.15995603306536496772374181066765665596e-8"),
    string_to_doubledouble("0.45261820069007790520447958280473183582e-10")
]

denom = [
    string_to_doubledouble("1.0"),
    string_to_doubledouble("-0.45441264709074310514348137469214538853e0"),
    string_to_doubledouble("0.96827131936192217313133611655555298106e-1"),
    string_to_doubledouble("-0.12745248725908178612540554584374876219e-1"),
    string_to_doubledouble("0.11473613871583259821612766907781095472e-2"),
    string_to_doubledouble("-0.73704168477258911962046591907690764416e-4"),
    string_to_doubledouble("0.34087499397791555759285503797256103259e-5"),
    string_to_doubledouble("-0.11114024704296196166272091230695179724e-6"),
    string_to_doubledouble("0.23987051614110848595909588343223896577e-8"),
    string_to_doubledouble("-0.29477341859111589208776402638429026517e-10"),
    string_to_doubledouble("0.13222065991022301420255904060628100924e-12"),
]


print_doubledouble_array("numer", numer)
print()
print_doubledouble_array("denom", denom)
print()

print('//')
print('// Rational approximation of expm1(x) for -1/2 < x < 1/2')
print('//')
print('inline DoubleDouble expm1_rational_approx(const DoubleDouble& x)')
print('{')
print(f'    const DoubleDouble Y = DoubleDouble({slope[0]}, {slope[1]});')
print('    const DoubleDouble num = ', end='')
print_eval_poly('x', len(numer), 'numer', pad=29, suffix=';')
print('    const DoubleDouble den = ', end='')
print_eval_poly('x', len(denom), 'denom', pad=29, suffix=';')
print('    return x*Y + x * num/den;;')
print('}')
