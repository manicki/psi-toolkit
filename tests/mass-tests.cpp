#include "mass_tester.hpp"

/*!development,testing/900

Mass testing
============

Apart from regular unit testing with Boost Test, a framework for
so-called "mass testing" was created in PSI toolkit. In each instance
of mass testing the following elements must be specified:
* PSI pipeline to run
* input data
* expected output data.

A failure is reported when the actual output is different from the
expected output.

Running mass testing
--------------------

To run mass testing, use `mass-tests` program:
    ::cltu
    $ ./tests/mass-tests ..
    ...

The directories to analyze are given as arguments.

*/


int main(int argc, char* argv[]) {
    MassTester massTester(argc, argv);

    return massTester.run();
}
