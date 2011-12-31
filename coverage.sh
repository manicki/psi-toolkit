#!/bin/bash

# Generating test coverage reports.

rm -rf gcov.output.dir

TARGET_DIR=build
cd $TARGET_DIR

find . -name '*.gcda' -exec rm '{}' ';'
rm -rf *.gcov

../main-tests.sh

perl ../rungcov.pl
gcovr -e '/usr' --xml > coverage-report.xml
