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

The directories given as parameters are searched by `mass-tests` for
`m/` directories. Then each subdirectory of a given `m/` directory is
assumed to be a *batch test*.

Writing batch tests
-------------------

In a batch test directory, `WHAT` file containing a PSI pipeline
(without a ./psi-pipe command) must be given. This pipeline will be
applied to all files whose names are of the `BASENAME.in.FOO`, where
`FOO` is any extention (without a dot). The expected output file is
given as `BASENAME.out.BAR`, where `BOO` is any extention, not
necessarily the same as `FOO`.

*/

int main(int argc, char* argv[]) {
    MassTester massTester(argc, argv);

    return massTester.run();
}
