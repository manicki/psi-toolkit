#!/bin/bash

cd prj
./runner

valgrind --xml=yes --xml-file=valgrind.xml --suppressions=./memcpy.supp ./runner
xsltproc  ~/valgrind-reports-to-xunit/xslt/valgrind_transform.xsl valgrind.xml > valgrindTestResults.xml

cd ..
cppcheck -D__cplusplus -f --xml . --enable=all echo `find . -type d ! -path './.git*' ! -path './prj*' | perl -ne 'chomp; print "-I$_ "'`  2> cppcheck-result.xml

if ! type -P jruby > /dev/null; then
	echo -e "WARNING: jRuby has been not found!\n\n"
else
	cd server/t
	jruby runner.rb
	cd ../../
fi

