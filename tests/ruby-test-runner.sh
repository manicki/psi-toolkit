#!/bin/bash

echo '========================================================'

RUBY_BINDINGS_DIR=`pwd`/bindings/ruby
PRJ_DIR=`pwd`/build

if [[ -e $PRJ_DIR/bindings/ruby/libpsitoolkit_ruby_simple.so ]]; then #FIXME: ?

    export RUBYPATH="$RUBYPATH:${PRJ_DIR}/bindings/ruby"

    echo 'Start testing Ruby bindings.'
    cd $PRJ_DIR
    ruby -I. -I$PRJ_DIR/bindings/ruby $RUBY_BINDINGS_DIR/test/test_psitoolkit.rb

else
    echo 'Ruby bindings NOT CREATED. Testing Ruby bindings ignored!'
fi

echo ''
echo '========================================================'
