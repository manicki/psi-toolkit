#!/bin/bash

echo '========================================================'

PERL_BINDINGS_DIR=`pwd`/bindings/perl
PRJ_DIR=`pwd`/prj

if [[ -e $PRJ_DIR/libSimple.so ]]; then

echo 'Start testing Perl bindings.'
echo ''

PATH=.:${PRJ_DIR}:${PATH}
PERL5LIB=${PRJ_DIR}:${PERL5LIB}

cd $PRJ_DIR
perl $PERL_BINDINGS_DIR/Test.PL $PERL_BINDINGS_DIR

else
    echo 'Perl bindings NOT CREATED. Testing perl bindings ignored!'
fi

echo ''
echo '========================================================'
