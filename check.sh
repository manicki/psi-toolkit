#!/bin/bash

# Arch packages needed for running this: python-distribute, python-nose

TARGET_DIR=build

mkdir -p $TARGET_DIR

cd $TARGET_DIR
make
./tests/test_runner --log_format=XML --log_level=all --log_sink=test-results.xml

./tests/mass-tests .. --xml mass-tests-results.xml

cd ..

# Warning: requires following perl packages:
# TAP::Formatter::JUnit
# TAP::Harness
./tests/perl-test-runner.sh

cd $TARGET_DIR

PATH=".:${PATH}" PERL5LIB="../../cltu:${PERL5LIB}" perl ../../cltu/cltu.pl ../framework/t/general.ush --log cltu.log

valgrind --xml=yes --xml-file=valgrind.xml --suppressions=../memcpy.supp ./tests/test_runner
xsltproc  ~/valgrind-reports-to-xunit/xslt/valgrind_transform.xsl valgrind.xml > valgrindTestResults.xml

cd ..
cppcheck -D__cplusplus -f --xml . --enable=all echo `find . -type d ! -path './.git*' ! -path "./${TARGET_DIR}"'*' | perl -ne 'chomp; print "-I$_ "'` -i ${TARGET_DIR}/bindings/perl 2> cppcheck-result.xml

if ! type -P jruby > /dev/null; then
    echo -e "WARNING: jRuby has been not found!\n\n"
else
    cd server/t
    jruby runner.rb
    cd ../../
fi

