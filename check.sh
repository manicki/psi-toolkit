#!/bin/bash

# Arch packages needed for running this: python-distribute, python-nose

TARGET_DIR=build

mkdir -p $TARGET_DIR

cd $TARGET_DIR
make -j 6
../main-tests.sh

cd ..

# Warning: requires following perl packages:
# TAP::Formatter::JUnit
# TAP::Harness
./tests/perl-test-runner.sh
./tests/python-test-runner.sh

cd $TARGET_DIR

PATH="./framework:${PATH}" PERL5LIB="../../cltu:${PERL5LIB}" perl ../../cltu/cltu.pl ../framework/t/general.ush --log cltu.log

valgrind --xml=yes --xml-file=valgrind.xml --suppressions=../valgrind.supp ./tests/test_runner
xsltproc  ~/valgrind-reports-to-xunit/xslt/valgrind_transform.xsl valgrind.xml > valgrindTestResults.xml

cd ..
cppcheck -D__cplusplus -D__GNUC__=3 -f --xml . --enable=all echo `find . -type d ! -path './.git*' ! -path "./${TARGET_DIR}"'*' | perl -ne 'chomp; print "-I$_ "'` -i ${TARGET_DIR}/bindings/perl -i ${TARGET_DIR}/bindings/python 2> cppcheck-result.xml
find . -regextype posix-extended -regex '.*\.(cpp|hpp|h|c)' ! -regex '\./(build|server/mpfd-parser|utf8|maxent|sundown)/.*' ! -path './common/config.hpp' | vera++ - -profile psi -showrules 2>&1 | ./vera++2cppcheck.pl > vera++-result.xml

if ! type -P jruby > /dev/null; then
    echo -e "WARNING: jRuby has been not found!\n\n"
else
    cd server/t
    jruby runner.rb
    cd ../../
fi
