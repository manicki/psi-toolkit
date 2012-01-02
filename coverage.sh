#!/bin/bash

# Generating test coverage reports.

HEREDIR=`pwd`

rm -rf gcov.output.data

TARGET_DIR=build
cd $TARGET_DIR

find . -name '*.gcda' -exec rm '{}' ';'
rm -rf *.gcov coverage-report.xml

../main-tests.sh

perl ../rungcov.pl
gcovr -e '/usr' --xml | perl -pne 's{filename="'$HEREDIR'/}{filename="}' > coverage-report.xml
