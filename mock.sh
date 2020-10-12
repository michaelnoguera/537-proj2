#!/bin/bash
# Mock of the program used for testing

sed '/^.\{4096\}/d' | sed 's/ /\*/g' | sed 's/[a-z]/\U&/g'
wc -l