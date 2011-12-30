#!/bin/bash

./tests/test_runner --log_format=XML --log_level=all --log_sink=test-results.xml
./tests/mass-tests .. --xml mass-tests-results.xml
