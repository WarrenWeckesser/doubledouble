/*

CheckIt is a minimalist, header-only C++ class for unit tests.

The source is maintained at https://github.com/WarrenWeckesser/checkit

MIT license:

Copyright © 2022 Warren Weckesser

Permission is hereby granted, free of charge, to any person obtaining a copy of this
software and associated documentation files (the “Software”), to deal in the Software
without restriction, including without limitation the rights to use, copy, modify,
merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be included in all copies
or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#ifndef CHECKIT_H
#define CHECKIT_H

#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include <limits>
#include <typeinfo>
#include <iomanip>
#include <cmath>


class CheckIt {

    int num_assertions{0};
    int num_failed{0};
    std::ostream& errfile;

public:

    CheckIt(std::ostream &errfile) : errfile(errfile) {}

    int fprint_summary(std::ostream& out, const std::string& label)
    {
        out << label;
        out << "Assertions: " << num_assertions << "    Failures: " << num_failed << std::endl;
        return num_failed > 0;
    }

    int print_summary(const std::string& label)
    {
        return fprint_summary(std::cerr, label);
    }

    int print_summary(void)
    {
        return fprint_summary(std::cerr, "");
    }

    void log_failure(const std::string &details, const char* filename, int linenumber,
                     const std::string& msg)
    {
        errfile << "* failure:  " << details << "\n";
        errfile << "  location: " << filename << ":" << linenumber << "\n";
        errfile << "  message:  " << msg << std::endl;
    }

    void assert_true(int value, const std::string& msg, const char *filename, int linenumber)
    {
        num_assertions += 1;
        if (!value) {
            num_failed += 1;
            std::ostringstream ss;
            ss << "value is not true: " << value;
            auto details = ss.str();
            log_failure(details, filename, linenumber, msg);
        }
    }

    // Floating point equality comparison
    template<typename FP>
    void assert_equal_fp(FP value1, FP value2,
                         const std::string& msg, const char *filename, int linenumber)
    {
        num_assertions += 1;
        if (value1 != value2) {
            num_failed += 1;
            std::ostringstream ss;
            ss << "floating point (type " << typeid(FP).name() << ") not equal: "
               << std::setprecision(std::numeric_limits<FP>::max_digits10)
               << value1 << " ≠ " << value2;
            auto details = ss.str();
            log_failure(details, filename, linenumber, msg);
        }
    }

    // Floating point comparison based on allowed relative error.
    //
    // In this check, the second argument is the expected value.  This value is
    // considered the "true" value, and the test passes if the relative error of
    // `value` is less than or equal to the given tolerance.
    //
    // Because this test is based on relative error *only*, it must not be used
    // when the expected value is 0.
    //
    template<typename FP>
    void assert_close_fp(FP value, FP expected, FP reltol,
                         const std::string& msg, const char *filename, int linenumber)
    {
        num_assertions += 1;
        FP relerr = fabs(value - expected)/fabs(expected);
        if (relerr > reltol) {
            num_failed += 1;
            std::ostringstream ss;
            ss << "floating point (type " << typeid(FP).name() << ") not close: "
               << std::setprecision(std::numeric_limits<FP>::max_digits10)
               << value << " ≉ " << expected << "; relerr (" << relerr << ") < reltol (" << reltol << ")";
            auto details = ss.str();
            log_failure(details, filename, linenumber, msg);
        }
    }

    // Integer comparison
    template<typename Integer>
    void assert_equal_integer(Integer value1, Integer value2,
                              const std::string& msg, const char *filename, int linenumber)
    {
        num_assertions += 1;
        if (value1 != value2) {
            num_failed += 1;
            std::ostringstream ss;
            ss << "integers not equal: " << value1 << " ≠ " << value2;
            auto details = ss.str();
            log_failure(details, filename, linenumber, msg);
        }
    }

    void assert_equal_char(char value1, char value2,
                           const std::string& msg, const char *filename, int linenumber)
    {
        num_assertions += 1;
        if (value1 != value2) {
            num_failed += 1;
            std::ostringstream ss;
            ss << "chars not equal: '" << value1 << "' ≠ '" << value2 << "'";
            auto details = ss.str();
            log_failure(details, filename, linenumber, msg);
        }
    }

    void assert_equal_pointer(void *value1, void *value2,
                              const std::string& msg, const char *filename, int linenumber)
    {
        num_assertions += 1;
        if (value1 != value2) {
            num_failed += 1;
            log_failure("pointer values not equal", filename, linenumber, msg);

        }
    }

    void assert_equal_cstr(const char *value1, const char *value2,
                           const std::string& msg, const char *filename, int linenumber)
    {
        num_assertions += 1;
        if (strcmp(value1, value2) != 0) {
            num_failed += 1;
            std::ostringstream ss;
            ss << "cstr values not equal: '" << value1 << "' ≠ '" << value2 << "'";
            auto details = ss.str();
            log_failure(details, filename, linenumber, msg);
        }
    }
};


#define check_true(A, B, C)              A .assert_true(B, C, __FILE__, __LINE__)
#define check_equal_pointer(A, B, C, D)  A .assert_equal_pointer(B, C, D, __FILE__, __LINE__)
#define check_equal_cstr(A, B, C, D)     A .assert_equal_cstr(B, C, D, __FILE__, __LINE__)
#define check_equal_fp(A, B, C, D)       A .assert_equal_fp(B, C, D, __FILE__, __LINE__)
#define check_close_fp(A, B, C, D, E)    A .assert_close_fp(B, C, D, E, __FILE__, __LINE__)
#define check_equal_integer(A, B, C, D)  A .assert_equal_integer(B, C, D, __FILE__, __LINE__)
#define check_equal_char(A, B, C, D)     A .assert_equal_char(B, C, D, __FILE__, __LINE__)

#endif
