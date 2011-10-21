#!/bin/bash

echo '========================================================'

PERL_BINDINGS_DIR=`pwd`/bindings/perl
PRJ_DIR=`pwd`/bin

if [[ -e $PRJ_DIR/libSimple.so ]]; then

echo 'Start testing Perl bindings.'
echo ''

PATH=.:${PRJ_DIR}:${PATH}
PERL5LIB=${PRJ_DIR}:${PERL5LIB}

TEST_OUTPUT_DIR=$PRJ_DIR/perl-test-results
if [[ -d $TEST_OUTPUT_DIR ]]; then
    rm -rf $TEST_OUTPUT_DIR
fi

mkdir -p $TEST_OUTPUT_DIR

export PERL_TEST_HARNESS_DUMP_TAP=$TEST_OUTPUT_DIR

cd $PRJ_DIR
perl $PERL_BINDINGS_DIR/Test.PL $PERL_BINDINGS_DIR

else
    echo 'Perl bindings NOT CREATED. Testing perl bindings ignored!'
fi

echo ''
echo '========================================================'
